/*
  Edge-AI motor vibration monitoring - ESP32 + MPU6050 (real hardware)

  Pipeline per window:
    MPU6050 1 kHz (synced to the sensor's data-ready flag) -> 1024 samples
    -> 12 features (edge_features.h) -> Random Forest (model.h)
    -> debounced alarm: red LED, buzzer, servo  -> MQTT status (~45 B)

  Wiring (report Table 3.1):
    MPU6050  VCC 3V3, GND, SDA GPIO21, SCL GPIO22 (addr 0x68, AD0 low)
    Red LED  GPIO27 -> 220R -> LED -> GND
    Buzzer   GPIO25 (active buzzer +), - to GND
    Servo    signal GPIO18, red to VIN, brown/black to GND
             (VIN = USB 5 V through the board's diode; see POWER notes in README_HARDWARE.md)

  Libraries: PubSubClient (Nick O'Leary), ESP32Servo (Kevin Harrington).
  Board: "ESP32 Dev Module".

  Experiment switches (report Section 5):
    TX_MODE 0  edge-only   : publish class only                    (proposed)
    TX_MODE 1  edge+cloud  : publish 12 features, cloud classifies  (E5)
    TX_MODE 2  cloud-only  : publish raw window, cloud classifies   (E5)
    SERIAL_RAW 1           : print every raw window for serial_logger.py (own-motor data, E7)
*/
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <Preferences.h>
#include "esp_system.h"
#include "edge_features.h"
#include "model.h"
#include "selftest_vectors.h"

// ============================== configuration ==============================
const char *WIFI_SSID     = "123456";
const char *WIFI_PASSWORD = "77777777";
const char *MQTT_HOST     = "broker.hivemq.com";
const int   MQTT_PORT     = 1883;
#define TOPIC_BASE "esp32motor_7f3a"          // status on TOPIC_BASE/status, etc.

#define TX_MODE        0      // 0 edge-only, 1 edge+cloud (features), 2 cloud-only (raw)
#define SERIAL_RAW     0      // 1 = dump raw windows over Serial (921600 baud) for data collection

#define MPU_ADDR       0x68
#define MPU_AXIS       2      // 0 = X, 1 = Y, 2 = Z  - pick the axis radial to the motor shaft
#define MPU_RANGE_G    2      // 2 = finest resolution (0.06 mg) for small motors; 4/8/16 for big vibration
#define SYNC_TO_SENSOR 1      // 1 = wait for MPU data-ready (true 1 kHz), 0 = ESP32 timer

#define PIN_LED        27
#define PIN_LED_OK     -1     // set to 26 if you also wired a green LED
#define PIN_BUZZER     25
#define PIN_SERVO      18
#define USE_SERVO      1
#define SERVO_NORMAL   0      // degrees
#define SERVO_ALARM    90
#define SERVO_SETTLE_MS 500   // servo moves, then is detached before the next window

#define ALARM_ON_WINDOWS  2   // consecutive fault windows needed to raise the alarm
#define ALARM_OFF_WINDOWS 2   // consecutive normal windows needed to clear it
#define MIN_CONFIDENCE    0.50f
#define CLOUD_TIMEOUT_MS  3000

// Healthy-motor calibration (stops "always fault" on a new motor / sensor mounting).
// The ESP32 learns the normal vibration level of YOUR motor, then only reports a fault
// when the window is clearly above that level. Send 'c' in Serial Monitor to recalibrate.
#define USE_CALIBRATION   1
#define CALIB_WINDOWS     15     // ~16 s of normal running
#define GATE_K            4.0f   // how many "normal spreads" above normal counts as abnormal
                                 // raise to 5-6 if you still get false alarms
// ===========================================================================

WiFiClient wifi;
PubSubClient mqtt(wifi);
Servo servo;

static float win[WIN_N];
static int16_t raw16[WIN_N];
static uint32_t seq = 0;

// metrics (E4/E5/E8)
static uint32_t i2c_errors = 0, publish_ok = 0, publish_fail = 0, reconnects = 0;
static uint64_t bytes_sent = 0;
static uint32_t offline_since = 0, last_reconnect_ms = 0, last_net_try = 0;
static float fs_measured = 0, jitter_max_us = 0, jitter_sd_us = 0;

// alarm state
static bool alarm_on = false;
static int fault_run = 0, normal_run = 0;

// cloud reply (TX_MODE 1/2)
static volatile int cloud_cls = -1;
static volatile uint32_t cloud_seq = 0;

