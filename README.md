# 🤖 Robot Rody S3 (Mochi AI Edition)

> **Robot Trợ Lý Để Bàn, Xe Tự Hành Biểu Cảm AI & Hệ Sinh Thái Chẩn Đoán Phần Cứng**  
> Xây dựng trên nền tảng vi điều khiển **ESP32-S3 DevKit N16R8** (16MB Flash, 8MB Octal PSRAM), kết hợp màn hình đôi mắt cảm xúc **TFT 1.54" ST7789**, hệ thống âm thanh I2S hai chiều (**Mic INMP441** & **Loa MAX98357A**), bộ điều khiển xung **PCA9685**, và dẫn động 2 bánh vi sai Servo liên tục 360°.

---

## 🌐 Trải Nghiệm Trực Tiếp Trên Trình Duyệt (GitHub Pages)

Tất cả ứng dụng web, game 3D và sơ đồ mạch thông minh đã được publish công khai tại:

👉 **[https://tody-agent.github.io/rody-s3/](https://tody-agent.github.io/rody-s3/)**

| Trải Nghiệm | Mô Tả & Điểm Nổi Bật | Link Trực Tiếp |
| :--- | :--- | :---: |
| 🎮 **Rody 3D Cyber Arena** | Game 3D WebGL Three.js sa bàn đấu trường, điều khiển robot né chướng ngại vật | [Vào chơi Game 3D](https://tody-agent.github.io/rody-s3/rody_3d_game.html) |
| 🩺 **Hardware Diagnostic Center** | Trung tâm đo kiểm phần cứng Light Mode (OpenDesign): đo nhiệt độ CPU, quét bus I2C, nhích servo | [Mở Chẩn Đoán](https://tody-agent.github.io/rody-s3/rody_debug_center.html) |
| 📐 **Sơ Đồ Mạch Smart CAD Rody S3** | Bản vẽ đấu dây 2D tương tác thông minh, tra cứu chân pin, hover dây dẫn, né linh kiện | [Xem Sơ Đồ Mạch](https://tody-agent.github.io/rody-s3/hardware/wiring_interactive.html) |
| 🐾 **Sơ Đồ Mạch Rody Pet (Sensory)** | Sơ đồ đấu dây tích hợp cảm biến gia tốc MPU6050 & cảm biến chạm vuốt ve TTP223 | [Xem Sơ Đồ Pet](https://tody-agent.github.io/rody-s3/hardware/wiring_pet_interactive.html) |
| 📺 **Sơ Đồ Mạch Xiaozhi Voice & TFT** | Sơ đồ đấu dây màn hình ST7789 SPI DMA + Loa I2S MAX98357A + Mic I2S INMP441 | [Xem Sơ Đồ Voice](https://tody-agent.github.io/rody-s3/hardware/wiring_xiaozhi_interactive.html) |
| 🐾 **Pet Sensory Simulator** | Giả lập robot thú cưng AI: vuốt ve đầu, lật ngửa bụng, lắc chóng mặt, kêu meo meo | [Mở Giả Lập Pet](https://tody-agent.github.io/rody-s3/pet_simulator.html) |
| 🚗 **2D Autonomous Drive Simulator** | Giả lập xe tự hành sa bàn 2D, điều khiển gamepad, 12 biểu cảm mắt ST7789 60 FPS | [Mở Giả Lập Xe](https://tody-agent.github.io/rody-s3/robot_simulator.html) |

---

## 🌟 Điểm Nổi Bật Của Rody S3

- 🖥️ **Màn hình biểu cảm ST7789 1.54" (240x240 IPS)**: 12 trạng thái đôi mắt Mochi / Xiaozhi (Vui vẻ, Lắng nghe, Suy nghĩ, Nói chuyện, Tiến, Lùi, Liếc mắt, Chóng mặt, Vật cản, Ngủ say...) hiển thị 60 FPS, vẽ hai lớp (double-buffered sprite) không giật hình.
- 🎙️ **Tương tác Giọng nói AI (I2S0 INMP441)**: Lắng nghe và nhận diện các khẩu lệnh tiếng Việt (*"Rody ơi", "Tiến lên", "Lùi lại", "Rẽ trái", "Rẽ phải", "Dừng lại", "Quay vòng", "Vui vẻ"*) với thuật toán đo năng lượng âm thanh RMS thời gian thực.
- 🔊 **Phát âm thanh Robot SFX (I2S1 MAX98357A)**: Loa công suất kết hợp amply kỹ thuật số Class-D phát âm thanh khởi động, tiếng hót chim R2-D2, chuông nhận diện lệnh, còi cảnh báo vật cản.
- 🚗 **Điều khiển Động cơ PCA9685 & Khử Lệch (Trim/Drift Calibration)**: 2 Servo MG90S 360° được căn chỉnh dải chết (Deadband), bù lệch tốc độ theo đường thẳng và lưu cấu hình vào bộ nhớ NVS flash.
- 🩺 **Hardware Diagnostic & Bench Center (v0.2.1)**:
  - Giao diện Light Mode chuẩn **OpenDesign** (Linear / Vercel style), tối ưu 100% cho mobile.
  - Giám sát nhiệt độ lõi CPU ESP32-S3 (`temperatureRead()`), bảo vệ chống quá nhiệt và chạm chập.
  - Tự động nhận diện nguồn điện ADC (pin 1S 3.4–4.2V hoặc Bypass cắm USB máy tính).
  - Quét bus I2C tự động tìm và nhận dạng module ngoại vi (PCA9685, MPU6050, SSD1306,...).
  - Kịch bản **Self-Test 5s** tự động kiểm tra 6 tiêu chuẩn an toàn phần cứng.

---

## 📐 Sơ Đồ Chân Kết Nối (Hardware Pinout)

Tất cả chân được định nghĩa tập trung tại [`include/pins.h`](include/pins.h) và tuân thủ tuyệt đối vùng an toàn của ESP32-S3 (tránh GPIO 26–37 của Octal PSRAM và GPIO 19/20 của Native USB):

| Hệ thống | Module / Thiết bị | Chân ESP32-S3 | Giao thức / Ghi chú |
| :--- | :--- | :---: | :--- |
| **I2C Bus** | PCA9685 (0x40) | **GPIO 8** (SDA), **GPIO 9** (SCL) | Điều khiển Servo MG90S |
| **SPI TFT 1.54"** | ST7789 (240x240) | **SCLK: 42**, **MOSI: 41**, **DC: 40**, **RST: 39**, **CS: 38**, **BLK: 21** | Giao diện màn hình SPI DMA |
| **I2S0 Input** | Mic INMP441 | **SCK: 4**, **WS: 5**, **SD: 6** | Thu âm kỹ thuật số 16kHz 24-bit |
| **I2S1 Output** | Loa MAX98357A | **DIN: 7**, **LRC: 15**, **BCLK: 16** | Giải mã DAC + Amply Class-D 3.2W |
| **Tachometer / Line**| 2x IR (TCRT5000) | **GPIO 10** (Trái), **GPIO 11** (Phải) | Ngắt phần cứng ISR đếm xung & Dò line |
| **Đèn LED RGB** | WS2812B tích hợp | **GPIO 48** | Đèn báo trạng thái hoạt động |
| **Cảm biến Siêu âm** | HC-SR05 | **GPIO 12** (Trig), **GPIO 13** (Echo) | Đo khoảng cách vật cản |
| **Đo điện áp pin** | Mạch chia áp 0-25V | **GPIO 1** (ADC1) | Tự động bypass khi cắm cáp USB |
| **IMU MPU6050** *(Bản Pet)* | Gia tốc kế 6 trục (0x68) | **GPIO 8** (SDA), **GPIO 9** (SCL) | Chung I2C với PCA9685, 0 pin phụ |
| **Touch Sensor** *(Bản Pet)* | TTP223 / Chạm điện dung | **GPIO 2** | Cảm ứng vuốt ve đầu thú cưng |

---

## 🛠️ Hướng Dẫn Biên Dịch & Nạp Code

Dự án sử dụng PlatformIO. Môi trường mặc định là `rody` (đồng thời duy trì alias `otto` để tương thích ngược, và `rody-pet` cho bản thú cưng).

### 1. Chạy Unit Test trên máy tính (Native):
```bash
# Test bản xe tự hành & firmware (13/13 PASSED):
DEVELOPER_DIR=/Library/Developer/CommandLineTools pio test -e native

# Test bản thú cưng cảm xúc (8/8 PASSED):
DEVELOPER_DIR=/Library/Developer/CommandLineTools pio test -d firmware_pet -e native
```

### 2. Biên dịch Firmware ESP32-S3:
```bash
# Bản xe tự hành v0.2.1:
pio run -e rody

# Bản thú cưng AI Pet v0.3.0:
pio run -e rody-pet
```

### 3. Nạp Firmware vào Robot Rody qua cổng Type-C:
```bash
export RODY_PORT=/dev/ttyUSB0   # macOS: /dev/cu.usbmodem*, Windows: COM5

# Nạp bản xe tự hành:
pio run -e rody -t upload --upload-port $RODY_PORT

# Nạp bản thú cưng cảm xúc:
pio run -e rody-pet -t upload --upload-port $RODY_PORT
```

---

## 🔌 Chế Độ USB Serial Bridge (Preview không cần đổi Wi-Fi)

Khi cắm ESP32 qua cổng USB máy tính, bạn có thể chạy cầu nối Python để điều khiển và chẩn đoán board ngay trên trình duyệt mà không cần chuyển mạng Wi-Fi:

```bash
python3 tools/web_preview_server.py
```
Sau đó mở trình duyệt tại: **`http://localhost:8080`**

---

## 📚 Tài Liệu Hướng Dẫn Kỹ Thuật

- 📖 [**`docs/HUONG_DAN_SU_DUNG.md`**](docs/HUONG_DAN_SU_DUNG.md): Sổ tay hướng dẫn chi tiết từ A-Z (Lắp ráp, nạp code, hiệu chuẩn động cơ, điều khiển Web, khẩu lệnh giọng nói và chẩn đoán sự cố).
- 📜 [**`CHANGELOG.md`**](CHANGELOG.md): Nhật ký thay đổi qua các phiên bản (từ v0.1.0 đến v0.2.1-debug).
- 📄 [`docs/hardware/WIRING.md`](docs/hardware/WIRING.md): Tiêu chuẩn đấu dây đầy đủ v2.0 (BOM chi tiết, bảng chân, phân bổ nguồn).
- 📄 [`docs/hardware/WIRING_MINIMAL.md`](docs/hardware/WIRING_MINIMAL.md): Hướng dẫn lắp ráp siêu tối giản (không tăng áp, không diode, cắm dây chạy ngay).
- 📄 [`docs/hardware/WIRING_PET_S3.md`](docs/hardware/WIRING_PET_S3.md): Hướng dẫn lắp ráp bản Pet thông minh với MPU6050 & TTP223.
- 📄 [`docs/hardware/WIRING_XIAOZHI_TFT.md`](docs/hardware/WIRING_XIAOZHI_TFT.md): Hướng dẫn chi tiết màn hình TFT 1.54" + Loa MAX98357A + Mic INMP441.
- 📋 [`AGENTS.md`](AGENTS.md): Quy tắc vận hành và các chốt an toàn phần cứng (Safety Gates G0 - G2).
