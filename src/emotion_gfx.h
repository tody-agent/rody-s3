#pragma once
#include <stdint.h>
#include "store.h"

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
void switchDisplay(store::DisplayType dt);
void setEmotion(Emotion e);
Emotion getEmotion();
const char* getEmotionStr();
bool setEmotionByName(const char* name);
void update(); // Call periodically in loop or FreeRTOS task

int getWidth();
int getHeight();
const char* getActiveDisplayModel();
bool isReady();
void testDisplayPattern();

}

