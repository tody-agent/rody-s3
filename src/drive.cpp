#include "drive.h"
#include "../include/pins.h"
#include "store.h"
#include "sensors.h"
#include "calib.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

namespace drive {

static Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(PCA_ADDR);
static uint32_t stopDeadlineMs = 0;
static bool activeMotion = false;

bool init() {
  Wire.begin(pins::I2C_SDA, pins::I2C_SCL, 400000);
  Wire.setTimeOut(20);
  Wire.setClock(400000);
  if (!pca.begin()) {
    Serial.println("# [drive] PCA9685 connection failed at 0x40");
    return false;
  }
  pca.setPWMFreq(50); // 50 Hz for standard analog/digital servos
  stop();
  Serial.println("# [drive] PCA9685 initialized at 50Hz, channels full-off, I2C 400kHz");
  return true;
}

void stop() {
  pca.setPin(CH_L, 0, false);
  pca.setPin(CH_R, 0, false);
  activeMotion = false;
  stopDeadlineMs = 0;
}

bool isMoving() {
  return activeMotion;
}

bool setPwmRaw(int ch, uint16_t us, uint32_t durationMs) {
  if (ch != CH_L && ch != CH_R) return false;
  if (us == 0) {
    pca.setPin(ch, 0, false);
  } else {
    if (sensors::isBatteryLow() || sensors::isEmergencyStop()) {
      stop();
      return false;
    }
    pca.writeMicroseconds(ch, us);
    activeMotion = true;
    stopDeadlineMs = millis() + (durationMs > 0 ? durationMs : 10000);
  }
  return true;
}

bool drive(float speedL, float speedR, uint32_t durationMs, const char*& err) {
  if (sensors::isBatteryLow()) {
    stop();
    err = "low_battery";
    return false;
  }
  if (sensors::isEmergencyStop()) {
    stop();
    err = "emergency_stop_tilt";
    return false;
  }
  if (!store::gCalValid) {
    err = "uncalibrated";
    return false;
  }
  if (durationMs > 5000) {
    durationMs = 5000;
  }

  float vmax = calib::commonMaxRpm(store::gCal.l, store::gCal.r);
  uint16_t pL = calib::pulseFor(store::gCal.l, speedL, vmax);
  uint16_t pR = calib::pulseFor(store::gCal.r, speedR, vmax);

  if (pL == 0) {
    pca.setPin(CH_L, 0, false);
  } else {
    pca.writeMicroseconds(CH_L, pL);
  }

  if (pR == 0) {
    pca.setPin(CH_R, 0, false);
  } else {
    pca.writeMicroseconds(CH_R, pR);
  }

  activeMotion = (pL != 0 || pR != 0);
  stopDeadlineMs = millis() + durationMs;
  err = nullptr;
  return true;
}

void update() {
  if (activeMotion) {
    if (sensors::isBatteryLow()) {
      Serial.println("# [drive] Low battery detected during motion! Emergency stop.");
      stop();
      return;
    }
    if (sensors::isEmergencyStop()) {
      Serial.println("# [drive] Emergency tilt/fall detected! Cutting motor power.");
      stop();
      return;
    }
    if (millis() >= stopDeadlineMs) {
      stop();
    }
  }
}

}
