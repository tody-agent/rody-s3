#include "imu_sensor.h"
#include "../include/pins.h"
#include <Arduino.h>
#include <Wire.h>
#include <math.h>

namespace imu_sensor {

static uint8_t activeAddr = MPU6050_ADDR; // 0x68 default
static ChipType detectedChip = ChipType::UNKNOWN;
static bool magnetometerFound = false;

// MPU6050 / MPU6500 / MPU9250 Registers
static constexpr uint8_t REG_SMPLRT_DIV     = 0x19;
static constexpr uint8_t REG_CONFIG         = 0x1A;
static constexpr uint8_t REG_GYRO_CONFIG    = 0x1B;
static constexpr uint8_t REG_ACCEL_CONFIG   = 0x1C;
static constexpr uint8_t REG_ACCEL_CONFIG_2 = 0x1D; // MPU6500 / MPU9250 DLPF
static constexpr uint8_t REG_INT_PIN_CFG    = 0x37; // Bit 1 = BYPASS_EN for AK8963
static constexpr uint8_t REG_ACCEL_XOUT_H   = 0x3B;
static constexpr uint8_t REG_USER_CTRL      = 0x6A; // Bit 5 = I2C_MST_EN
static constexpr uint8_t REG_PWR_MGMT_1     = 0x6B;
static constexpr uint8_t REG_WHO_AM_I       = 0x75;

// AK8963 Magnetometer Registers (on MPU9250 at 0x0C)
static constexpr uint8_t AK8963_REG_WIA     = 0x00; // WHO_I_AM, expected 0x48

// Scale factors for +/- 8g and +/- 1000 deg/s
static constexpr float ACCEL_SCALE = 4096.0f;  // LSB per g
static constexpr float GYRO_SCALE  = 32.8f;    // LSB per deg/s

static MotionState currentState = {};
static bool isInitialized = false;

// History tracking for shake, knock, freefall
static uint32_t lastUpdateTime = 0;
static uint32_t freefallStartTime = 0;
static uint32_t lastShakeReversalTime = 0;
static int shakeReversalCount = 0;
static float lastAx = 0.0f;
static float lastAz = 1.0f;
static uint32_t airplaneStartTime = 0;

static bool writeRegister(uint8_t addr, uint8_t reg, uint8_t data) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(data);
  return (Wire.endTransmission() == 0);
}

static bool readRegisters(uint8_t addr, uint8_t reg, uint8_t* buffer, size_t len) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  
  size_t readCount = Wire.requestFrom((int)addr, (int)len);
  if (readCount != len) return false;

  for (size_t i = 0; i < len; i++) {
    buffer[i] = Wire.read();
  }
  return true;
}

static inline bool writeReg(uint8_t reg, uint8_t data) {
  return writeRegister(activeAddr, reg, data);
}

static inline bool readRegs(uint8_t reg, uint8_t* buffer, size_t len) {
  return readRegisters(activeAddr, reg, buffer, len);
}

