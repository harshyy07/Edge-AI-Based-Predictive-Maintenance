#include <Wire.h>

static const uint8_t MPU_ADDR = 0x68;
static const uint8_t SDA_PIN = 21;
static const uint8_t SCL_PIN = 22;
static const uint16_t SAMPLE_COUNT = 200;
static const uint32_t SAMPLE_PERIOD_US = 5000;
static const float ACCEL_LSB_PER_G = 16384.0f;
static const char* LABELS[] = {"NORMAL", "HIGH_FRICTION", "MOUNTING_LOOSENESS", "ABNORMAL_VIBRATION", "IRREGULAR_MOVEMENT", "STOP"};
static const size_t LABEL_COUNT = sizeof(LABELS) / sizeof(LABELS[0]);

char currentLabel[32] = "NORMAL";
uint32_t windowId = 0;

bool readRegister(uint8_t reg, uint8_t* data, size_t length) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)length) != length) return false;
  for (size_t i = 0; i < length; ++i) data[i] = Wire.read();
  return true;
}

bool writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool labelAllowed(const char* candidate) {
  for (size_t i = 0; i < LABEL_COUNT; ++i) {
    if (strcmp(candidate, LABELS[i]) == 0) return true;
  }
  return false;
}

void processSerial() {
  static char line[48];
  static size_t used = 0;
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      line[used] = '\0';
      if (strncmp(line, "LABEL,", 6) == 0 && labelAllowed(line + 6)) {
        strncpy(currentLabel, line + 6, sizeof(currentLabel) - 1);
        currentLabel[sizeof(currentLabel) - 1] = '\0';
        Serial.print("#LABEL,"); Serial.println(currentLabel);
      } else if (used > 0) {
        Serial.println("#ERROR,Use LABEL,<one of the six supported labels>");
      }
      used = 0;
    } else if (used < sizeof(line) - 1) {
      line[used++] = c;
    } else {
      used = 0;
      Serial.println("#ERROR,Command too long");
    }
  }
}

bool readAccel(float& ax, float& ay, float& az) {
  uint8_t bytes[6];
  if (!readRegister(0x3B, bytes, sizeof(bytes))) return false;
  int16_t x = (int16_t)((bytes[0] << 8) | bytes[1]);
  int16_t y = (int16_t)((bytes[2] << 8) | bytes[3]);
  int16_t z = (int16_t)((bytes[4] << 8) | bytes[5]);
  ax = x / ACCEL_LSB_PER_G;
  ay = y / ACCEL_LSB_PER_G;
  az = z / ACCEL_LSB_PER_G;
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);
  delay(100);
  uint8_t who = 0;
  if (!readRegister(0x75, &who, 1) || (who != 0x68 && who != 0x69)) {
    Serial.println("#ERROR,MPU6050 not found at 0x68");
    while (true) delay(1000);
  }
  bool ok = writeRegister(0x6B, 0x00) && writeRegister(0x1C, 0x00) &&
            writeRegister(0x1A, 0x01) && writeRegister(0x19, 0x04);
  if (!ok) {
    Serial.println("#ERROR,MPU6050 configuration failed");
    while (true) delay(1000);
  }
  delay(100);
  Serial.println("#READY,MPU6050,200Hz,200 samples/window");
  Serial.println("#COMMAND,LABEL,<NORMAL|HIGH_FRICTION|MOUNTING_LOOSENESS|ABNORMAL_VIBRATION|IRREGULAR_MOVEMENT|STOP>");
  Serial.println("window_id,sample_index,timestamp_us,label,ax_g,ay_g,az_g");
}

void loop() {
  processSerial();
  ++windowId;
  uint32_t nextSample = micros();
  for (uint16_t i = 0; i < SAMPLE_COUNT; ++i) {
    processSerial();
    while ((int32_t)(micros() - nextSample) < 0) processSerial();
    nextSample += SAMPLE_PERIOD_US;
    float ax, ay, az;
    if (!readAccel(ax, ay, az)) {
      Serial.println("#ERROR,Sensor read failed; window aborted");
      break;
    }
    Serial.printf("%lu,%u,%lu,%s,%.6f,%.6f,%.6f\n",
      (unsigned long)windowId, (unsigned)i, (unsigned long)micros(), currentLabel, ax, ay, az);
  }
}
