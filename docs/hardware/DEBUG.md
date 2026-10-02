# Debug playbook (agent dùng khi test FAIL)

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