bool init() {
  Wire.begin(pins::I2C_SDA, pins::I2C_SCL, 400000);
  Wire.setTimeOut(20);
  delay(10);

  // Auto-probe candidate I2C addresses: 0x68 (AD0=GND) and 0x69 (AD0=VCC/pull-up)
  const uint8_t candidateAddrs[] = { MPU6050_ADDR, MPU_ADDR_ALT };
  uint8_t whoAmI = 0;
  bool found = false;

  for (uint8_t addr : candidateAddrs) {
    if (readRegisters(addr, REG_WHO_AM_I, &whoAmI, 1)) {
      if (whoAmI == 0x68 || whoAmI == 0x70 || whoAmI == 0x71 || whoAmI == 0x72 || whoAmI == 0x73) {
        activeAddr = addr;
        found = true;
        break;
      }
    }
  }

  if (!found) {
    Serial.printf("# [imu] No IMU (MPU6050/6500/9250) detected at 0x68 or 0x69 (last read: 0x%02X)\n", whoAmI);
    isInitialized = false;
    detectedChip = ChipType::UNKNOWN;
    return false;
  }

  // Identify chip model from WHO_AM_I register
  switch (whoAmI) {
    case 0x68: detectedChip = ChipType::MPU6050; break;
    case 0x70: detectedChip = ChipType::MPU6500; break;
    case 0x71: detectedChip = ChipType::MPU9250; break;
    case 0x72:
    case 0x73: detectedChip = ChipType::MPU9255; break;
    default:   detectedChip = ChipType::UNKNOWN; break;
  }

  // Wake up IMU (clear sleep bit in PWR_MGMT_1, clock source = auto/PLL with Gyro)
  writeReg(REG_PWR_MGMT_1, 0x01);
  delay(10);

  // Sample rate divider = 4 -> 200Hz
  writeReg(REG_SMPLRT_DIV, 0x04);
  // DLPF (Low Pass Filter) config = 3 (~44Hz bandwidth for Gyro & Temp)
  writeReg(REG_CONFIG, 0x03);
  // Gyro +/- 1000 deg/s
  writeReg(REG_GYRO_CONFIG, 0x10);
  // Accel +/- 8g
  writeReg(REG_ACCEL_CONFIG, 0x10);

  // For MPU-6500 / MPU-9250 / MPU-9255: Configure accelerometer DLPF (REG_ACCEL_CONFIG_2)
  if (detectedChip != ChipType::MPU6050) {
    writeReg(REG_ACCEL_CONFIG_2, 0x03); // ~44Hz DLPF
  }

  // If MPU-9250 / MPU-9255: Enable I2C Bypass mode to expose AK8963 Magnetometer at 0x0C
  magnetometerFound = false;
  if (detectedChip == ChipType::MPU9250 || detectedChip == ChipType::MPU9255) {
    writeReg(REG_USER_CTRL, 0x00);
    delay(5);
    writeReg(REG_INT_PIN_CFG, 0x02);
    delay(10);

    uint8_t magWhoAmI = 0;
    if (readRegisters(AK8963_MAG_ADDR, AK8963_REG_WIA, &magWhoAmI, 1) && (magWhoAmI == 0x48)) {
      magnetometerFound = true;
      Serial.printf("# [imu] AK8963 Magnetometer active at 0x%02X (WIA: 0x%02X)\n", AK8963_MAG_ADDR, magWhoAmI);
    }
  }

  isInitialized = true;
  lastUpdateTime = millis();
  Serial.printf("# [imu] %s initialized successfully at 0x%02X (WHO_AM_I: 0x%02X%s)\n",
                getChipName(), activeAddr, whoAmI,
                magnetometerFound ? ", 9-DOF with AK8963" : "");
  return true;
}

const char* getChipName() {
  switch (detectedChip) {
    case ChipType::MPU6050: return "MPU-6050";
    case ChipType::MPU6500: return "MPU-6500 (GY-6500)";
    case ChipType::MPU9250: return "MPU-9250 (GY-9250)";
    case ChipType::MPU9255: return "MPU-9255";
    default: return isInitialized ? "MPU-Compatible" : "None";
  }
}

ChipType getChipType() {
  return detectedChip;
}

uint8_t getActiveAddress() {
  return activeAddr;
}

bool hasMagnetometer() {
  return magnetometerFound;
}

bool isAvailable() {
  return isInitialized;
}

void clearTransientFlags() {
  currentState.isImpact = false;
  currentState.isKnocked = false;
}

