#pragma once
#include <stdint.h>

namespace touch_sensor {

enum class TouchMode {
  DIGITAL_TTP223,   // Module cảm ứng TTP223 rời (digitalRead)
  CAPACITIVE_PAD    // Cảm ứng điện dung tích hợp của ESP32-S3 (touchRead)
};

// Khởi tạo cảm biến chạm trên GPIO 2
void init(TouchMode mode = TouchMode::DIGITAL_TTP223);

// Cập nhật trạng thái cảm ứng (gọi tuần hoàn trong loop)
void update();

// Đang có tiếp xúc chạm vào đầu robot ngay lúc này
bool isTouched();

// Được vuốt ve liên tục (> 300ms)
bool isPetting();

// Thời gian vuốt ve liên tục tính bằng milliseconds
uint32_t getPetDurationMs();

// Có một cú chạm nhẹ ngắn vừa kết thúc (50ms - 250ms)
bool checkAndClearTap();

// Bị cù lét: Người dùng chạm liên tục nhanh 3+ lần (multi-tap < 600ms)
bool checkAndClearTickle();

} // namespace touch_sensor
