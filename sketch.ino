/*
  Edge-AI motor fault detection - ESP32 (Wokwi simulation)
  Pipeline: sample window -> 12 features -> Random Forest -> publish ~30 B JSON over MQTT.

  Wokwi cannot produce real vibration, so SIM mode synthesises the motor signal in firmware.
  Turn the potentiometer to choose the injected fault:
     0 normal | 1 imbalance | 2 misalignment | 3 bearing | 4 looseness
  For real hardware set USE_REAL_MPU 1 (MPU6050 on I2C 21/22), nothing else changes.
*/
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "edge_features.h"
#include "model.h"

#define USE_REAL_MPU 0
#define POT_PIN 34
#define LED_OK 26
#define LED_FAULT 27
#define F0 25.0f

const char *WIFI_SSID = "Wokwi-GUEST";
const char *MQTT_HOST = "broker.hivemq.com";
const char *MQTT_TOPIC = "yourname_motor_edge/status";   // CHANGE to something unique
const char *NAMES[5] = {"normal", "imbalance", "misalignment", "bearing", "looseness"};

WiFiClient wifi;
PubSubClient mqtt(wifi);
float win[WIN_N];

float rnd(float a, float b) { return a + (b - a) * (random(0, 10000) / 10000.0f); }
float gauss(float sd) {                                   // Box-Muller
  float u1 = rnd(0.0001f, 1), u2 = rnd(0, 1);
  return sd * sqrtf(-2 * logf(u1)) * cosf(2 * (float)M_PI * u2);
}

void synth_window(int cls) {                              // mirrors data_gen.py
  float f0 = F0 * rnd(0.985f, 1.015f), a = rnd(0.85f, 1.15f), sig = rnd(0.1f, 0.16f);
  float amp[4] = {0, 0, 0, 0}, mult[4] = {0.5f, 1, 2, 3}, ph[4];
  for (int k = 0; k < 4; k++) ph[k] = rnd(0, 6.2832f);
  if (cls == 0) amp[1] = 0.5f;
  if (cls == 1) amp[1] = 1.6f;
  if (cls == 2) { amp[1] = 0.6f; amp[2] = 0.9f; amp[3] = 0.4f; }
  if (cls == 3) amp[1] = 0.5f;
  if (cls == 4) { amp[0] = 0.5f; amp[1] = 0.7f; amp[2] = 0.5f; amp[3] = 0.4f; sig *= 2; }
  for (int i = 0; i < WIN_N; i++) {
    float t = (float)i / FS, v = gauss(sig);
    for (int k = 0; k < 4; k++) v += a * amp[k] * sinf(2 * (float)M_PI * mult[k] * f0 * t + ph[k]);
    win[i] = v;
  }
  if (cls == 3) {                                         // bearing impulses
    float period = 1.0f / (3.58f * f0);
    for (float tk = rnd(0, period); tk < (float)WIN_N / FS; tk += period) {
      int i0 = (int)(tk * FS);
      for (int j = 0; j < 60 && i0 + j < WIN_N; j++) {
        float tau = (float)j / FS;
        win[i0 + j] += a * 1.5f * expf(-tau * 600) * sinf(2 * (float)M_PI * 450 * tau);
      }
    }
  }
}

#if USE_REAL_MPU
void acquire_real() {                                     // 2 kHz sampling of MPU6050 Z-axis (in g)
  unsigned long next = micros();
  for (int i = 0; i < WIN_N; i++) {
    while ((long)(micros() - next) < 0) {}
    next += 1000000UL / FS;
    Wire.beginTransmission(0x68); Wire.write(0x3F); Wire.endTransmission(false);
    Wire.requestFrom(0x68, 2);
    int16_t raw = (Wire.read() << 8) | Wire.read();
    win[i] = raw / 16384.0f;                              // +/-2 g range
  }
}
#endif

void connect_net() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, "", 6);
    while (WiFi.status() != WL_CONNECTED) delay(200);
  }
  while (!mqtt.connected()) {
    mqtt.connect(("esp32-" + String(random(0xffff), HEX)).c_str());
    if (!mqtt.connected()) delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_OK, OUTPUT); pinMode(LED_FAULT, OUTPUT);
#if USE_REAL_MPU
  Wire.begin(21, 22); Wire.setClock(400000);
  Wire.beginTransmission(0x68); Wire.write(0x6B); Wire.write(0); Wire.endTransmission();
#endif
  mqtt.setServer(MQTT_HOST, 1883);
  randomSeed(esp_random());
  connect_net();
}

void loop() {
  if (!mqtt.connected()) connect_net();
  mqtt.loop();

#if USE_REAL_MPU
  acquire_real();
#else
  int injected = constrain(analogRead(POT_PIN) * 5 / 4096, 0, 4);
  synth_window(injected);
#endif

  unsigned long t0 = micros();
  float f[RF_N_FEATURES], p[RF_N_CLASSES];
  compute_features(win, f);
  int cls = rf_predict(f, p);
  unsigned long t_us = micros() - t0;                     // <-- use this in edge_vs_cloud.py (EDGE_MS_ESP32)

  digitalWrite(LED_OK, cls == 0); digitalWrite(LED_FAULT, cls != 0);

  char msg[64];
  int n = snprintf(msg, sizeof msg, "{\"s\":\"%s\",\"c\":%.2f}", NAMES[cls], p[cls]);
  mqtt.publish(MQTT_TOPIC, msg);
  Serial.printf("state=%-12s conf=%.2f  compute=%lu us  payload=%d B\n", NAMES[cls], p[cls], t_us, n);
  delay(1000);
}
