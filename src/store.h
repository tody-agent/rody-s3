#pragma once
#include <stdint.h>
#include "calib.h"

struct CalBlob {
  uint16_t ver;
  calib::Wheel l;
  calib::Wheel r;
  float wheelbase_cm;
  uint32_t crc;
};

namespace store {
constexpr uint16_t CAL_VERSION = 1;
extern CalBlob gCal;
extern bool gCalValid;

void init();
bool load();
bool save();
void reset();
uint32_t computeCrc(const CalBlob& blob);
}
