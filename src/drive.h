#pragma once
#include <stdint.h>

namespace drive {
bool init();
void stop();
bool setPwmRaw(int ch, uint16_t us, uint32_t durationMs = 10000);
bool drive(float speedL, float speedR, uint32_t durationMs, const char*& err);
void update();
bool isMoving();
}
