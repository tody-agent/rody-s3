#pragma once
#include <stdint.h>

namespace acoustic_monitor {

// Khởi tạo bộ theo dõi âm thanh qua I2S0 (INMP441)
bool init();

// Cập nhật và xử lý mẫu âm thanh (gọi tuần hoàn trong loop)
void update();

// Cường độ âm thanh hiện tại (0 - 100%)
uint8_t getCurrentRmsLevel();

// Phát hiện môi trường quá ồn ào liên tục (> 70% trong > 2.5 giây)
bool isLoudNoiseSustained();

// Phát hiện xung âm thanh va đập mạnh đột ngột (Knock spike)
bool checkAndClearAudioSpike();

// Phát hiện vỗ tay 2 cái liên tiếp (Clap-Clap nhịp điệu 160-550ms)
bool checkAndClearClap();

// Phát hiện người dùng ghé sát mic thổi luồng gió mạnh (> 350ms)
bool isBlowingAir();

// Thời gian môi trường ồn ào liên tục tính bằng ms
uint32_t getNoiseDurationMs();

} // namespace acoustic_monitor
