#include "calib_run.h"
#include "../include/pins.h"
#include "store.h"
#include "drive.h"
#include "sensors.h"
#include "calib.h"
#include <Arduino.h>
#include <math.h>

namespace calib_run {

static bool getWheelContext(char wheel, int& ch, int& idx, calib::Wheel*& w) {
  if (wheel == 'L' || wheel == 'l') {
    ch = CH_L;
    idx = 0;
    w = &store::gCal.l;
    return true;
  } else if (wheel == 'R' || wheel == 'r') {
    ch = CH_R;
    idx = 1;
    w = &store::gCal.r;
    return true;
  }
  return false;
}

bool runDeadband(char wheel, uint16_t& lo, uint16_t& hi) {
  int ch, idx;
  calib::Wheel* w;
  if (!getWheelContext(wheel, ch, idx, w)) return false;

  constexpr size_t MAX_PTS = 80;
  uint16_t pulses[MAX_PTS];
  bool moving[MAX_PTS];
  size_t n = 0;

  for (uint16_t p = 1350; p <= 1650 && n < MAX_PTS; p += 4) {
    drive::setPwmRaw(ch, p);
    delay(200); // settle
    sensors::resetTach();
    delay(400); // count edges
    uint32_t eL, eR;
    sensors::getTachCounts(eL, eR);
    uint32_t edges = (idx == 0) ? eL : eR;
    pulses[n] = p;
    moving[n] = (edges >= 1);
    n++;
  }
  drive::stop();

  if (calib::findDeadband(pulses, moving, n, 1500, lo, hi)) {
    w->db_lo = lo;
    w->db_hi = hi;
    return true;
  }
  return false;
}

bool runSpin(char wheel) {
  int ch, idx;
  calib::Wheel* w;
  if (!getWheelContext(wheel, ch, idx, w)) return false;

  uint16_t pulse = w->db_hi + 60;
  drive::setPwmRaw(ch, pulse);
  delay(1500);
  drive::stop();
  return true;
}

bool setDir(char wheel, int8_t dir) {
  int ch, idx;
  calib::Wheel* w;
  if (!getWheelContext(wheel, ch, idx, w)) return false;
  if (dir != 1 && dir != -1) return false;
  w->fwd_sign = dir;
  return true;
}

bool runSweep(char wheel, float* fwdRpm, float* revRpm, size_t nPts) {
  int ch, idx;
  calib::Wheel* w;
  if (!getWheelContext(wheel, ch, idx, w)) return false;
  if (w->fwd_sign == 0) return false;

  // Sweep Forward
  for (size_t i = 0; i < calib::NPTS; i++) {
    uint16_t p = calib::pulseAt(*w, true, calib::OFFSETS[i]);
    drive::setPwmRaw(ch, p);
    delay(200);
    sensors::resetTach();
    delay(1500);
    uint32_t eL, eR;
    sensors::getTachCounts(eL, eR);
    float rpm = calib::rpmFromEdges((idx == 0 ? eL : eR), 1500, TACH_EDGES_PER_REV);
    w->fwd.rpm[i] = rpm;
    if (fwdRpm && i < nPts) fwdRpm[i] = rpm;
  }
  calib::makeMonotonic(w->fwd);

  // Sweep Reverse
  for (size_t i = 0; i < calib::NPTS; i++) {
    uint16_t p = calib::pulseAt(*w, false, calib::OFFSETS[i]);
    drive::setPwmRaw(ch, p);
    delay(200);
    sensors::resetTach();
    delay(1500);
    uint32_t eL, eR;
    sensors::getTachCounts(eL, eR);
    float rpm = calib::rpmFromEdges((idx == 0 ? eL : eR), 1500, TACH_EDGES_PER_REV);
    w->rev.rpm[i] = rpm;
    if (revRpm && i < nPts) revRpm[i] = rpm;
  }
  calib::makeMonotonic(w->rev);
  drive::stop();
  return true;
}

bool runMeas(float speedPct, float& targetRpm, float& rpmL, float& rpmR, float& errPct) {
  if (!store::gCalValid) return false;
  const char* err = nullptr;
  if (!drive::drive(speedPct, speedPct, 2500, err)) return false;

  delay(500); // settle speed
  sensors::resetTach();
  delay(1000); // sample
  uint32_t eL, eR;
  sensors::getTachCounts(eL, eR);
  drive::stop();

  rpmL = calib::rpmFromEdges(eL, 1000, TACH_EDGES_PER_REV);
  rpmR = calib::rpmFromEdges(eR, 1000, TACH_EDGES_PER_REV);

  float maxR = fmaxf(rpmL, rpmR);
  errPct = (maxR > 0.01f) ? (fabsf(rpmL - rpmR) / maxR * 100.0f) : 0.0f;

  float vmax = calib::commonMaxRpm(store::gCal.l, store::gCal.r);
  targetRpm = fabsf(speedPct) / 100.0f * vmax;
  return true;
}

bool setWheelbase(float cm) {
  if (cm <= 0) return false;
  store::gCal.wheelbase_cm = cm;
  return true;
}

bool runDrift(float dCm, float dTotalCm, float& trimL, float& trimR) {
  if (dTotalCm <= 0) return false;
  float ratio = calib::driftRatio(dCm, dTotalCm, store::gCal.wheelbase_cm);
  calib::applyDrift(store::gCal.l, store::gCal.r, ratio);
  trimL = store::gCal.l.trim;
  trimR = store::gCal.r.trim;
  return true;
}

}
