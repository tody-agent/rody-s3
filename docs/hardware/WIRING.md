# Otto S3 – Wiring Standard v1.0

## 1. BOM
| # | Linh kiện | SL | Ghi chú |
|---|-----------|----|---------|
| 1 | ESP32-S3 N16R8 DevKit 40 pin, 2 USB-C | 1 | |
| 2 | PCA9685 16 kênh | 1 | địa chỉ mặc định 0x40, KHÔNG hàn jumper địa chỉ |
| 3 | MG90S 360° | 2 | |
| 4 | HC-SR05 | 1 | chân OUT bỏ trống |
| 5 | Module IR (DO/AO) | 2 | chỉ dùng DO |
| 6 | Module chuyển mức logic 4 kênh 3.3V↔5V | 1 | |
| 7 | Module đo áp 0–25V | 1 | hệ số chia 5 |
| 8 | TP4056 có bảo vệ (cổng B+/B−/OUT+/OUT−) | 1 | |
| 9 | Boost 5V ≥2A, chỉnh được áp | 1 | |
| 10 | Diode Schottky 1N5819 (hoặc SS14/SS34) | 1 | OR nguồn ESP32 |
| 11 | Tụ hóa 470–1000µF ≥10V | 1 | nếu PCA9685 chưa có sẵn tụ |
| 12 | Công tắc gạt | 1 | |
| 13 | Buzzer active/passive 3.3V (tùy chọn) | 1 | |
| 14 | Pin Li-ion 3.7V 700mAh | 1 | bỏ nếu phồng hoặc <2.5V khi chưa sạc |

## 2. Quy ước màu dây
ĐỎ = 5V servo (SERVO_5V) · CAM = 5V logic (LOGIC_5V) · TRẮNG = 3V3 · ĐEN = GND ·
VÀNG = I2C · XANH LÁ = tín hiệu cảm biến · XANH DƯƠNG = Trig/Echo.

## 3. Cây nguồn
```
 Pin 3.7V ──B+/B−──► TP4056 ──OUT+──► [CÔNG TẮC] ──► VBAT_SW ──┬──► Module đo áp (VCC/GND)
                         ▲                                    │
                     USB sạc                                  └──► Boost IN+ / IN−
                                                                     │
                                         Boost OUT+ (5.1V) ──┬───────┴──► SERVO_5V ──► PCA9685 V+ (cọc vít)
                                                             │                         + tụ 470–1000µF
                                                             └──►|── 1N5819 ──► LOGIC_5V = chân 5V của ESP32
                                                                (vạch diode về phía ESP32)
 ESP32 3V3 ──► 3V3 rail: PCA9685 VCC, module IR VCC, level shifter LV
 LOGIC_5V  ──► HC-SR05 VCC, level shifter HV
 TẤT CẢ GND NỐI CHUNG (sao về chân GND của boost)
```
Diode chặn dòng đi từ USB ngược sang boost và ngược lại, nên được phép cắm USB khi
công tắc đang bật. Servo KHÔNG BAO GIỜ lấy nguồn từ USB.

## 4. Bảng chân
| ESP32-S3 | Hướng | Nối tới | Màu |
|----------|-------|---------|-----|
| GPIO8 | I2C SDA | PCA9685 SDA | vàng |
| GPIO9 | I2C SCL | PCA9685 SCL | vàng |
| GPIO4 | OUT | Level shifter LV1 → HV1 → HC-SR05 Trig | xanh dương |
| GPIO5 | IN | Level shifter LV2 ← HV2 ← HC-SR05 Echo | xanh dương |
| GPIO6 | IN (interrupt) | IR trái DO | xanh lá |
| GPIO7 | IN (interrupt) | IR phải DO | xanh lá |
| GPIO1 | ADC1 | Module đo áp chân S | xanh lá |
| GPIO10 | OUT | Buzzer + | – |
| GPIO0 | IN | nút BOOT trên board (đổi chế độ) | – |
| GPIO48 | OUT | LED RGB trên board (kiểm tra, một số bản là GPIO38) | – |
| 3V3 | – | PCA VCC, IR VCC×2, shifter LV | trắng |
| 5V | – | ← diode; → HC-SR05 VCC, shifter HV | cam |
| GND | – | chung | đen |

PCA9685: kênh 0 = servo TRÁI, kênh 1 = servo PHẢI (dây nâu = GND, đỏ = V+, cam = PWM).
OE của PCA9685 để trống. Module đo áp: cọc VCC/GND ← VBAT_SW (sau công tắc,
để khi tắt máy module không rút pin); chân "−" → GND.

## 5. Chân cấm dùng
GPIO19/20 (USB), 26–32 (flash), 33–37 (PSRAM octal), 43/44 (UART0),
0/3/45/46 (strapping – chỉ dùng làm input sau boot).

## 6. Đồ gá cân chỉnh (bắt buộc cho Gate G1)
- In tools/tach_disc.svg (Ø30mm, 4 sọc đen / 4 trắng), dán vào mặt trong mỗi bánh.
- Gắn tạm mỗi module IR cách đĩa 3–8mm, vặn biến trở tới khi LED DO đổi trạng thái
  theo từng sọc khi quay bánh bằng tay.
- Kê thân robot sao cho 2 bánh quay tự do.

## 7. Checklist đo trước khi cấp nguồn (Gate G0)
| ID | Đo | Đạt |
|----|----|-----|
| M1 | Ω giữa SERVO_5V–GND, LOGIC_5V–GND, 3V3–GND (rút nguồn) | > 1kΩ, không chạm |
| M2 | Áp pin tại B+ | 3.0–4.2V |
| M3 | Boost OUT khi CHƯA nối tải (chỉnh biến trở) | 5.05–5.20V |
| M4 | LOGIC_5V sau diode (bật công tắc, chưa cắm USB) | 4.7–5.0V |
| M5 | 3V3 trên board | 3.25–3.35V |
| M6 | Chân S module đo áp | ≈ VBAT/5 (0.6–0.85V) |
| M7 | Phía LV của level shifter, kênh Echo, khi siêu âm hoạt động | ≤ 3.4V |
| M8 | Cực tính diode: vạch về phía ESP32 | đúng |
