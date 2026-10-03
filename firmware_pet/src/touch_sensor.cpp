#include "touch_sensor.h"
#include "../include/pet_pins.h"
#include <Arduino.h>

namespace touch_sensor {

static TouchMode currentMode = TouchMode::DIGITAL_TTP223;
static bool touchState = false;
static uint32_t touchStartTime = 0;
static uint32_t touchReleaseTime = 0;
static bool tapPending = false;
static uint32_t tapHistory[4] = {0, 0, 0, 0};
static uint8_t tapIndex = 0;
static bool ticklePending = false;
static int baselineCap = 50000;

void init(TouchMode mode) {
  currentMode = mode;
  if (currentMode == TouchMode::DIGITAL_TTP223) {
    pinMode(pet_pins::TOUCH_PIN, INPUT_PULLDOWN);
    Serial.println("# [touch] Initialized in DIGITAL_TTP223 mode on GPIO 2");
  } else {
#if defined(CONFIG_IDF_TARGET_ESP32S3) || defined(ARDUINO_ARCH_ESP32)
    // Calibration baseline for internal capacitive touch
    long sum = 0;
    for (int i = 0; i < 16; i++) {
      sum += touchRead(pet_pins::TOUCH_PIN);
      delay(5);
    }
    baselineCap = sum / 16;
    Serial.printf("# [touch] Initialized in CAPACITIVE_PAD mode (Baseline: %d)\n", baselineCap);
#endif
  }
  touchState = false;
  touchStartTime = 0;
}

void update() {
  bool activeNow = false;

  if (currentMode == TouchMode::DIGITAL_TTP223) {
    activeNow = (digitalRead(pet_pins::TOUCH_PIN) == HIGH);
  } else {
#if defined(CONFIG_IDF_TARGET_ESP32S3) || defined(ARDUINO_ARCH_ESP32)
    int val = touchRead(pet_pins::TOUCH_PIN);
    // On ESP32-S3, touchRead values typically decrease or increase significantly on contact
    activeNow = (val > baselineCap + 4000 || val < baselineCap - 4000);
#endif
  }

  uint32_t now = millis();

  if (activeNow && !touchState) {
    // Touch begins
    touchState = true;
    touchStartTime = now;
  } else if (!activeNow && touchState) {
    // Touch ends
    touchState = false;
    touchReleaseTime = now;
    uint32_t duration = touchReleaseTime - touchStartTime;
    if (duration >= 50 && duration <= 280) {
      tapPending = true;
      tapHistory[tapIndex % 4] = now;
      tapIndex++;
      // Kiểm tra xem 3 cú chạm gần nhất có diễn ra trong vòng 600ms không
      if (tapIndex >= 3) {
        uint32_t oldestTap = tapHistory[(tapIndex + 1) % 4];
        if (oldestTap > 0 && (now - oldestTap <= 600)) {
          ticklePending = true;
        }
      }
    }
    touchStartTime = 0;
  }
}

bool isTouched() {
  return touchState;
}

bool isPetting() {
  if (!touchState) return false;
  return (millis() - touchStartTime >= 300);
}

uint32_t getPetDurationMs() {
  if (!touchState || touchStartTime == 0) return 0;
  return (millis() - touchStartTime);
}

bool checkAndClearTap() {
  if (tapPending) {
    tapPending = false;
    return true;
  }
  return false;
}

bool checkAndClearTickle() {
  if (ticklePending) {
    ticklePending = false;
    tapIndex = 0;
    return true;
  }
  return false;
}

} // namespace touch_sensor
