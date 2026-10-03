#include <unity.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

// =========================================================================
// 1. Logic Test: IMU Freefall & Impact Calculation
// =========================================================================
struct TestMotionState {
  float ax, ay, az;
  float totalAccel;
  bool isFreefall;
  bool isImpact;
  bool isBellyUp;
  bool isFallen;
  bool isShaking;
  bool isKnocked;
};

static void evalImuState(float ax, float ay, float az, TestMotionState& state) {
  state.ax = ax;
  state.ay = ay;
  state.az = az;
  state.totalAccel = sqrtf(ax * ax + ay * ay + az * az);

  // Freefall: |a| < 0.25g
  state.isFreefall = (state.totalAccel < 0.25f);

  // Impact: sudden shock > 2.8g
  state.isImpact = (state.totalAccel > 2.8f);

  // Belly Up: Z axis negative
  state.isBellyUp = (az < -0.45f);

  // Fallen over: tilted pitch/roll > 65 deg
  float pitch = atan2f(ax, sqrtf(ay * ay + az * az)) * 57.29578f;
  float roll  = atan2f(ay, sqrtf(ax * ax + az * az)) * 57.29578f;
  state.isFallen = (!state.isBellyUp && (fabsf(pitch) > 65.0f || fabsf(roll) > 65.0f));
}

void test_imu_upright_neutral() {
  TestMotionState s = {};
  evalImuState(0.0f, 0.0f, 1.0f, s); // 1g pointing down
  TEST_ASSERT_FALSE(s.isFreefall);
  TEST_ASSERT_FALSE(s.isImpact);
  TEST_ASSERT_FALSE(s.isBellyUp);
  TEST_ASSERT_FALSE(s.isFallen);
  TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, s.totalAccel);
}

void test_imu_belly_up_detection() {
  TestMotionState s = {};
  evalImuState(0.05f, 0.02f, -0.98f, s); // Robot flipped on its back
  TEST_ASSERT_TRUE(s.isBellyUp);
  TEST_ASSERT_FALSE(s.isFallen);
}

void test_imu_freefall_and_impact() {
  TestMotionState s = {};
  // During drop
  evalImuState(0.05f, 0.05f, 0.08f, s); // total ~0.10g
  TEST_ASSERT_TRUE(s.isFreefall);
  TEST_ASSERT_FALSE(s.isImpact);

  // On collision hitting the floor
  evalImuState(0.8f, 0.5f, 3.2f, s); // total ~3.33g
  TEST_ASSERT_FALSE(s.isFreefall);
  TEST_ASSERT_TRUE(s.isImpact);
}

void test_imu_tilted_fall() {
  TestMotionState s = {};
  // Tilted 75 degrees on right side
  evalImuState(0.1f, 0.96f, 0.25f, s);
  TEST_ASSERT_TRUE(s.isFallen);
  TEST_ASSERT_FALSE(s.isBellyUp);
}

// =========================================================================
// 2. Logic Test: Touch Sensor Petting vs Tap
// =========================================================================
enum class TestTouchGesture {
  NONE,
  TAP,
  PETTING
};

static TestTouchGesture evaluateTouch(uint32_t durationMs, bool isReleased) {
  if (!isReleased) {
    if (durationMs >= 300) return TestTouchGesture::PETTING;
    return TestTouchGesture::NONE;
  }
  // Released
  if (durationMs >= 50 && durationMs <= 280) {
    return TestTouchGesture::TAP;
  }
  return TestTouchGesture::NONE;
}

void test_touch_gestures() {
  // Short tap: 120ms released -> TAP
  TEST_ASSERT_TRUE(evaluateTouch(120, true) == TestTouchGesture::TAP);

  // Very short glitch: 20ms -> NONE (debounced)
  TEST_ASSERT_TRUE(evaluateTouch(20, true) == TestTouchGesture::NONE);

  // Long touch ongoing: 450ms not released -> PETTING
  TEST_ASSERT_TRUE(evaluateTouch(450, false) == TestTouchGesture::PETTING);
}

// =========================================================================
// 3. Logic Test: Pet Mood State Machine & Emotional Dynamics
// =========================================================================
struct TestPetMood {
  uint8_t affection;
  uint8_t stress;
  uint8_t energy;
  const char* mood;
};

