# Proposal: bootstrap-otto-s3

## Why
Robot Otto HP Starter dùng mạch HP riêng. Cần firmware và quy trình kiểm thử để chạy
trên ESP32-S3 N16R8 + PCA9685, với servo 360° không encoder nên phải cân bằng
2 bánh bằng phép đo.

## What changes
- Cấu hình build/nạp cho ESP32-S3 N16R8.
- Chuẩn đi dây + nguồn an toàn (cho phép USB và pin cùng lúc).
- Serial console JSON để agent tự kiểm thử.
- An toàn chuyển động: timeout, stop tức thì, cảnh báo pin yếu.
- Driver cảm biến: siêu âm, IR, pin, tachometer quang.
- Hiệu chỉnh: deadband, chiều quay, đường cong tốc độ, cân bằng, trim theo độ lệch sàn.
- Hành vi: điều khiển tay (WiFi), tránh vật, dò line.

## Out of scope
Otto chân đi bộ, MicroPython, app HP Robots, OTA.

## Impact
Phần cứng mới cần: diode Schottky, tụ lọc, đĩa sọc in giấy.