void update() {
  if (!isInitialized) return;

  uint32_t now = millis();
  float dt = (now - lastUpdateTime) / 1000.0f;
  if (dt < 0.015f) return; // run max ~66Hz to save CPU
  lastUpdateTime = now;

  // Read 14 burst bytes: Accel (6) + Temp (2) + Gyro (6)
  uint8_t rawBuf[14];
  if (!readRegs(REG_ACCEL_XOUT_H, rawBuf, 14)) {
    return;
  }

  int16_t rawAx = (int16_t)((rawBuf[0] << 8) | rawBuf[1]);
  int16_t rawAy = (int16_t)((rawBuf[2] << 8) | rawBuf[3]);
  int16_t rawAz = (int16_t)((rawBuf[4] << 8) | rawBuf[5]);

  int16_t rawGx = (int16_t)((rawBuf[8]  << 8) | rawBuf[9]);
  int16_t rawGy = (int16_t)((rawBuf[10] << 8) | rawBuf[11]);
  int16_t rawGz = (int16_t)((rawBuf[12] << 8) | rawBuf[13]);

  // Convert to physical units
  float ax = (float)rawAx / ACCEL_SCALE;
  float ay = (float)rawAy / ACCEL_SCALE;
  float az = (float)rawAz / ACCEL_SCALE;

  float gx = (float)rawGx / GYRO_SCALE;
  float gy = (float)rawGy / GYRO_SCALE;
  float gz = (float)rawGz / GYRO_SCALE;

  // Low-pass filter for smooth tilt angles
  currentState.ax = currentState.ax * 0.7f + ax * 0.3f;
  currentState.ay = currentState.ay * 0.7f + ay * 0.3f;
  currentState.az = currentState.az * 0.7f + az * 0.3f;
  currentState.gx = gx;
  currentState.gy = gy;
  currentState.gz = gz;

  // Total acceleration magnitude
  float totalAccel = sqrtf(ax * ax + ay * ay + az * az);
  currentState.totalAccel = totalAccel;

  // Pitch & Roll (degrees)
  currentState.pitch = atan2f(currentState.ax, sqrtf(currentState.ay * currentState.ay + currentState.az * currentState.az)) * 57.29578f;
  currentState.roll  = atan2f(currentState.ay, sqrtf(currentState.ax * currentState.ax + currentState.az * currentState.az)) * 57.29578f;

  // 1. FREEFALL DETECTION (|a| < 0.25g)
  if (totalAccel < 0.25f) {
    if (freefallStartTime == 0) freefallStartTime = now;
    if (now - freefallStartTime >= 50) {
      currentState.isFreefall = true;
    }
  } else {
    // 2. IMPACT DETECTION (Freefall followed by spike, or sudden heavy hit > 2.8g)
    if (currentState.isFreefall && totalAccel > 2.2f) {
      currentState.isImpact = true;
      currentState.isFreefall = false;
      freefallStartTime = 0;
    } else if (totalAccel > 2.8f) {
      currentState.isImpact = true;
    }
    if (totalAccel >= 0.6f) {
      currentState.isFreefall = false;
      freefallStartTime = 0;
    }
  }

  // 3. BELLY UP (Z axis pointing downwards / robot flipped upside down)
  currentState.isBellyUp = (currentState.az < -0.45f);

  // 4. FALLEN OVER (Tilted more than 65 degrees while not belly up)
  float absPitch = fabsf(currentState.pitch);
  float absRoll  = fabsf(currentState.roll);
  currentState.isFallen = (!currentState.isBellyUp && (absPitch > 65.0f || absRoll > 65.0f));

  // 5. SHAKE DETECTION (Oscillating rapid sign changes in acceleration or gyro)
  float deltaAx = ax - lastAx;
  if (fabsf(deltaAx) > 1.2f || fabsf(gz) > 280.0f) {
    if (now - lastShakeReversalTime < 450) {
      shakeReversalCount++;
      if (shakeReversalCount >= 3) {
        currentState.isShaking = true;
      }
    } else {
      shakeReversalCount = 1;
    }
    lastShakeReversalTime = now;
  } else if (now - lastShakeReversalTime > 800) {
    currentState.isShaking = false;
    shakeReversalCount = 0;
  }
  lastAx = ax;

  // 6. KNOCK DETECTION (Sharp impulse shock on Z axis returning rapidly)
  float deltaAz = fabsf(az - lastAz);
  if (deltaAz > 1.5f && totalAccel > 2.1f && !currentState.isShaking && !currentState.isFreefall) {
    currentState.isKnocked = true;
  }
  lastAz = az;

  // 7. AIRPLANE FLIGHT MODE (Lifted and banking/swooping through the air)
  float gyroSum = fabsf(gx) + fabsf(gy) + fabsf(gz);
  if (!currentState.isBellyUp && !currentState.isFallen && !currentState.isShaking &&
      totalAccel >= 0.75f && totalAccel <= 1.35f &&
      gyroSum >= 20.0f && gyroSum <= 220.0f &&
      (absPitch >= 10.0f || absRoll >= 10.0f)) {
    if (airplaneStartTime == 0) {
      airplaneStartTime = now;
    } else if (now - airplaneStartTime >= 800) {
      currentState.isAirplaneMode = true;
    }
  } else if (now - airplaneStartTime > 500 && (gyroSum < 10.0f || absPitch < 8.0f)) {
    airplaneStartTime = 0;
    currentState.isAirplaneMode = false;
  }
}

const MotionState& getState() {
  return currentState;
}

} // namespace imu_sensor
