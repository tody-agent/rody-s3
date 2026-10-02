#include "behaviors.h"
#include "../include/pins.h"
#include "sensors.h"
#include "drive.h"
#include <Arduino.h>
#include <string.h>

namespace behaviors {

static Mode currentMode = Mode::MANUAL;
static uint32_t lastBehaviorUpdate = 0;

void init() {
  currentMode = Mode::MANUAL;
}

bool setMode(const char* modeStr) {
  if (strcmp(modeStr, "manual") == 0) {
    currentMode = Mode::MANUAL;
    drive::stop();
    return true;
  } else if (strcmp(modeStr, "avoid") == 0) {
    currentMode = Mode::AVOID;
    drive::stop();
    return true;
  } else if (strcmp(modeStr, "line") == 0) {
    currentMode = Mode::LINE;
    drive::stop();
    return true;
  }
  return false;
}

const char* getModeStr() {
  switch (currentMode) {
    case Mode::MANUAL: return "manual";
    case Mode::AVOID:  return "avoid";
    case Mode::LINE:   return "line";
    default:           return "unknown";
  }
}

Mode getMode() {
  return currentMode;
}

void update() {
  uint32_t now = millis();
  if (now - lastBehaviorUpdate < 80) return; // rate limit behavior loop to ~12Hz
  lastBehaviorUpdate = now;

  if (currentMode == Mode::AVOID) {
    float dist = sensors::measureDistanceCmOnce();
    const char* err = nullptr;
    if (dist <= OBSTACLE_CM && dist > 1.0f) {
      // Obstacle detected within 20cm!
      drive::stop();
      // Back up slightly or rotate
      drive::drive(-40.0f, 40.0f, 350, err); // pivot
    } else {
      // Clear ahead
      drive::drive(45.0f, 45.0f, 200, err);
    }
  }
  else if (currentMode == Mode::LINE) {
    int l = 0, r = 0;
    sensors::readLine(l, r);
    const char* err = nullptr;
    if (l == 0 && r == 0) {
      // On track
      drive::drive(40.0f, 40.0f, 150, err);
    } else if (l == 1 && r == 0) {
      // Left sensor off track / on line -> turn left
      drive::drive(15.0f, 45.0f, 150, err);
    } else if (l == 0 && r == 1) {
      // Right sensor on line -> turn right
      drive::drive(45.0f, 15.0f, 150, err);
    } else {
      // Both detected (cross line) -> slow straight
      drive::drive(30.0f, 30.0f, 150, err);
    }
  }
}

}
