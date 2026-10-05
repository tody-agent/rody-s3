#include "behaviors.h"
#include "../include/pins.h"
#include "sensors.h"
#include "imu_sensor.h"
#include "drive.h"
#include "emotion_gfx.h"
#include "audio_player.h"
#include <Arduino.h>
#include <string.h>

namespace behaviors {

static Mode currentMode = Mode::MANUAL;
static uint32_t lastBehaviorUpdate = 0;
static bool wasEmergency = false;

void init() {
  currentMode = Mode::MANUAL;
  wasEmergency = false;
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

  // IMU Motion Intelligence & Tilt Protection
  if (sensors::isEmergencyStop()) {
    drive::stop();
    wasEmergency = true;
    if (sensors::isBellyUp()) {
      emotion_gfx::setEmotion(emotion_gfx::Emotion::DIZZY);
    } else {
      emotion_gfx::setEmotion(emotion_gfx::Emotion::OBSTACLE);
    }
    return;
  } else if (wasEmergency) {
    wasEmergency = false;
    audio_player::playSfx(audio_player::SoundEffect::HAPPY_CHIRP);
    emotion_gfx::setEmotion(emotion_gfx::Emotion::HAPPY);
  }

  // Interactive gestures: Shaking / Knocking
  if (imu_sensor::isAvailable()) {
    const auto& imuSt = imu_sensor::getState();
    if (imuSt.isShaking) {
      drive::stop();
      emotion_gfx::setEmotion(emotion_gfx::Emotion::DIZZY);
      audio_player::playSfx(audio_player::SoundEffect::OBSTACLE_ALARM);
      return;
    }
    if (imuSt.isKnocked) {
      emotion_gfx::setEmotion(emotion_gfx::Emotion::OBSTACLE);
      imu_sensor::clearTransientFlags();
    }
  }

  if (currentMode == Mode::AVOID) {
    float dist = sensors::measureDistanceCmOnce();
    const char* err = nullptr;
    if (dist <= OBSTACLE_CM && dist > 1.0f) {
      // Obstacle detected within 20cm!
      drive::stop();
      emotion_gfx::setEmotion(emotion_gfx::Emotion::OBSTACLE);
      audio_player::playSfx(audio_player::SoundEffect::OBSTACLE_ALARM);
      // Back up slightly or rotate
      drive::drive(-40.0f, 40.0f, 350, err); // pivot
    } else {
      // Clear ahead
      emotion_gfx::setEmotion(emotion_gfx::Emotion::DRIVE_FWD);
      drive::drive(45.0f, 45.0f, 200, err);
    }
  }
  else if (currentMode == Mode::LINE) {
    int l = 0, r = 0;
    sensors::readLine(l, r);
    const char* err = nullptr;
    if (l == 0 && r == 0) {
      // On track
      emotion_gfx::setEmotion(emotion_gfx::Emotion::DRIVE_FWD);
      drive::drive(40.0f, 40.0f, 150, err);
    } else if (l == 1 && r == 0) {
      // Left sensor off track / on line -> turn left
      emotion_gfx::setEmotion(emotion_gfx::Emotion::TURN_LEFT);
      drive::drive(15.0f, 45.0f, 150, err);
    } else if (l == 0 && r == 1) {
      // Right sensor on line -> turn right
      emotion_gfx::setEmotion(emotion_gfx::Emotion::TURN_RIGHT);
      drive::drive(45.0f, 15.0f, 150, err);
    } else {
      // Both detected (cross line) -> slow straight & happy
      emotion_gfx::setEmotion(emotion_gfx::Emotion::HAPPY);
      drive::drive(30.0f, 30.0f, 150, err);
    }
  }
}

}
