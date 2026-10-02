#pragma once
#include <stdint.h>
#include <stddef.h>

namespace calib {
constexpr size_t NPTS = 12;
constexpr int16_t OFFSETS[NPTS] = {0,10,20,30,40,60,80,100,130,160,200,250};

struct DirCurve { float rpm[NPTS]; };     // rpm at OFFSETS[i]
struct Wheel {
  uint16_t db_lo = 1500, db_hi = 1500;    // deadband [lo, hi]
  int8_t   fwd_sign = 0;                  // +1: pulse up => robot forward
  DirCurve fwd{}, rev{};                  // in ROBOT direction terms
  float    trim = 1.0f;
};

bool     findDeadband(const uint16_t* pulse, const bool* moving, size_t n,
                      uint16_t hint, uint16_t& lo, uint16_t& hi);
float    rpmFromEdges(uint32_t edges, uint32_t ms, uint8_t edgesPerRev);
void     makeMonotonic(DirCurve& c);
float    offsetForRpm(const DirCurve& c, float rpm);   // -1 if > max
float    commonMaxRpm(const Wheel& l, const Wheel& r, float margin = 0.9f);
uint16_t pulseAt(const Wheel& w, bool robotFwd, float offsetUs);
uint16_t pulseFor(const Wheel& w, float speedPct, float vmax); // 0 => outputs off
float    driftRatio(float drift_cm, float dist_cm, float wheelbase_cm); // d>0 = left
void     applyDrift(Wheel& l, Wheel& r, float ratio);
}