static void applyPetting(TestPetMood& p) {
  p.affection = (p.affection + 15 > 100) ? 100 : p.affection + 15;
  p.stress = (p.stress >= 20) ? p.stress - 20 : 0;
  p.mood = "purring";
}

static void applyShake(TestPetMood& p) {
  p.stress = (p.stress + 25 > 100) ? 100 : p.stress + 25;
  p.mood = "shaken_dizzy";
}

static void applyBellyUp(TestPetMood& p) {
  p.stress = (p.stress + 35 > 100) ? 100 : p.stress + 35;
  p.mood = "belly_up";
}

void test_pet_mood_dynamics() {
  TestPetMood pet = { .affection = 50, .stress = 40, .energy = 90, .mood = "idle" };

  // Vuốt ve: Stress giảm, Affection tăng, mood -> purring
  applyPetting(pet);
  TEST_ASSERT_EQUAL_STRING("purring", pet.mood);
  TEST_ASSERT_EQUAL_UINT8(65, pet.affection);
  TEST_ASSERT_EQUAL_UINT8(20, pet.stress);

  // Bị lắc liên tục: Stress tăng, mood -> shaken_dizzy
  applyShake(pet);
  TEST_ASSERT_EQUAL_STRING("shaken_dizzy", pet.mood);
  TEST_ASSERT_EQUAL_UINT8(45, pet.stress);

  // Bị lật ngửa: Stress tăng cao, mood -> belly_up
  applyBellyUp(pet);
  TEST_ASSERT_EQUAL_STRING("belly_up", pet.mood);
  TEST_ASSERT_EQUAL_UINT8(80, pet.stress);
}

void test_creative_pet_behaviors() {
  TestPetMood pet = { .affection = 50, .stress = 30, .energy = 70, .mood = "idle" };

  // 1. Cù lét: Stress giảm, Affection tăng, mood -> tickle
  pet.stress = (pet.stress >= 25) ? pet.stress - 25 : 0;
  pet.affection = (pet.affection + 15 > 100) ? 100 : pet.affection + 15;
  pet.mood = "tickle";
  TEST_ASSERT_EQUAL_STRING("tickle", pet.mood);
  TEST_ASSERT_EQUAL_UINT8(5, pet.stress);
  TEST_ASSERT_EQUAL_UINT8(65, pet.affection);

  // 2. Vỗ tay Disco: Năng lượng bùng nổ 100%, Affection tăng, mood -> disco
  pet.energy = 100;
  pet.affection = (pet.affection + 20 > 100) ? 100 : pet.affection + 20;
  pet.mood = "disco";
  TEST_ASSERT_EQUAL_STRING("disco", pet.mood);
  TEST_ASSERT_EQUAL_UINT8(100, pet.energy);
  TEST_ASSERT_EQUAL_UINT8(85, pet.affection);

  // 3. Đập tay thành công: Affection max, mood -> cool_glasses
  pet.affection = (pet.affection + 25 > 100) ? 100 : pet.affection + 25;
  pet.mood = "cool_glasses";
  TEST_ASSERT_EQUAL_STRING("cool_glasses", pet.mood);
  TEST_ASSERT_EQUAL_UINT8(100, pet.affection);
}

void test_airplane_flight_detection() {
  // Test airplane condition: 0.75g <= totalAccel <= 1.35g, mild tilt, gyro in flight range
  float ax = 0.35f, ay = 0.45f, az = 0.85f;
  float totalAccel = sqrtf(ax * ax + ay * ay + az * az); // ~1.02g
  TEST_ASSERT_TRUE(totalAccel >= 0.75f && totalAccel <= 1.35f);

  float pitch = 25.0f; // degrees banking
  float gyroSum = 45.0f; // deg/s smooth swooping
  bool isAirplane = (totalAccel >= 0.75f && totalAccel <= 1.35f && pitch >= 10.0f && gyroSum >= 20.0f && gyroSum <= 220.0f);
  TEST_ASSERT_TRUE(isAirplane);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_imu_upright_neutral);
  RUN_TEST(test_imu_belly_up_detection);
  RUN_TEST(test_imu_freefall_and_impact);
  RUN_TEST(test_imu_tilted_fall);
  RUN_TEST(test_touch_gestures);
  RUN_TEST(test_pet_mood_dynamics);
  RUN_TEST(test_creative_pet_behaviors);
  RUN_TEST(test_airplane_flight_detection);
  return UNITY_END();
}