// ------------------------------------------------------------- MPU6050
static bool mpu_write(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(MPU_ADDR); Wire.write(reg); Wire.write(val);
  return Wire.endTransmission() == 0;
}
static int mpu_read(uint8_t reg, uint8_t *buf, int n) {
  Wire.beginTransmission(MPU_ADDR); Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return 0;
  int got = Wire.requestFrom((int)MPU_ADDR, n);
  for (int i = 0; i < got; i++) buf[i] = Wire.read();
  return got;
}

static float g_per_lsb() { return MPU_RANGE_G / 32768.0f; }

static bool mpu_init() {
  uint8_t who = 0;
  if (mpu_read(0x75, &who, 1) != 1) { Serial.println("MPU6050 not responding on I2C (check SDA21/SCL22/3V3)"); return false; }
  Serial.printf("MPU WHO_AM_I = 0x%02X %s\n", who, who == 0x68 ? "(MPU6050)" : "(clone/variant - usually fine)");
  uint8_t fs_sel = MPU_RANGE_G == 2 ? 0 : MPU_RANGE_G == 4 ? 1 : MPU_RANGE_G == 8 ? 2 : 3;
  bool ok = mpu_write(0x6B, 0x01)              // wake, PLL clock from gyro X
         && mpu_write(0x1A, 0x00)              // DLPF_CFG 0: accel bandwidth 260 Hz, accel ODR 1 kHz
         && mpu_write(0x19, 0x00)              // SMPLRT_DIV 0
         && mpu_write(0x1C, fs_sel << 3)       // accel full-scale range
         && mpu_write(0x38, 0x01);             // DATA_RDY interrupt flag enabled (we poll it)
  delay(50);
  return ok;
}

// Fills win[] (in g) and raw16[]; measures real sample rate and jitter (O1 / E4).
static void acquire_window() {
  const uint8_t reg = 0x3B + 2 * MPU_AXIS;
  uint8_t b[2];
  uint32_t t_prev = 0, t_first = 0;
  double sum_dt = 0, sum_dt2 = 0; float max_dev = 0;
#if !SYNC_TO_SENSOR
  uint32_t next = micros();
#endif
  for (int i = 0; i < WIN_N; i++) {
#if SYNC_TO_SENSOR
    uint32_t t_wait = micros(); uint8_t st = 0;
    do { mpu_read(0x3A, &st, 1); } while (!(st & 0x01) && micros() - t_wait < 5000);
#else
    while ((int32_t)(micros() - next) < 0) {}
    next += 1000000UL / FS;
#endif
    uint32_t t = micros();
    if (mpu_read(reg, b, 2) == 2) raw16[i] = (int16_t)((b[0] << 8) | b[1]);
    else { i2c_errors++; raw16[i] = i ? raw16[i - 1] : 0; }
    win[i] = raw16[i] * g_per_lsb();
    if (i == 0) t_first = t;
    else {
      double dt = (double)(t - t_prev);
      sum_dt += dt; sum_dt2 += dt * dt;
      float dev = fabsf((float)dt - 1e6f / FS);
      if (dev > max_dev) max_dev = dev;
    }
    t_prev = t;
  }
  double mean_dt = sum_dt / (WIN_N - 1);
  fs_measured = 1e6 / mean_dt;
  jitter_sd_us = sqrt(fmax(0.0, sum_dt2 / (WIN_N - 1) - mean_dt * mean_dt));
  jitter_max_us = max_dev;
  (void)t_first;
}

// ------------------------------------------------------------- outputs
static void buzz(int times, int on_ms = 120, int off_ms = 100) {
  for (int i = 0; i < times; i++) { digitalWrite(PIN_BUZZER, HIGH); delay(on_ms); digitalWrite(PIN_BUZZER, LOW); delay(off_ms); }
}

// Servo is powered from VIN. Move it only on an alarm change, then detach so it
// stops drawing holding current and stops shaking the sensor before the next window.
static void servo_to(int deg) {
#if USE_SERVO
  servo.attach(PIN_SERVO, 500, 2400);
  servo.write(deg);
  delay(SERVO_SETTLE_MS);
  servo.detach();
  digitalWrite(PIN_SERVO, LOW);
#endif
}

// ------------------------------------------------------------- healthy-motor calibration
#define CLS_ANOMALY RF_N_CLASSES                       // "abnormal, but the model can't name it"
static const int GATE_FEAT[7] = {0, 3, 7, 8, 9, 10, 11}; // rms, kurtosis, 5 bands
static float cal_mu[7], cal_sd[7];
static bool calib_ok = false;
static float gate_last = 0;
Preferences prefs;

