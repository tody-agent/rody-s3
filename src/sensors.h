#pragma once
#include <stdint.h>
#include <stddef.h>

namespace sensors {
void init();
float readBatteryVoltage();
float measureDistanceCmOnce();
bool readUltrasonicN(size_t n, float* results, size_t maxN);
void readLine(int& l, int& r);
void resetTach();
void getTachCounts(uint32_t& l, uint32_t& r);
void sampleTach(uint32_t ms, uint32_t& edgesL, uint32_t& edgesR, float& rpmL, float& rpmR);
}
