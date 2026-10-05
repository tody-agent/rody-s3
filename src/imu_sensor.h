#pragma once
#include <stdint.h>

namespace imu_sensor {

struct MotionState {
  float ax, ay, az;         // Gia tốc 3 trục (đơn vị: g, 1g ~ 9.8m/s^2)
  float gx, gy, gz;         // Vận tốc góc 3 trục (đơn vị: deg/s)
  float pitch, roll;        // Góc nghiêng (đơn vị: độ)
  float totalAccel;         // Độ lớn gia tốc tổng hợp |a|

  bool isFreefall;          // Rơi tự do (|a| < 0.25g)
  bool isImpact;            // Va đập mạnh / tiếp đất đột ngột
  bool isFallen;            // Bị ngã / té nghiêng (> 65 độ)
  bool isBellyUp;           // Bị lật ngửa bụng (az < -0.45g)
  bool isShaking;           // Bị lắc lắc liên tục
  bool isKnocked;           // Bị gõ mạnh đột ngột
  bool isAirplaneMode;      // Được nhấc bổng bay lượn trên không trung
};

enum class ChipType : uint8_t {
  UNKNOWN = 0,
  MPU6050 = 1,
  MPU6500 = 2,
  MPU9250 = 3,
  MPU9255 = 4
};

bool init();
void update();
const MotionState& getState();
bool isAvailable();
void clearTransientFlags();
const char* getChipName();
ChipType getChipType();
uint8_t getActiveAddress();
bool hasMagnetometer();

} // namespace imu_sensor