static const char *cls_name(int c) { return c == CLS_ANOMALY ? "anomaly" : RF_CLASS_NAMES[c]; }

static void calibrate() {
  Serial.println("CALIBRATING: run the motor NORMALLY (no fault) and don't touch it for ~16 s...");
  double s1[7] = {0}, s2[7] = {0};
  float f[N_FEAT];
  for (int w = 0; w < CALIB_WINDOWS; w++) {
    digitalWrite(PIN_LED, w & 1);
    acquire_window();
    compute_features(win, f);
    for (int j = 0; j < 7; j++) { double v = f[GATE_FEAT[j]]; s1[j] += v; s2[j] += v * v; }
  }
  digitalWrite(PIN_LED, LOW);
  for (int j = 0; j < 7; j++) {
    double mu = s1[j] / CALIB_WINDOWS, sd = sqrt(fmax(0.0, s2[j] / CALIB_WINDOWS - mu * mu));
    cal_mu[j] = mu;
    cal_sd[j] = fmax(sd, 0.10 * fabs(mu) + 1e-5);       // floor: avoids alarms from a too-tight spread
  }
  prefs.begin("calib", false);
  prefs.putBytes("mu", cal_mu, sizeof cal_mu);
  prefs.putBytes("sd", cal_sd, sizeof cal_sd);
  prefs.end();
  calib_ok = true;
  Serial.printf("CALIBRATED: normal rms=%.4f g (sd %.4f), kurtosis=%.2f. Saved - survives reboot.\n",
                cal_mu[0], cal_sd[0], cal_mu[1]);
  if (cal_mu[0] < 0.003f)
    Serial.println("  note: very low vibration - is the motor running and the sensor screwed/glued to it?");
  buzz(2, 40);
}

static bool load_calibration() {
  prefs.begin("calib", true);
  bool ok = prefs.getBytesLength("mu") == sizeof cal_mu && prefs.getBytesLength("sd") == sizeof cal_sd;
  if (ok) { prefs.getBytes("mu", cal_mu, sizeof cal_mu); prefs.getBytes("sd", cal_sd, sizeof cal_sd); }
  prefs.end();
  if (ok) Serial.printf("Calibration loaded: normal rms=%.4f g. Send 'c' to redo it.\n", cal_mu[0]);
  return ok;
}

// How far above healthy is this window? (max over features, in units of the normal spread)
static float gate_score(const float *f) {
  float worst = -1e9f;
  for (int j = 0; j < 7; j++) {
    float z = (f[GATE_FEAT[j]] - cal_mu[j]) / cal_sd[j];
    if (z > worst) worst = z;
  }
  return worst;
}

static void serial_commands() {
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'c' || ch == 'C') calibrate();
  }
}

// Debounced alarm decision. LED updates immediately; returns true if the state changed.
static bool decide_alarm(int cls, float conf) {
  bool fault = (cls != 0) && (cls == CLS_ANOMALY || conf >= MIN_CONFIDENCE);
  if (fault) { fault_run++; normal_run = 0; } else { normal_run++; fault_run = 0; }
  bool was = alarm_on;
  if (!alarm_on && fault_run >= ALARM_ON_WINDOWS) alarm_on = true;
  if (alarm_on && normal_run >= ALARM_OFF_WINDOWS) alarm_on = false;
  digitalWrite(PIN_LED, alarm_on);
  if (PIN_LED_OK >= 0) digitalWrite(PIN_LED_OK, !alarm_on);
  return alarm_on != was;
}

// Buzzer + servo only on a state change, after the MQTT message has gone out.
static void actuate_change() {
  if (alarm_on) { buzz(3); servo_to(SERVO_ALARM); }
  else          { buzz(1, 40); servo_to(SERVO_NORMAL); }
}

// ------------------------------------------------------------- network (never blocks the alert path)
static void on_mqtt(char *topic, byte *payload, unsigned int len) {
  // cloud reply: "<seq>,<class>"
  char buf[32]; unsigned n = len < sizeof(buf) - 1 ? len : sizeof(buf) - 1;
  memcpy(buf, payload, n); buf[n] = 0;
  unsigned long s; int c;
  if (sscanf(buf, "%lu,%d", &s, &c) == 2) { cloud_seq = s; cloud_cls = c; }
}

