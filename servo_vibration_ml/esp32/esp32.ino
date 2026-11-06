// Experimental servo vibration monitor. It moves a servo, samples an MPU6050
// at 200 Hz, classifies one-second windows, and alerts on non-NORMAL classes.
// Validate the synthetic-heavy model against real servo trials before use.
#include <Wire.h>
#include <string.h>
#include "model.h"
#include "servo_features.h"

static const uint8_t MPU_ADDR = 0x68;
static const uint8_t SDA_PIN = 21;
static const uint8_t SCL_PIN = 22;
static const uint8_t SERVO_PIN = 18;
static const uint8_t LED_PIN = 2;
static const uint8_t BUZZER_PIN = 25;
static const int WINDOW_SIZE = 200;
static const uint32_t SAMPLE_PERIOD_US = 5000;
static const float ACCEL_LSB_PER_G = 16384.0f;
static const int CONFIRM_WINDOWS = 2;
static const int SERVO_STEP_DEG = 2;
static const int SERVO_MIN_DEG = 45;
static const int SERVO_MAX_DEG = 135;

float samples[WINDOW_SIZE][3];
float features[SERVO_RF_FEATURES];
int servoAngle = SERVO_MIN_DEG;
int servoDirection = 1;
int nonNormalWindows = 0;
int normalWindows = 0;
bool alarmActive = false;
uint32_t lastServoUpdate = 0;

bool writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool readRegister(uint8_t reg, uint8_t *data, size_t length) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)length) != length) return false;
  for (size_t i = 0; i < length; ++i) data[i] = Wire.read();
  return true;
}

bool readAccel(float &ax, float &ay, float &az) {
  uint8_t bytes[6];
  if (!readRegister(0x3B, bytes, sizeof(bytes))) return false;
  const int16_t x = (int16_t)((bytes[0] << 8) | bytes[1]);
  const int16_t y = (int16_t)((bytes[2] << 8) | bytes[3]);
  const int16_t z = (int16_t)((bytes[4] << 8) | bytes[5]);
  ax = x / ACCEL_LSB_PER_G;
  ay = y / ACCEL_LSB_PER_G;
  az = z / ACCEL_LSB_PER_G;
  return true;
}

void updateServo() {
  const uint32_t now = millis();
  if (now - lastServoUpdate < 20) return;
  lastServoUpdate = now;
  servoAngle += servoDirection * SERVO_STEP_DEG;
  if (servoAngle >= SERVO_MAX_DEG) { servoAngle = SERVO_MAX_DEG; servoDirection = -1; }
  if (servoAngle <= SERVO_MIN_DEG) { servoAngle = SERVO_MIN_DEG; servoDirection = 1; }
  // ESP32 Arduino core 3.x LEDC PWM at 50 Hz; 500–2400 us maps to 0–180°.
  const uint32_t pulseUs = 500 + (1900UL * (uint32_t)servoAngle) / 180UL;
  const uint32_t duty = (pulseUs * 65535UL) / 20000UL;
  ledcWrite(SERVO_PIN, duty);
}

void setAlarm(bool on) {
  alarmActive = on;
  digitalWrite(LED_PIN, on ? HIGH : LOW);
  digitalWrite(BUZZER_PIN, on ? HIGH : LOW); // use an active buzzer module
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  setAlarm(false);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  uint8_t who = 0;
  if (!readRegister(0x75, &who, 1) || (who != 0x68 && who != 0x69)) {
    Serial.println("ERROR: MPU6050 not found");
    while (true) { setAlarm(true); delay(250); setAlarm(false); delay(250); }
  }
  // ±2 g, DLPF enabled, 1 kHz base / (1 + 4) = 200 Hz sample rate.
  if (!writeRegister(0x6B, 0x00) || !writeRegister(0x1C, 0x00) ||
      !writeRegister(0x1A, 0x01) || !writeRegister(0x19, 0x04)) {
    Serial.println("ERROR: MPU6050 setup failed");
    while (true) delay(1000);
  }
  if (!ledcAttach(SERVO_PIN, 50, 16)) {
    Serial.println("ERROR: servo PWM setup failed");
    while (true) delay(1000);
  }
  const uint32_t initialPulseUs = 500 + (1900UL * (uint32_t)servoAngle) / 180UL;
  ledcWrite(SERVO_PIN, (initialPulseUs * 65535UL) / 20000UL);
  Serial.printf("READY,servo-monitor,200Hz,window=%d\n", WINDOW_SIZE);
}

void loop() {
  uint32_t nextSample = micros();
  for (int i = 0; i < WINDOW_SIZE; ++i) {
    updateServo();
    while ((int32_t)(micros() - nextSample) < 0) {
      updateServo();
      delayMicroseconds(200);
    }
    nextSample += SAMPLE_PERIOD_US;
    if (!readAccel(samples[i][0], samples[i][1], samples[i][2])) {
      Serial.println("ERROR: sensor read failed; window discarded");
      return;
    }
  }

  servo_extract_features(samples, WINDOW_SIZE, features);
  const int classIndex = servo_rf_predict(features);
  const char *label = SERVO_RF_LABELS[classIndex];
  const bool nonNormal = strcmp(label, "NORMAL") != 0;
  if (nonNormal) { ++nonNormalWindows; normalWindows = 0; }
  else { ++normalWindows; nonNormalWindows = 0; }
  if (nonNormalWindows >= CONFIRM_WINDOWS) setAlarm(true);
  if (normalWindows >= CONFIRM_WINDOWS) setAlarm(false);

  Serial.printf("PREDICTION,%s,servo_deg=%d,alarm=%d\n", label, servoAngle,
                alarmActive ? 1 : 0);
}
