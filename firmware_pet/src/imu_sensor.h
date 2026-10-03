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

// Khởi tạo cảm biến IMU (MPU6050 / GY-6500 / GY-9250)
bool init();

// Đọc và cập nhật trạng thái gia tốc (gọi tuần hoàn trong loop, chu kỳ 20-50ms)
void update();

// Lấy thông tin trạng thái chuyển động hiện tại
const MotionState& getState();

// Kiểm tra xem cảm biến IMU có phản hồi trên I2C không
bool isAvailable();

// Đặt lại các cờ va đập / giật mình sau khi đã xử lý
void clearTransientFlags();

// Phân loại dòng chip quán tính
enum class ChipType : uint8_t {
  UNKNOWN = 0,
  MPU6050 = 1,
  MPU6500 = 2,
  MPU9250 = 3,
  MPU9255 = 4
};

// Lấy tên chip IMU đã nhận diện dưới dạng chuỗi
const char* getChipName();

// Lấy loại chip enum
ChipType getChipType();

// Lấy địa chỉ I2C đang hoạt động (0x68 hoặc 0x69)
uint8_t getActiveAddress();

// Kiểm tra xem la bàn số AK8963 (MPU9250) có phản hồi không
bool hasMagnetometer();

} // namespace imu_sensor
