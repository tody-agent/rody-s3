#include "calib.h"
#include <math.h>

namespace calib {

bool findDeadband(const uint16_t* p, const bool* mv, size_t n,
                  uint16_t hint, uint16_t& lo, uint16_t& hi) {
  size_t bestS = 0, bestL = 0, s = 0, L = 0;
  for (size_t i = 0; i < n; i++) {
    if (!mv[i]) { if (L == 0) s = i; if (++L > bestL) { bestL = L; bestS = s; } }
    else L = 0;
  }
  if (bestL < 2) return false;
  lo = p[bestS]; hi = p[bestS + bestL - 1];
  return lo <= hint + 150 && hi + 150 >= hint;
}

float rpmFromEdges(uint32_t e, uint32_t ms, uint8_t epr) {
  if (ms == 0 || epr == 0) return 0;
  return (float)e / epr * 60000.0f / ms;
}

void makeMonotonic(DirCurve& c) {
  for (size_t i = 1; i < NPTS; i++) if (c.rpm[i] < c.rpm[i-1]) c.rpm[i] = c.rpm[i-1];
}

float offsetForRpm(const DirCurve& c, float rpm) {
  if (rpm <= 0) return 0;
  for (size_t i = 1; i < NPTS; i++) {
    if (c.rpm[i] >= rpm) {
      float r0 = c.rpm[i-1], r1 = c.rpm[i];
      if (r1 <= r0) return OFFSETS[i];
      float t = (rpm - r0) / (r1 - r0);
      return OFFSETS[i-1] + t * (OFFSETS[i] - OFFSETS[i-1]);
    }
  }
  return -1;
}

float commonMaxRpm(const Wheel& l, const Wheel& r, float m) {
  float v = fminf(fminf(l.fwd.rpm[NPTS-1], l.rev.rpm[NPTS-1]),
                  fminf(r.fwd.rpm[NPTS-1], r.rev.rpm[NPTS-1]));
  return v * m;
}

uint16_t pulseAt(const Wheel& w, bool robotFwd, float off) {
  bool up = (robotFwd == (w.fwd_sign > 0));
  float p = up ? w.db_hi + 1 + off : w.db_lo - 1 - off;
  return (uint16_t)lroundf(p);
}

uint16_t pulseFor(const Wheel& w, float s, float vmax) {
  if (fabsf(s) < 1.0f || w.fwd_sign == 0) return 0;
  if (s > 100) s = 100; if (s < -100) s = -100;
  bool fwd = s > 0;
  float target = fabsf(s) / 100.0f * vmax * w.trim;
  float off = offsetForRpm(fwd ? w.fwd : w.rev, target);
  if (off < 0) off = OFFSETS[NPTS-1];
  return pulseAt(w, fwd, off);
}

float driftRatio(float d, float D, float W) {
  if (D <= 0) return 1.0f;
  float r = 1.0f - 2.0f * d * W / (D * D);
  return r < 0.8f ? 0.8f : (r > 1.2f ? 1.2f : r);
}

void applyDrift(Wheel& l, Wheel& r, float ratio) {
  if (ratio < 1.0f) r.trim *= ratio; else l.trim *= 1.0f / ratio;
}
}
