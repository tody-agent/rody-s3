# Rody S3 – Hướng Dẫn Triển Khai Siêu Tối Giản (Minimalist Setup v1.0)

> **Mục tiêu:** Rút gọn tối đa số lượng linh kiện, không cần hàn trở/diot/tụ, không cần mạch tăng áp hay module đo áp. Cắm dây jumper là chạy ngay!

---

## 1. Danh sách linh kiện CẦN & ĐỦ (BOM Tối Giản)

| # | Linh kiện | Số lượng | Ghi chú |
|---|-----------|:--------:|---------|
| 1 | **ESP32-S3 DevKit** (N16R8, 40 pin) | 1 | Bo điều khiển trung tâm |
| 2 | **Mạch PCA9685** (16 kênh PWM I2C) | 1 | Địa chỉ mặc định 0x40, giữ nguyên |
| 3 | **Động cơ Servo MG90S 360°** (hoặc 180° liên tục) | 2 | Bánh trái (Kênh 0), Bánh phải (Kênh 1) |
| 4 | **Module Cảm biến hồng ngoại IR** (TCRT5000 / LM393) | 2 | Dò line & Đĩa sọc đo tốc độ (Tachometer) |
| 5 | **Mạch sạc TP4056 có bảo vệ** (cổng B+/B-, OUT+/OUT-) | 1 | Sạc & quản lý pin an toàn |
| 6 | **Pin Li-ion 1S 3.7V** (18650 hoặc Lipo) | 1 | Nguồn cấp cho động cơ |
| 7 | **Dây cắm breadboard/jumper** (Đực-Cái, Đực-Đực) | ~12 sợi | Không cần hàn |

### ❌ Các linh kiện ĐÃ LOẠI BỎ (Không cần mua/không cần lắp):
- ❌ **Mạch tăng áp (Boost 5V MT3608)**: Bỏ qua, 2 servo MG90S chạy trực tiếp từ pin 1S (3.7V–4.2V) rất mượt.
- ❌ **Diode Schottky (1N5819)**: Bỏ qua.
- ❌ **Tụ hóa (470–1000µF)**: Bỏ qua (chạy 2 servo nhỏ tải nhẹ không bắt buộc).
- ❌ **Module đo áp (0–25V)**: Bỏ qua (Firmware đã được cập nhật tính năng tự nhận diện/bypass).
- ❌ **Module chuyển mức logic (Level Shifter 3.3V↔5V)**: Bỏ qua hoàn toàn.
- ❌ **Cảm biến siêu âm HC-SR05/04**: Tạm hoãn.

---

## 2. Sơ đồ khối & Chiến lược cấp nguồn an toàn

### A. Khi nạp code & Calib trên bàn (Bench Mode - Cắm cáp USB máy tính):
```
  [Máy tính] ──Cáp USB-C──► ESP32-S3 (Chân 3V3 cấp nguồn logic cho PCA9685 & 2x IR)
                               │
                              GND (Nối chung GND toàn hệ thống)
                               ▲
  [Pin 3.7V] ──► TP4056 ──OUT+/OUT-──► Cọc vít V+/GND của PCA9685 ──► Nuôi riêng 2 Servo
```
> **Ưu điểm lớn:** ESP32 ăn nguồn sạch 5V từ cổng USB. Động cơ ăn nguồn riêng từ Pin 1S. Hai nguồn độc lập chỉ nối chung mass GND -> **Không bao giờ lo sụt áp máy tính hay reset brownout!**

---

### B. Khi chạy độc lập dưới sàn (Floor Mode - Không cắm máy tính):
```
  [Pin 3.7V] ──► TP4056 ──OUT+ ──┬──► Cọc V+ PCA9685 (Nuôi 2 Servo)
                                 │
                                 └──► Chân 5V (hoặc VIN) của ESP32 (*)
  
  [Pin 3.7V] ──► TP4056 ──OUT- ──────► GND chung (Cọc GND PCA9685 & Chân GND ESP32)
```
> ⚠️ **LƯU Ý SỐNG CÒN (*):** Vì không có Diode chặn dòng, **PHẢI RÚT dây OUT+ khỏi chân 5V của ESP32** trước khi cắm lại cáp USB vào máy tính!
> 
> *(Mẹo cực tiện: Nếu có 1 cục sạc dự phòng điện thoại nhỏ, bạn chỉ cần cắm cáp USB từ sạc dự phòng vào ESP32 là xong, không cần câu dây OUT+ vào chân 5V!)*

---

## 3. Bảng kết nối dây chi tiết (Tổng cộng chỉ 10 bước nối dây)

