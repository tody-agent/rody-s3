#pragma once
#include <stdint.h>
#include <stddef.h>

namespace calib_run {
bool runDeadband(char wheel, uint16_t& lo, uint16_t& hi);
bool runSpin(char wheel);
bool setDir(char wheel, int8_t dir);
bool runSweep(char wheel, float* fwdRpm, float* revRpm, size_t nPts);
bool runMeas(float speedPct, float& targetRpm, float& rpmL, float& rpmR, float& errPct);
bool setWheelbase(float cm);
bool runDrift(float dCm, float dTotalCm, float& trimL, float& trimR);
}
