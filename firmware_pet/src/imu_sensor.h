#pragma once
#include <stdint.h>

namespace imu_sensor {

struct MotionState {
  float ax, ay, az;         // Gia tốc 3 trục (đơn vị: g, 1g ~ 9.8m/s^2)
  float gx, gy, gz;         // Vận tốc góc 3 trục (đơn vị: deg/s)
  float pitch, roll;        // Góc nghiêng (đơn vị: độ)
  float totalAccel;         // Độ lớn gia tốc tổng hợp |a|
  
  bool isFreefall;          // Rơi tự do (|a| < 0.25g)
  bool isImpact;            // Va đập mạnh (|a| > 2.8g)
  bool isFallen;            // Bị ngã / té nghiêng (> 65 độ)
  bool isBellyUp;           // Bị lật ngửa bụng (az < -0.5g)
  bool isShaking;           // Bị lắc lắc liên tục
  bool isKnocked;           // Bị gõ mạnh đột ngột
  bool isAirplaneMode;      // Được nhấc bổng bay lượn trên không trung
};

// Khởi tạo cảm biến MPU6050
bool init();

// Đọc và cập nhật trạng thái gia tốc (gọi tuần hoàn trong loop, chu kỳ 20-50ms)
void update();

// Lấy thông tin trạng thái chuyển động hiện tại
const MotionState& getState();

// Kiểm tra xem MPU6050 có phản hồi trên I2C không
bool isAvailable();

// Đặt lại các cờ va đập / giật mình sau khi đã xử lý
void clearTransientFlags();

} // namespace imu_sensor