### 3.1. Nối ESP32-S3 với Mạch PCA9685 (4 dây)
| Chân ESP32-S3 | Chân PCA9685 (Hàng 6 chân bên hông) | Tác dụng | Màu dây khuyên dùng |
|:---:|:---:|:---|:---:|
| **3V3** (Pin 1) | **VCC** | Cấp nguồn nuôi chip logic PCA9685 | Đỏ / Trắng |
| **GND** (Pin 20 hoặc 21) | **GND** | Nối mass chung | Đen |
| **IO8** (Pin 11) | **SDA** | Tín hiệu I2C Data | Vàng |
| **IO9** (Pin 14) | **SCL** | Tín hiệu I2C Clock | Xanh lá / Tím |
| *(Bỏ trống)* | `V+`, `OE` | Không cắm vào hàng chân này | — |

---

### 3.2. Cấp nguồn động cơ vào Cọc Vít PCA9685 (Terminal xanh 2 chân)
| Cọc Vít PCA9685 | Nguồn nối tới | Ghi chú |
|:---:|:---|:---|
| **Cọc V+** | Chân **OUT+** của mạch sạc TP4056 | Cấp dòng xả từ pin trực tiếp cho Servo |
| **Cọc GND** | Chân **OUT-** của mạch sạc TP4056 | Mass nguồn động cơ |

---

### 3.3. Cắm 2 Động cơ Servo MG90S vào PCA9685
- **Bánh Trái (Wheel L):** Cắm vào **Kênh 0** (Hàng 0).
- **Bánh Phải (Wheel R):** Cắm vào **Kênh 1** (Hàng 1).
- **Thứ tự 3 chân cắm:**
  - Chân màu **Cam / Vàng** (PWM Signal): Hàng chân trên cùng (kí hiệu `PWM`).
  - Chân màu **Đỏ** (V+): Hàng chân ở giữa (kí hiệu `V+`).
  - Chân màu **Nâu / Đen** (GND): Hàng chân dưới cùng (kí hiệu `GND`).

---

### 3.4. Nối 2 Cảm biến Dò Line Hồng Ngoại (IR)
| Module IR | Chân IR | Chân ESP32-S3 | Ghi chú |
|:---|:---:|:---:|:---|
| **IR Trái (Left)** | **VCC** | **3V3** | Cắm chung vào chân 3V3 của ESP32 |
| | **GND** | **GND** | Cắm chung vào chân GND của ESP32 |
| | **DO** | **IO6** | Tín hiệu digital dò line / tachometer |
| | *AO* | *(Bỏ trống)* | Không dùng |
| **IR Phải (Right)** | **VCC** | **3V3** | Cắm chung vào chân 3V3 của ESP32 |
| | **GND** | **GND** | Cắm chung vào chân GND của ESP32 |
| | **DO** | **IO7** | Tín hiệu digital dò line / tachometer |
| | *AO* | *(Bỏ trống)* | Không dùng |

---

## 4. Lộ trình cảm biến khoảng cách: Laser VL53L0X (Thay thế HC-SR05)

Khi bạn muốn bổ sung tính năng né vật cản, **Cực kỳ khuyên dùng Cảm biến khoảng cách Laser ToF VL53L0X** thay cho cảm biến siêu âm:

| Đặc điểm | HC-SR05 / HC-SR04 (Siêu âm) | **VL53L0X (Laser ToF - Đề xuất)** |
|:---|:---|:---|
| **Điện áp hoạt động** | 5V (Echo ra 5V làm hỏng ESP32 nếu không có Level Shifter) | **2.8V – 3.3V** (Cắm trực tiếp vào ESP32 3V3) |
| **Số chân tốn trên ESP32** | 2 chân GPIO riêng (`IO4`, `IO5`) | **0 chân mới** (Dùng chung bus I2C `IO8` SDA, `IO9` SCL với PCA9685!) |
| **Mạch chuyển mức (Shifter)** | **BẮT BUỘC CÓ** | **HOÀN TOÀN KHÔNG CẦN** |
| **Độ chính xác & Chống nhiễu**| Dễ nhiễu do góc chùm âm rộng, dội góc chéo | Đo bằng tia laser quang tử, thẳng tắp, chính xác từng mm (3cm–200cm) |
| **Kích thước** | To cồng kềnh (2 mắt tròn to) | Siêu nhỏ gọn (bằng đầu ngón tay) |
| **Giá thành** | ~20.000đ – 30.000đ | ~28.000đ – 38.000đ trên Shopee |

---

## 5. Tự động tương thích trong Code (Firmware)

Firmware đã được cập nhật tự động xử lý khi thiếu module đo pin:
1. **Tự động nhận diện (Auto-detect):** Nếu chân `IO1` đọc điện áp < 1.0V (do không gắn module chia áp), hệ thống tự kích hoạt chế độ **Bypass**:
   - Không báo lỗi `WARNING: Battery voltage low`.
   - Không chặn lệnh quay động cơ `drive`, `pwm`, `calib`.
   - Đèn LED RGB hiển thị màu Xanh/Vàng bình thường thay vì bị kẹt màu Đỏ.
2. Khi nào bạn gắn thêm module đo áp trong tương lai, code sẽ tự động nhận diện lại bình thường mà không cần sửa firmware.
