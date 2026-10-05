# Debug playbook (agent dùng khi test FAIL)
> 💡 *Xem cẩm nang chuyên sâu về toolchain, debug crash dump và xử lý sự cố toàn diện tại: [**`docs/FIRMWARE_GUIDE.md`**](../FIRMWARE_GUIDE.md)*

| Triệu chứng | Nguyên nhân hay gặp | Kiểm tra / xử lý |
|---|---|---|
| Upload lỗi "No serial data received" | chưa vào chế độ download | giữ BOOT, nhấn-nhả RST, nhả BOOT; thử cổng USB-C còn lại |
| Có cổng nhưng serial im lặng | sai cấu hình CDC | cổng UART (chip cầu USB) → ARDUINO_USB_CDC_ON_BOOT=0; cổng USB native → =1 |
| Boot loop, log báo lỗi PSRAM | sai memory_type | phải là qio_opi + psram_type=opi (N16R8) |
| info.flash ≠ 16MB | board khác mô tả | `esptool --chip esp32s3 flash-id` (bản cũ: flash_id), cập nhật platformio.ini |
| Reset khi servo bắt đầu quay (reset_reason=BROWNOUT) | sụt áp | thêm tụ 1000µF ở V+, kiểm tra M3/M4, dây nguồn ngắn/to, pin yếu |
| i2c không thấy 0x40 | SDA/SCL đảo, PCA thiếu VCC | đo 3V3 ở VCC của PCA; thấy thêm 0x70 là bình thường (địa chỉ All-Call) |
| Servo giật dù đang "stop" | đang xuất xung trong vùng chết | stop phải dùng full-off (setPin 0), không gửi 1500µs |
| Bánh bò chậm ở speed 0 | deadband sai | chạy lại `cal deadband` |
| tach luôn = 0 | IR quá xa/biến trở sai | LED DO phải nháy theo sọc khi quay tay |
| tach đếm quá lớn, không ổn định | nhiễu/ánh sáng | che sáng; debounce ≥2ms; đĩa mực đen mờ |
| us = 999 liên tục | Echo không qua shifter / HV thiếu 5V | đo M7; kiểm tra HV=5V, LV=3V3 |
| bat đọc sai | sai hệ số | so với đồng hồ, chỉnh BAT_DIV (mặc định 5.0) |
| Chạy thẳng vẫn lệch sau cân bằng | lốp khác cỡ, ma sát bi | `cal drift` lặp lại; kiểm tra bi lăn trơn |
| Màn hình TFT tối đen | Chân BLK chưa kéo cao / thiếu 3V3 | Kiểm tra GPIO 21 (BLK), đo 3V3 tại chân VCC màn hình |
| Màn hình trắng xóa hoặc sọc nhiễu | Sai chân SPI hoặc clock quá cao | Kiểm tra SCLK (IO42), MOSI (IO41), DC (IO40), CS (IO38), RST (IO39) |
| Loa MAX98357A không kêu / rè | Thiếu nguồn 5V Boost hoặc sai chân I2S | Đo 5V tại VIN amply; kiểm tra BCLK (IO16), LRC (IO15), DIN (IO7); SPK- CẤM GND |
| Mic INMP441 không thu được âm | Cấp nhầm 5V hoặc chân L/R bị hở | Đo VDD phải là 3.3V; chân L/R phải nối đất GND; kiểm tra SCK (IO4), WS (IO5), SD (IO6) |