static bool net_up() { return WiFi.status() == WL_CONNECTED && mqtt.connected(); }

static void net_service() {
  if (net_up()) { mqtt.loop(); return; }
  if (offline_since == 0) offline_since = millis();
  if (millis() - last_net_try < 5000) return;          // retry every 5 s, don't stall the loop
  last_net_try = millis();
  if (WiFi.status() != WL_CONNECTED) { WiFi.disconnect(); WiFi.begin(WIFI_SSID, WIFI_PASSWORD); return; }
  String id = "esp32-motor-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  if (mqtt.connect(id.c_str())) {
    mqtt.subscribe(TOPIC_BASE "/result");
    reconnects++;
    last_reconnect_ms = millis() - offline_since;
    Serial.printf("NET up (outage %lu ms)\n", (unsigned long)last_reconnect_ms);
    offline_since = 0;
  }
}

static bool publish(const char *topic, const uint8_t *data, unsigned len) {
  if (!net_up()) { publish_fail++; return false; }
  bool ok = mqtt.publish(topic, data, len);
  if (ok) { publish_ok++; bytes_sent += len + strlen(topic) + 4; } else publish_fail++;
  return ok;
}

#if TX_MODE != 0
// Wait for the cloud's decision (TX_MODE 1/2). Returns class or -1 on timeout.
static int wait_cloud(uint32_t s) {
  uint32_t t0 = millis();
  while (millis() - t0 < CLOUD_TIMEOUT_MS) {
    mqtt.loop();
    if (cloud_seq == s && cloud_cls >= 0) return cloud_cls;
    delay(1);
  }
  return -1;
}
#endif

// ------------------------------------------------------------- self-test (E6)
static void self_test() {
  float f[N_FEAT], p[RF_N_CLASSES], maxrel = 0;
  compute_features(ST_RAW, f);
  for (int j = 0; j < N_FEAT; j++) {
    float r = fabsf(f[j] - ST_RAW_FEAT[j]) / (fabsf(ST_RAW_FEAT[j]) + 1e-6f);
    if (r > maxrel) maxrel = r;
  }
  int match = 0;
  uint32_t t0 = micros();
  for (int i = 0; i < ST_N_VEC; i++) match += rf_predict(ST_X[i], p) == ST_Y[i];
  uint32_t per = (micros() - t0) / ST_N_VEC;
  Serial.printf("SELFTEST features max_rel_err=%.2e  model %d/%d match Python  (%lu us/inference)\n",
                maxrel, match, ST_N_VEC, (unsigned long)per);
}

// ------------------------------------------------------------- setup / loop
void setup() {
  Serial.begin(SERIAL_RAW ? 921600 : 115200);
  delay(300);
  esp_reset_reason_t rr = esp_reset_reason();
  Serial.printf("\nBoot. reset reason=%d%s\n", (int)rr,
                rr == ESP_RST_BROWNOUT ? "  <-- BROWNOUT: supply sagged (servo/Wi-Fi peak). See POWER notes." : "");
  if (MODEL_IS_PLACEHOLDER) Serial.println("WARNING: model.h is a PLACEHOLDER (synthetic). Run train_edge.py on real data.");
  Serial.printf("Model: %d trees, %d nodes, ~%d B flash.  Window %d @ %d Hz\n", RF_N_TREES, RF_N_NODES, RF_MODEL_BYTES, WIN_N, FS);

  pinMode(PIN_LED, OUTPUT); pinMode(PIN_BUZZER, OUTPUT);
  if (PIN_LED_OK >= 0) pinMode(PIN_LED_OK, OUTPUT);
  digitalWrite(PIN_LED, HIGH); buzz(1, 60); digitalWrite(PIN_LED, LOW);   // power-on check
  servo_to(SERVO_NORMAL);

  features_init();
  self_test();

  Wire.begin(21, 22); Wire.setClock(400000);
  while (!mpu_init()) { digitalWrite(PIN_LED, !digitalRead(PIN_LED)); delay(500); }
#if USE_CALIBRATION
  calib_ok = load_calibration();
  if (!calib_ok) calibrate();                            // first boot: learn the healthy motor
#endif

  // Wi-Fi after the servo has moved: avoids stacking the two current peaks on VIN at boot
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(on_mqtt);
  mqtt.setSocketTimeout(2);
  mqtt.setBufferSize(2 * WIN_N + 64);
  offline_since = millis();
  Serial.println("WIN,seq,class,conf,alarm,gate,rf_class,rms_g,peak_g,raw_span,feat_us,inf_us,fs_meas,jit_sd_us,jit_max_us,heap,min_heap,net,pub_ok,pub_fail,bytes,i2c_err");
}

