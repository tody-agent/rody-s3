# 🐾 Sơ Đồ Đấu Nối Rody S3 Pet Edition (Gia Tốc Kế MPU6050 + Cảm Biến Chạm)

> **Phiên bản:** `v0.3.0-pet`  
> **Mục tiêu:** Nâng cấp Robot Rody S3 thành thú cưng ảo để bàn (Desktop AI Pet) với cảm biến gia tốc 6 trục MPU6050 và cảm biến chạm điện dung trên đỉnh đầu.  
> 
> 📖 **Tài liệu phần cứng chuẩn:** Xem file chi tiết tại [WIRING_PET_S3.md](file:///Volumes/Builder/Arduino/OtooRobot/docs/hardware/WIRING_PET_S3.md)  
> 🖥️ **Mô phỏng đồ họa tương tác 2D CAD:** Mở file [wiring_pet_interactive.html](file:///Volumes/Builder/Arduino/OtooRobot/docs/hardware/wiring_pet_interactive.html) trong trình duyệt để soi từng đường dây, phóng to thu nhỏ và tra cứu mã linh kiện Shopee.

---

## 1. Danh Sách Linh Kiện Bổ Sung (BOM Thú Cưng)

Để nâng cấp từ bản Rody S3 tiêu chuẩn lên bản Pet Edition, bạn chỉ cần mua thêm **2 linh kiện** cực kỳ rẻ và phổ biến:

| # | Linh kiện | Thông số kỹ thuật | Giá tham khảo | Vai trò |
|---|-----------|-------------------|:-------------:|---------|
| 1 | **Module IMU MPU6050 (GY-521) / GY-6500 / GY-9250** | Gia tốc 3 trục $\pm 8g$ + Con quay 3 trục $\pm 1000^\circ/\text{s}$, I2C 0x68 / 0x69 | ~15.000đ – 35.000đ | Nhận biết rơi ngã, lật ngửa bụng, lắc liên tục, gõ mạnh (GY-9250 kèm la bàn AK8963) |
| 2 | **Module Cảm biến Chạm TTP223** *(Hoặc dùng giấy bạc)* | Cảm ứng điện dung mini (15x11mm), ngõ ra số HIGH khi chạm | ~4.000đ – 7.000đ | Nhận biết vuốt ve đầu, chạm nhẹ âu yếm |
| 3 | **Dây Jumper Cái-Cái** | 20cm, nhiều màu | ~5.000đ | Cắm nối các module |

*(Mẹo tiết kiệm: Nếu không dùng module TTP223, bạn có thể nối 1 sợi dây từ GPIO 2 ra 1 miếng giấy nhôm/đồng dán bên trong vỏ đầu robot, firmware sẽ tự kích hoạt chế độ `CAPACITIVE_PAD` qua `touchRead()`).*

---

## 2. Điểm Đột Phá: Chia Sẻ Bus I2C – Không Tốn Thêm Chân!

- **PCA9685** có địa chỉ I2C là `0x40`.
- **IMU (MPU6050 / GY-6500 / GY-9250)** có địa chỉ I2C mặc định là `0x68` (khi chân AD0 nối GND) hoặc `0x69` (khi AD0 nối 3.3V / kéo cao).
- **AK8963 (La bàn trên GY-9250)** tự động xuất hiện ở địa chỉ `0x0C` qua chế độ I2C Bypass.
👉 Các module này **chạy chung trên cùng đường truyền I2C (GPIO 8 SDA, GPIO 9 SCL)** mà không hề xung đột, giúp giữ nguyên toàn bộ chân GPIO khác cho Màn hình SPI, Loa I2S và Micro I2S!

---

## 3. Bảng Đấu Dây Mở Rộng

### 🔹 Module 1: MPU6050 (GY-521) / GY-6500 / GY-9250
| Chân Module IMU | Nối vào ESP32-S3 | Ghi chú |
| :---: | :---: | :---|
| **VCC** | **3.3V** | Cấp nguồn 3.3V từ bo ESP32-S3 (Khuyên dùng 3.3V) |
| **GND** | **GND** | Nối vào đất chung |
| **SCL** | **GPIO 9** | Bus I2C chung với PCA9685 |
| **SDA** | **GPIO 8** | Bus I2C chung với PCA9685 |
| **AD0** | **GND** (hoặc hở) | Nối GND = 0x68, nối 3.3V = 0x69. Firmware tự động nhận diện cả 2 |
| **INT** | *Bỏ trống* | Firmware đọc định kỳ bằng I2C polling 50Hz |

---

### 🔹 Module 2: Cảm biến Chạm TTP223
| Chân TTP223 | Nối vào ESP32-S3 | Ghi chú |
| :---: | :---: | :---|
| **VCC** | **3.3V** | Nguồn nuôi cảm ứng |
| **GND** | **GND** | Đất chung |
| **I/O (SIG)**| **GPIO 2** | Tín hiệu chạm mức HIGH khi ngón tay chạm vào |

---

## 4. Sơ Đồ Cắm Dây Chi Tiết (ASCII Diagram)

```
                            ESP32-S3 DevKitC-1 N16R8
                              +------------------+
                              |   [USB-C OTG]    |
       (Mic SCK)  GPIO 04 | [ ]          [ ] | 3V3  ----+--- Cấp 3.3V cho MPU6050
        (Mic WS)  GPIO 05 | [ ]          [ ] | GND  ----+--- Cấp GND cho MPU6050 & Touch
        (Mic SD)  GPIO 06 | [ ]          [ ] | 5V       |
      (Loa DIN)   GPIO 07 | [ ]          [ ] | GPIO 01  |
    (I2C SDA) --> GPIO 08 | [ ]          [ ] | GPIO 02  <--- Cảm biến Chạm TTP223 (SIG)
    (I2C SCL) --> GPIO 09 | [ ]          [ ] | GPIO 42  (TFT SCLK)
      (IR Trái)   GPIO 10 | [ ]          [ ] | GPIO 41  (TFT MOSI)
      (IR Phải)   GPIO 11 | [ ]          [ ] | GPIO 40  (TFT DC)
                          |   [USB-C UART]   |
                          +------------------+
                                   |      |
                                   |      +------------+
                                   v                   v
                        +--------------------+  +--------------------+
                        | Mạch PWM PCA9685   |  | MPU6050 (GY-521)   |
                        | SDA: GPIO 08       |  | SDA: GPIO 08       |
                        | SCL: GPIO 09       |  | SCL: GPIO 09       |
                        | ADDR: 0x40         |  | AD0 -> GND (0x68)  |
                        +--------------------+  +--------------------+
```

---

## 5. Vị Trí Lắp Đặt Thực Tế Khuyên Dùng

1. **Module MPU6050**:
   - Dán bằng băng dính xốp 2 mặt ở **đáy khung robot**, nằm giữa 2 bánh xe và song song với mặt đất.
   - Hướng mũi tên trục $Z$ hướng thẳng lên trên trời (trục $X$ hướng về phía trước mui xe).
   - Khi robot bị lật ngửa bụng, trục $Z$ sẽ bị đảo chiều âm ($a_z < -0.5g$) giúp thuật toán kích hoạt phản xạ giãy giụa chính xác 100%.

2. **Cảm Biến Chạm (TTP223)**:
   - Dán mặt cảm ứng úp sát vào **mặt trong đỉnh đầu** vỏ 3D robot.
   - Sóng điện dung có khả năng xuyên qua lớp nhựa in 3D (dày 1.5mm – 2.5mm) cực kỳ nhạy, giúp bề ngoài robot liền mạch, vuốt ve vào đầu là robot cảm nhận được ngay!
