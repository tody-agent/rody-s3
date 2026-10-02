#include <unity.h>
#include "calib.h"
using namespace calib;

void setUp() {} void tearDown() {}

static Wheel mkWheel(int8_t sign) {
  Wheel w; w.db_lo = 1480; w.db_hi = 1520; w.fwd_sign = sign;
  for (size_t i = 0; i < NPTS; i++) { w.fwd.rpm[i] = OFFSETS[i] * 0.4f; w.rev.rpm[i] = OFFSETS[i] * 0.5f; }
  return w;
}

void test_deadband() {
  uint16_t p[10]; bool m[10];
  for (int i = 0; i < 10; i++) { p[i] = 1460 + i * 10; m[i] = (i < 3 || i > 6); }
  uint16_t lo, hi;
  TEST_ASSERT_TRUE(findDeadband(p, m, 10, 1500, lo, hi));
  TEST_ASSERT_EQUAL_UINT16(1490, lo); TEST_ASSERT_EQUAL_UINT16(1520, hi);
}
void test_rpm()      { TEST_ASSERT_FLOAT_WITHIN(0.01, 60.0, rpmFromEdges(8, 1000, 8)); }
void test_monotonic(){ DirCurve c{}; c.rpm[1]=10; c.rpm[2]=8; makeMonotonic(c); TEST_ASSERT_EQUAL_FLOAT(10, c.rpm[2]); }
void test_inverse()  { Wheel w = mkWheel(1); TEST_ASSERT_FLOAT_WITHIN(0.01, 50.0, offsetForRpm(w.fwd, 20.0f));
                       TEST_ASSERT_EQUAL_FLOAT(-1, offsetForRpm(w.fwd, 1000)); }
void test_direction(){ Wheel a = mkWheel(1), b = mkWheel(-1);
                       TEST_ASSERT_TRUE(pulseFor(a, 50, 80) > 1520); TEST_ASSERT_TRUE(pulseFor(b, 50, 80) < 1480);
                       TEST_ASSERT_TRUE(pulseFor(a, -50, 80) < 1480); TEST_ASSERT_EQUAL_UINT16(0, pulseFor(a, 0, 80)); }
void test_balance()  { Wheel l = mkWheel(1), r = mkWheel(-1);
                       float v = commonMaxRpm(l, r);           // min = 100 rpm * 0.9
                       TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, v); }
void test_drift()    { Wheel l = mkWheel(1), r = mkWheel(-1);
                       float k = driftRatio(5, 100, 10);       // lệch trái → giảm bánh phải
                       TEST_ASSERT_FLOAT_WITHIN(1e-4, 0.99, k); applyDrift(l, r, k);
                       TEST_ASSERT_FLOAT_WITHIN(1e-4, 0.99, r.trim); TEST_ASSERT_EQUAL_FLOAT(1.0, l.trim); }

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_deadband); RUN_TEST(test_rpm); RUN_TEST(test_monotonic);
  RUN_TEST(test_inverse); RUN_TEST(test_direction); RUN_TEST(test_balance); RUN_TEST(test_drift);
  return UNITY_END();
}