void loop() {
  serial_commands();
  net_service();

  acquire_window();                                      // ~1.024 s
  uint32_t t_end = micros();

  float f[N_FEAT], p[RF_N_CLASSES];
  compute_features(win, f);
  uint32_t t_feat = micros();
  int rf_cls = rf_predict(f, p);
  int cls = rf_cls;
  float conf = p[rf_cls];
#if USE_CALIBRATION
  if (calib_ok) {
    gate_last = gate_score(f);
    if (gate_last < GATE_K) { cls = 0; conf = 1.0f; }          // looks like the healthy motor
    else if (rf_cls == 0)   { cls = CLS_ANOMALY; conf = 1.0f; } // abnormal, model can't name it
  }
#endif
  uint32_t t_inf = micros();
  seq++;

  bool changed = decide_alarm(cls, conf);             // local alert works with or without network

  net_service();
  char msg[400]; int n = 0; (void)msg;
#if TX_MODE == 0
  n = snprintf(msg, sizeof msg, "{\"q\":%lu,\"s\":\"%s\",\"c\":%.2f,\"a\":%d}",
               (unsigned long)seq, cls_name(cls), conf, alarm_on);
  publish(TOPIC_BASE "/status", (uint8_t *)msg, n);
#else
#if TX_MODE == 1
  n = snprintf(msg, sizeof msg, "%lu", (unsigned long)seq);
  for (int j = 0; j < N_FEAT; j++) n += snprintf(msg + n, sizeof msg - n, ",%.6g", f[j]);
  uint32_t t_tx = micros();
  publish(TOPIC_BASE "/features", (uint8_t *)msg, n);
#else
  static uint8_t pkt[4 + 2 * WIN_N];
  memcpy(pkt, &seq, 4); memcpy(pkt + 4, raw16, 2 * WIN_N);          // little-endian int16 counts
  uint32_t t_tx = micros();
  n = sizeof pkt;
  publish(TOPIC_BASE "/raw", pkt, n);
#endif
  int ccls = wait_cloud(seq);
  uint32_t t_reply = micros();
  // cloud detection latency = window end -> decision back on the device (compare with edge feat+inf time)
  Serial.printf("CLOUD,%lu,%s,net_rtt_ms=%.1f,cloud_latency_ms=%.1f,edge_latency_ms=%.2f,edge_says=%s\n",
                (unsigned long)seq, ccls >= 0 ? RF_CLASS_NAMES[ccls] : "timeout",
                (t_reply - t_tx) / 1000.0f, (t_reply - t_end) / 1000.0f, (t_inf - t_end) / 1000.0f,
                cls_name(cls));
#endif

  if (changed) actuate_change();

  int16_t rmin = 32767, rmax = -32768;
  for (int i = 0; i < WIN_N; i++) { if (raw16[i] < rmin) rmin = raw16[i]; if (raw16[i] > rmax) rmax = raw16[i]; }
  if (rmax - rmin < 4) Serial.println("SENSOR FLAT: readings barely change - check wiring/axis, is the motor on?");
  Serial.printf("WIN,%lu,%s,%.2f,%d,%.1f,%s,%.4f,%.4f,%d,%lu,%lu,%.1f,%.1f,%.0f,%lu,%lu,%d,%lu,%lu,%llu,%lu\n",
                (unsigned long)seq, cls_name(cls), conf, alarm_on, gate_last, RF_CLASS_NAMES[rf_cls], f[0], f[1], (int)(rmax - rmin),
                (unsigned long)(t_feat - t_end), (unsigned long)(t_inf - t_feat),
                fs_measured, jitter_sd_us, jitter_max_us,
                (unsigned long)ESP.getFreeHeap(), (unsigned long)ESP.getMinFreeHeap(), net_up(),
                (unsigned long)publish_ok, (unsigned long)publish_fail, (unsigned long long)bytes_sent,
                (unsigned long)i2c_errors);
#if SERIAL_RAW
  Serial.printf("RAW,%lu,%.2f,%.6f", (unsigned long)seq, fs_measured, g_per_lsb());
  for (int i = 0; i < WIN_N; i++) Serial.printf(",%d", raw16[i]);
  Serial.println();
#endif
}
