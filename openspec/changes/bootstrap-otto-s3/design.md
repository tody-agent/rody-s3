# Design

## 1. Kiến trúc firmware
src/
  main.cpp        setup/loop, chọn mode
  console.cpp     parser lệnh serial → JSON
  drive.cpp       PCA9685, pulseFor() từ lib calib, watchdog
  sensors.cpp     ultrasonic, IR, battery, tach ISR
  calib_run.cpp   các thủ tục đo (gọi lib/otto_calib)
  store.cpp       NVS load/save
  behaviors.cpp   manual/avoid/line
  web.cpp         SoftAP "Otto-S3" + trang điều khiển
lib/otto_calib    toán thuần, test native

## 2. Giao thức serial (115200, '\n')
Phản hồi: {"cmd":"<token đầu>","ok":true|false, ...}. Lỗi: "err":"<mô tả>".
| Lệnh | Phản hồi chính |
|------|----------------|
| ping | fw |
| info | fw, flash, psram, heap, uptime_ms, reset_reason, cal_valid |
| i2c | devices:[int] |
| bat | v |
| us [n=5] | cm:[...], valid |
| ir | l, r (1 = phát hiện) |
| tach <ms> | l, r (số cạnh), rpm_l, rpm_r |
| pwm <L|R> <us|off> | – (tự tắt sau 10s) |
| drive <l%> <r%> <ms≤5000> | – (dùng hiệu chỉnh; tự stop) |
| stop | – |
| cal deadband <L|R> | lo, hi |
| cal dir <L|R> <+1|-1> | – |
| cal spin <L|R> | – (quay 1.5s, xung > db_hi, để người quan sát chiều) |
| cal sweep <L|R> | fwd:[12], rev:[12] (rpm) |
| meas <speed%> | target, rpm_l, rpm_r, err_pct |
| cal wheelbase <cm> | – |
| cal drift <d_cm> <D_cm> | trim_l, trim_r (d>0 = lệch TRÁI) |
| cal show / save / reset | dữ liệu hiệu chỉnh |
| mode <manual|avoid|line> | – |

## 3. Tachometer quang
Đĩa 4 đen/4 trắng, ngắt CHANGE → 8 cạnh/vòng. ISR:
```cpp
volatile uint32_t gEdges[2], gLastUs[2];
void IRAM_ATTR isrTach(void* arg) {
  int i = (int)(intptr_t)arg; uint32_t t = micros();
  if (t - gLastUs[i] > 2000) { gEdges[i]++; gLastUs[i] = t; }   // debounce 2ms
}
// attachInterruptArg(pins::IR_L, isrTach, (void*)0, CHANGE);
```
Tach chỉ dùng ở cổng G1; ở G2 hai IR trở lại vai trò dò line (cùng chân).

## 4. Thuật toán hiệu chỉnh
1. Deadband: quét 1350→1650µs bước 4; mỗi điểm giữ 600ms (bỏ 200ms đầu), đếm cạnh
   trong 400ms; moving = edges ≥ 1. Lấy dải đứng yên dài nhất gần 1500 → [lo, hi].
   Sai số dao động thạch anh của PCA9685 không ảnh hưởng vì mọi điểm đều đo thực tế.
2. Chiều quay: `cal spin`, người xác nhận → `cal dir` (fwd_sign).
3. Đường cong: offset {0,10,20,30,40,60,80,100,130,160,200,250}µs, mỗi hướng,
   xung = pulseAt(); giữ 1.7s, đo 1.5s. makeMonotonic().
4. Cân bằng: vmax = 0.9 × min(rpm max của 4 đường cong).
   speed% → rpm mục tiêu = |s|/100·vmax·trim → nội suy ngược offset → xung.
5. Trim sàn: lệch ngang d sau quãng D, cầu bánh W. Bán kính quay R = D²/(2d)
   ⇒ vR/vL − 1 ≈ W/R = 2dW/D². Hệ số r = 1 − 2dW/D² (kẹp 0.8–1.2);
   r<1 → trim_r *= r; r>1 → trim_l *= 1/r (chỉ giảm bánh nhanh, không vượt vmax).

## 5. Lưu trữ
struct CalBlob { uint16_t ver; Wheel l, r; float wheelbase_cm; uint32_t crc; }.
Khi CRC hoặc version sai → cal_valid=false; lệnh drive báo lỗi "uncalibrated"
(pwm thô vẫn dùng được).

## 6. An toàn
Boot: PCA full-off. drive/pwm có hạn. Pin < 3.4V → stop, LED đỏ, chỉ cho phép stop/info.
Mất kết nối WiFi client > 1s ở mode manual → stop.
