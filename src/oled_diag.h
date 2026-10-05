#pragma once
#include <Arduino.h>
#include <stdint.h>

namespace oled_diag {

struct ScanResult {
  bool found = false;
  int sdaPin = -1;
  int sclPin = -1;
  uint8_t address = 0;
  bool isSh1106 = false;
};

// Quét toàn diện các cặp chân GPIO và phát hiện màn hình 0.96" I2C (SSD1306 / SH1106)
ScanResult scanAllCandidatePins();

// Quét header đã biết (I2C, TFT, IR, siêu âm). Không đụng chân I2S, không ghi loại màn.
ScanResult scanKnownHeaders();

// Ép sáng toàn bộ màn hình OLED ở mức phần cứng (Hardware Force Full Brightness)
// Sử dụng lệnh 0x8D/0x14 (Charge Pump ON), 0x81/0xFF (Max Contrast), 0xAF (Display ON), 0xA5 (All Pixels ON)
bool forceHardwareLightUp(int sda, int scl, uint8_t addr);

// Chớp nháy màn hình và vẽ mẫu thử nghiệm kiểm tra điểm chết
bool runVisualTest(int sda, int scl, uint8_t addr);

// Báo cáo chi tiết điện áp / trở kéo trên các chân GPIO
String getPinDiagnosticsReport();

// True when at least 4 of MOSI/SCLK/DC/RST/CS have an external pull-up.
bool spiHeaderConnected();

// Bit-bang SSD1306 bring-up plus all-pixels-on (0xA5) on the display header.
bool forceSpiPanelOn();

// Bơm xung đánh thức và ép sáng phần cứng đồng thời trên tất cả các chân ứng viên
bool pulseAllPossibleOledHeaders();

} // namespace oled_diag
