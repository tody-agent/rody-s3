#pragma once
#include <stdint.h>

namespace emotion_gfx {

enum class Emotion {
  IDLE,
  HAPPY,
  LISTENING,
  THINKING,
  SPEAKING,
  DRIVE_FWD,
  DRIVE_REV,
  TURN_LEFT,
  TURN_RIGHT,
  DIZZY,
  OBSTACLE,
  SLEEPY
};

void init();
void setEmotion(Emotion e);
Emotion getEmotion();
const char* getEmotionStr();
bool setEmotionByName(const char* name);
void update(); // Call periodically in loop or FreeRTOS task

}
