# 📖 Sổ Tay Hướng Dẫn Sử Dụng & Lắp Ráp Robot Rody S3 (Mochi AI Edition)

Chào mừng bạn đến với tài liệu hướng dẫn chính thức dành cho **Robot Rody S3**!  
Tài liệu này cung cấp toàn bộ kiến thức từ chuẩn bị linh kiện, đấu dây, cài đặt môi trường lập trình, nạp firmware, đến các bước hiệu chuẩn và điều khiển robot.

---

## 📑 Mục Lục
1. [Giới Thiệu Tổng Quan Robot Rody S3](#1-giới-thiệu-tổng-quan-robot-rody-s3)
2. [Danh Sách Linh Kiện & Hai Phương Án Lắp Ráp](#2-danh-sách-linh-kiện--hai-phương-án-lắp-ráp)
3. [Sơ Đồ Đấu Dây Chi Tiết (Pinout)](#3-sơ-đồ-đấu-dây-chi-tiết-pinout)
4. [Cài Đặt Môi Trường & Nạp Firmware](#4-cài-đặt-môi-trường--nạp-firmware)
5. [Quy Trình Kiểm Thử Tự Động (Unit Test)](#5-quy-trình-kiểm-thử-tự-động-unit-test)
6. [Quy Trình Hiệu Chuẩn Động Cơ (Motor Calibration)](#6-quy-trình-hiệu-chuẩn-động-cơ-motor-calibration)
7. [Các Cách Thức Điều Khiển & Tương Tác](#7-các-cách-thức-điều-khiển--tương-tác)
8. [Bảng Tra Cứu Lệnh Cổng Nối Tiếp (Serial JSON Console)](#8-bảng-tra-cứu-lệnh-cổng-nối-tiếp-serial-json-console)
9. [Chẩn Đoán & Xử Lý Sự Cố Thường Gặp (FAQ)](#9-chẩn-đoán--xử-lý-sự-cố-thường-gặp-faq)

---

## 1. Giới Thiệu Tổng Quan Robot Rody S3

**Rody S3** là dòng robot mini tự hành 2 bánh vi sai kết hợp vai trò **Trợ lý để bàn thông minh (Desktop AI Pet)**. Robot được trang bị:
- **Vi xử lý trung tâm**: ESP32-S3 DevKitC-1 N16R8 (Dual-core Xtensa 240MHz, 16MB Flash, 8MB Octal PSRAM).
- **Màn hình biểu cảm**: ST7789 TFT 1.54" IPS màu (240x240 pixel), hiển thị 12 trạng thái đôi mắt Mochi hoạt hình với tần số quét 60 FPS.
- **Hệ thống âm thanh hai chiều**:
  - Micro kỹ thuật số I2S INMP441: Thu âm và nhận diện giọng nói tiếng Việt tức thì.
  - Loa Mini kết hợp mạch giải mã kiêm khuếch đại Class-D I2S MAX98357A (3.2W): Phát hiệu ứng âm thanh robot SFX sống động.
- **Hệ thống dẫn động**: 2 Động cơ Servo MG90S 360 độ quay liên tục, được điều khiển qua mạch PWM I2C PCA9685.
- **Cảm biến định vị**: 2 Mắt hồng ngoại IR TCRT5000 đo tốc độ bánh xe (Tachometer) và dò đường theo vạch (Line Tracking).

---

## 2. Danh Sách Linh Kiện & Hai Phương Án Lắp Ráp

Bạn có thể chọn 1 trong 2 phương án tùy theo số lượng linh kiện đang có sẵn:

### 🔹 Phương Án A: Lắp Ráp Siêu Tối Giản (Minimalist Setup)
*Dành cho người mới bắt đầu, chưa có mạch tăng áp, diode hoặc linh kiện phụ trợ. Cắm dây jumper là chạy ngay.*

| # | Linh kiện | Số lượng | Ghi chú kết nối |
|---|-----------|:--------:|-----------------|
| 1 | Bo ESP32-S3 DevKit N16R8 | 1 | Nguồn cấp qua cổng Type-C hoặc chân 5V |
| 2 | Mạch PWM PCA9685 (I2C) | 1 | Địa chỉ mặc định 0x40 |
| 3 | Servo MG90S 360° | 2 | Cắm vào Kênh 0 (Trái) và Kênh 1 (Phải) |
| 4 | Cảm biến hồng ngoại IR (TCRT5000) | 2 | Chân ngõ ra số DO nối vào GPIO 10 & 11 |
| 5 | Mạch sạc TP4056 (có bảo vệ) | 1 | Cấp nguồn pin cho động cơ |
| 6 | Pin Li-ion 1S 3.7V (18650 hoặc Lipo) | 1 | Nối vào cực B+/B- của TP4056 |
| 7 | Dây cắm Breadboard/Jumper | ~12 sợi | Nối dây trực tiếp không cần hàn |

*(Lưu ý: Firmware Rody S3 tự động phát hiện khi chưa lắp mạch chia áp đo pin để bật cờ bypass an toàn, giúp robot vận hành bình thường).*

---

### 🔹 Phương Án B: Lắp Ráp Đầy Đủ (Full Mochi AI Edition)
*Bao gồm toàn bộ tính năng: Màn hình TFT 1.54", Loa, Mic, Cảm biến siêu âm.*

Bổ sung thêm các linh kiện sau vào Phương Án A:
- **Màn hình TFT 1.54" ST7789** (chuẩn giao tiếp SPI 7 chân hoặc 8 chân).
- **Module Microphone I2S INMP441** (6 chân cắm).
- **Module Amply I2S MAX98357A** (5 chân cắm).
- **Loa Mini 4Ω hoặc 8Ω công suất 2W–3W** (nối vào cọc loa của MAX98357A).
- **Module Cảm biến Siêu âm HC-SR05** (kèm mạch chia áp hoặc chuyển mức logic 5V $\to$ 3.3V cho chân Echo).
- **Mạch Tăng Áp MT3608** (chỉnh áp ra đúng 5.1V cấp riêng cho Servo & Amply Loa).

---

### 🔹 Phương Án C: Nâng Cấp Thú Cưng Cảm Xúc (Pet Edition v0.3.0)
*Dành cho bản thú cưng để bàn với các phản xạ xúc giác và quán tính sinh động (thư mục độc lập `firmware_pet/`).*

Bổ sung thêm 2 linh kiện vào Phương Án B:
- **Cảm biến Gia tốc & Con quay hồi chuyển IMU 6 trục (MPU6050)**: Mắc chung bus I2C (GPIO 8 SDA, GPIO 9 SCL) với PCA9685 ở địa chỉ `0x68`, hoàn toàn không tốn thêm chân GPIO phụ.
- **Cảm biến Chạm Vuốt Ve (TTP223 hoặc dây đồng chạm cảm ứng)**: Nối vào **GPIO 2**.

---

## 3. Sơ Đồ Đấu Dây Chi Tiết (Pinout)

Toàn bộ chân GPIO được quy định duy nhất tại [`include/pins.h`](include/pins.h), tuân thủ vùng an toàn bộ nhớ Octal PSRAM của ESP32-S3 (tuyệt đối không chạm vào GPIO 26–37):

```
                        ESP32-S3 DevKitC-1 N16R8
                          +------------------+
                          |   [USB-C OTG]    |
       (Mic SCK)  GPIO 04 | [ ]          [ ] | 3V3  ---> Cấp nguồn cảm biến, màn hình
        (Mic WS)  GPIO 05 | [ ]          [ ] | GND  ---> Nối đất chung (Common GND)
        (Mic SD)  GPIO 06 | [ ]          [ ] | 5V   ---> Nguồn vào 5V
      (Loa DIN)   GPIO 07 | [ ]          [ ] | GPIO 01  (Tùy chọn: Đo pin qua chia áp)
     (PCA9685 SDA) GPIO 08 | [ ]          [ ] | GPIO 02  (Dự phòng)
     (PCA9685 SCL) GPIO 09 | [ ]          [ ] | GPIO 42  (TFT SCLK / SCL)
      (IR Trái)   GPIO 10 | [ ]          [ ] | GPIO 41  (TFT MOSI / SDA)
      (IR Phải)   GPIO 11 | [ ]          [ ] | GPIO 40  (TFT DC / RS)
      (HC-SR05 Trig) GPIO 12| [ ]        [ ] | GPIO 39  (TFT RST / Reset)
      (HC-SR05 Echo) GPIO 13| [ ]        [ ] | GPIO 38  (TFT CS)
                          | [ ]          [ ] | GPIO 21  (TFT BLK / Backlight)
      (Loa LRC)   GPIO 15 | [ ]          [ ] | GPIO 48  (WS2812 RGB tích hợp)
      (Loa BCLK)  GPIO 16 | [ ]          [ ] | GPIO 47  (Dự phòng)
                          |   [USB-C UART]   |
                          +------------------+
```

### Bảng Kết Nối Từng Hệ Thống:

#### 1. Màn Hình TFT 1.54" ST7789 (Giao tiếp SPI DMA)
- **GND** $\to$ GND
- **VCC** $\to$ 3.3V
- **SCL (SCLK)** $\to$ **GPIO 42**
- **SDA (MOSI)** $\to$ **GPIO 41**
- **RES (RST)** $\to$ **GPIO 39**
- **DC** $\to$ **GPIO 40**
- **CS** $\to$ **GPIO 38**
- **BLK (Đèn nền)** $\to$ **GPIO 21**

#### 2. Microphone I2S INMP441 (Thu âm kỹ thuật số)
- **VDD** $\to$ 3.3V
- **GND** $\to$ GND
- **L/R** $\to$ GND (chọn kênh Trái)
- **SCK** $\to$ **GPIO 4**
- **WS** $\to$ **GPIO 5**
- **SD** $\to$ **GPIO 6**

#### 3. Amply Loa I2S MAX98357A (Phát âm thanh Class-D)
- **Vin** $\to$ 5V (hoặc nguồn pin 3.7V)
- **GND** $\to$ GND
- **DIN** $\to$ **GPIO 7**
- **LRC** $\to$ **GPIO 15**
- **BCLK** $\to$ **GPIO 16**
- Hai ngõ ra **+** và **-** nối vào hai dây của Loa Mini.

#### 4. Mạch Điều Khiển Servo PCA9685
- **VCC** (Logic) $\to$ 3.3V từ ESP32-S3
- **GND** $\to$ GND chung
- **SDA** $\to$ **GPIO 8**
- **SCL** $\to$ **GPIO 9**
- **V+** (Cọc vít nguồn động cơ) $\to$ Nối nguồn pin qua mạch TP4056 / MT3608
- **Kênh 0** $\to$ Servo Bánh Trái (Dây Vàng $\to$ PWM, Đỏ $\to$ V+, Nâu $\to$ GND)
- **Kênh 1** $\to$ Servo Bánh Phải

#### 5. Cảm Biến Hồng Ngoại IR (TCRT5000 / LM393)
- **VCC** $\to$ 3.3V
- **GND** $\to$ GND
- **DO Cảm biến Trái** $\to$ **GPIO 10**
- **DO Cảm biến Phải** $\to$ **GPIO 11**

#### 6. Cảm Biến Quán Tính MPU6050 & Chạm Vuốt Ve (Pet Edition v0.3.0)
- **Cảm biến MPU6050 (I2C 0x68)**:
  - **VCC** $\to$ 3.3V, **GND** $\to$ GND
  - **SCL** $\to$ **GPIO 9** (Mắc chung với PCA9685 SCL)
  - **SDA** $\to$ **GPIO 8** (Mắc chung với PCA9685 SDA)
  - **AD0** $\to$ GND (Đặt địa chỉ I2C `0x68`)
- **Cảm biến Chạm TTP223 (hoặc Touch Pad ESP32-S3)**:
  - **VCC** $\to$ 3.3V, **GND** $\to$ GND
  - **SIG (IO)** $\to$ **GPIO 2** (Chân an toàn Touch)

*(Chi tiết xem tài liệu chuyên biệt [`firmware_pet/docs/WIRING_PET.md`](file:///Volumes/Builder/Arduino/OtooRobot/firmware_pet/docs/WIRING_PET.md)).*

---

## 4. Cài Đặt Môi Trường & Nạp Firmware

Dự án phát triển bằng công cụ **PlatformIO** (tương thích cả VSCode Extension và PlatformIO CLI).

### Bước 1: Mở dự án trong PlatformIO
Mở thư mục dự án `OtooRobot` bằng VSCode đã cài extension *PlatformIO IDE*.

### Bước 2: Biên dịch Firmware
Mở terminal trong VSCode hoặc terminal hệ điều hành:
```bash
# Biên dịch firmware bản xe tự hành (v0.2.0-rody)
pio run -e rody

# Hoặc biên dịch firmware bản thú cưng cảm xúc (v0.3.0-pet)
pio run -e rody-pet
```
*(Nếu muốn chạy qua alias cũ `otto`, bạn vẫn có thể dùng `pio run -e otto`).*

### Bước 3: Nạp Firmware vào Robot
1. Cắm cáp USB-C từ máy tính vào cổng **UART** hoặc **OTG** trên ESP32-S3.
2. Xác định cổng Serial (ví dụ: `/dev/ttyUSB0` trên Linux, `/dev/cu.wchusbserial*` hoặc `/dev/cu.usbmodem*` trên macOS, hoặc `COM3`/`COM5` trên Windows).
3. Thiết lập biến môi trường và nạp:
```bash
# Trên macOS / Linux:
export RODY_PORT=/dev/ttyUSB0

# Nạp bản xe tự hành v0.2.0:
pio run -e rody -t upload --upload-port $RODY_PORT

# Hoặc nạp bản thú cưng AI Pet v0.3.0:
pio run -e rody-pet -t upload --upload-port $RODY_PORT

# Trên Windows (PowerShell):
$env:RODY_PORT="COM5"
pio run -e rody -t upload --upload-port $env:RODY_PORT
```

---

## 5. Quy Trình Kiểm Thử Tự Động (Unit Test)

Trước khi lắp đặt phần cứng hoặc sau mỗi lần sửa đổi thuật toán, bạn hãy chạy bộ kiểm thử Unit Test không phụ thuộc phần cứng (Native Unit Test):

```bash
# Chạy toàn bộ 13 bài test thuật toán toán học, calib, RMS, và nhận diện lệnh giọng nói:
DEVELOPER_DIR=/Library/Developer/CommandLineTools pio test -e native
```
Kết quả mong đợi: **13/13 test cases PASSED** (thời gian chạy ~1-2 giây).

---

## 6. Quy Trình Hiệu Chuẩn Động Cơ (Motor Calibration)

Động cơ Servo 360° là loại analog biến trở nên điểm dừng và tốc độ quay giữa 2 bánh có thể bị lệch. Rody S3 cung cấp quy trình hiệu chuẩn 3 bước thông minh lưu thẳng vào flash NVS:

### Bước 1: Tìm dải chết (Deadband Calibration)
Kê bánh xe robot lên khỏi mặt bàn. Gửi lệnh qua Serial:
```
cal deadband l
cal deadband r
```
Robot sẽ quét độ rộng xung để xác định chính xác dải xung mà động cơ đứng yên hoàn toàn (thường quanh mức 1500µs).

### Bước 2: Cân bằng tốc độ 2 bánh (Trim Balance)
Cho robot chạy thử một quãng:
```
meas 50
```
Robot sẽ đo tốc độ xung phản hồi từ 2 đĩa sọc hồng ngoại và tự động tính toán tỷ lệ bù trừ (Trim) để 2 bánh quay đều nhau tuyệt đối.

### Bước 3: Khử lệch đường thẳng (Drift Calibration)
Đặt robot trên sàn phẳng có dán vạch thẳng dài 100cm. Cho robot chạy thử:
```
drive 60 60 3000
```
Đo độ lệch ngang $d$ (cm) và quãng đường thực tế $D$ (cm). Gửi lệnh:
```
cal drift <d> <D>
cal save
```
Robot sẽ lưu toàn bộ thông số vào NVS (`store.cpp`). Từ nay về sau, robot luôn chạy thẳng băng tắp!

---

## 7. Các Cách Thức Điều Khiển & Tương Tác

### Cách 1: Điều Khiển Qua Giao Diện Giả Lập Web (Web Simulator Mobile-First)
Mở file [`docs/robot_simulator.html`](file:///Volumes/Builder/Arduino/OtooRobot/docs/robot_simulator.html) bằng trình duyệt web:
- **Trên máy tính**: Bấm đúp vào file hoặc gõ `open docs/robot_simulator.html`.
- **Trên điện thoại**: Chạy lệnh `python3 -m http.server 8080`, sau đó lấy điện thoại kết nối chung Wi-Fi và truy cập `http://<IP_MÁY_TÍNH>:8080/docs/robot_simulator.html`.

Tại giao diện Web Simulator, bạn có thể:
1. **Tab Biểu Cảm**: Bấm chọn 12 trạng thái cảm xúc Mochi để xem đôi mắt nháy động 60 FPS và bấm phát các hiệu ứng âm thanh robot SFX.
2. **Tab Lái Xe**: Nhìn sa bàn 2D mô phỏng và dùng bàn phím Gamepad D-Pad ngón tay cái chạm giữ để lái xe liên tục, buông tay để dừng. Có rung phản hồi (Haptic).
3. **Tab Giọng Nói**: Bật micro hoặc chạm nhanh vào các thẻ lệnh thoại tiếng Việt để ra lệnh cho robot.
4. **Tab Thông Số**: Xem cấu hình chân ESP32-S3 và màn hình Serial Monitor truyền nhận JSON.

---

### Cách 2: Điều Khiển Trực Tiếp Qua Wi-Fi Nội Bộ (SoftAP)
Khi cấp nguồn, Rody S3 sẽ tự phát mạng Wi-Fi riêng:
- **Tên Wi-Fi (SSID)**: `Rody-S3` (không mật khẩu).
- Dùng điện thoại kết nối vào Wi-Fi `Rody-S3`.
- Mở trình duyệt và truy cập địa chỉ IP: **`http://192.168.4.1`**.
- Màn hình điều khiển cảm ứng với các phím mũi tên và nút chọn chế độ (Thủ công / Tránh vật cản / Dò line) sẽ xuất hiện tức thì!

---

### Cách 3: Ra Lệnh Bằng Giọng Nói Tiếng Việt (Mic INMP441)
Khi robot đang ở trạng thái lắng nghe (`LISTENING`), bạn có thể nói vào Micro:
- *"Rody ơi"* hoặc *"Chào"* $\to$ Mắt cười hình trăng khuyết (`HAPPY`) + hót tiếng chim R2-D2.
- *"Tiến lên"* $\to$ Mắt mở to tiến tới (`DRIVE_FWD`) + chạy tới phía trước.
- *"Lùi lại"* $\to$ Mắt quan sát lùi (`DRIVE_REV`) + lùi xe.
- *"Rẽ trái"* $\to$ Mắt liếc trái (`TURN_LEFT`) + quay bánh cua trái.
- *"Rẽ phải"* $\to$ Mắt liếc phải (`TURN_RIGHT`) + quay bánh cua phải.
- *"Dừng lại"* $\to$ Dừng động cơ ngay lập tức + mắt về trạng thái quan sát (`IDLE`).
### Cách 4: Tương Tác Thú Cưng Động Cảm Xúc (Pet Edition v0.3.0)
Khi nạp firmware `firmware_pet/` (hoặc mở file giả lập [`docs/pet_simulator.html`](file:///Volumes/Builder/Arduino/OtooRobot/docs/pet_simulator.html)), Rody sẽ trở thành một chú thú cưng để bàn với 6 phản xạ sinh động:
1. **Được vuốt ve đầu (Petting)**: Dùng tay vuốt ve vùng cảm biến chạm trên đầu (GPIO 2) $\to$ Rody phát tiếng gừ gừ (purr), mắt cười trăng khuyết kèm tim hồng đập, tăng điểm tình cảm.
2. **Bị lật ngửa bụng (Belly Up)**: Khi bị lật ngửa ($A_z < -0.45g$) $\to$ Mắt đảo liên hồi, còi réo liên tục, hai bánh xe quẫy loạn xạ để cầu cứu. Đặt úp lại để trấn an.
3. **Bị lắc liên tục (Shaken)**: Cầm robot lắc qua lắc lại $\to$ Mắt xoắn ốc chóng mặt (dizzy). Nếu lắc quá lâu, robot sẽ cáu gắt đỏ mặt gầm gừ.
4. **Bị té / rơi (Fallen / Tripped)**: Robot bị trượt ngã hoặc va đập mạnh $\to$ Mắt rủ xuống ngấn lệ rơi, phát tiếng khóc thút thít xót xa.
5. **Bị gõ mạnh vào thân (Knocked)**: Gõ ngón tay vào thân robot $\to$ Mắt giật mình mở to, kêu thảng thốt và lùi lại 2cm để phòng thủ.
6. **Âm thanh xung quanh quá ồn ào (Loud Ambient Noise)**: Mic thu được tiếng ồn $>70\%$ liên tục 2.5s $\to$ Rody nhăn nhó cụp tai khó chịu và phát tiếng thở dài.
7. **Vỗ tay nhảy Disco (Clap-Clap Dance)**: Vỗ tay 2 cái liên tiếp vào nhịp $\to$ Mắt kính râm neon nhấp nháy, phát beat funky 8-bit disco, bánh xe nhảy Moonwalk lùi và lắc hông theo beat!
8. **Thổi gió hắt xì (Achoo! 🤧)**: Thổi luồng gió vào mic $\to$ Rody run rẩy hít sâu rồi hắt xì nổ tung, bánh xe giật lùi 5cm.
9. **Nhấc bay lượn máy bay (Airplane Mode)**: Nhấc bổng lượn sóng trên không $\to$ Mắt kính phi công rẽ mây, loa phát động cơ rền vang, hai bánh quay tít như cánh quạt.
10. **Cù lét nhột (Tickle Monster 😆)**: Chạm nhanh 3+ lần liên tiếp (multi-tap < 600ms) $\to$ Mắt híp tịt `> <`, hai má ửng hồng phúng phính, cười khúc khích, bánh rung lắc nhột ngặt nghẽo.
11. **Trò chơi đập tay (High-Five Challenge ✋)**: Rody giơ bàn tay vàng mời gọi $\to$ nếu người dùng chạm tay trong 1.2s $\to$ Kèn đồng chiến thắng, pháo hoa bắn rực rỡ và xoay vòng 360 độ ăn mừng!
12. **Húc tay đòi nịnh (The Gentle Nudge 🥺)**: Khi bị bỏ rơi lâu $\to$ Mắt cún long lanh to tròn, phát tiếng kêu nũng nịu, khẽ bò lại gần húc nhẹ vào tay chủ.
13. **Chuyển đổi 3 Lốt tính cách (Personas)**: 🐱 **Mèo Lười** (thích vuốt ve), 🐶 **Cún Cưng** (tăng động, thích đập tay), 🤖 **Mecha Robot** (quét radar, kính đen Thug Life).

---

## 8. Bảng Tra Cứu Lệnh Cổng Nối Tiếp (Serial JSON Console)

Kết nối cổng Serial tốc độ **115200 Baud**. Mỗi lệnh gửi cách nhau bằng dấu xuống dòng (`\n`). Robot sẽ phản hồi kết quả dạng JSON:

### Bảng Lệnh Chung (Bản Xe Tự Hành & Thú Cưng):
| Lệnh | Ý nghĩa | Ví dụ dữ liệu phản hồi |
| :--- | :--- | :--- |
| `ping` | Kiểm tra kết nối | `{"cmd":"ping","ok":true,"fw":"0.2.0-rody"}` |
| `info` | Đọc thông số bộ nhớ, uptime | `{"cmd":"info","ok":true,"flash":16777216,"psram":8388608,"heap":298400}` |
| `bat` | Đọc điện áp pin hiện tại | `{"cmd":"bat","ok":true,"v":3.92}` |
| `drive <L> <R> <ms>` | Lái 2 bánh tốc độ L, R trong ms | `{"cmd":"drive","ok":true,"l":50.0,"r":50.0,"ms":1000}` |
| `stop` | Dừng khẩn cấp toàn bộ động cơ | `{"cmd":"stop","ok":true}` |
| `mode <manual\|avoid\|line>` | Đổi chế độ hoạt động | `{"cmd":"mode","ok":true,"mode":"avoid"}` |
| `face <tên_biểu_cảm>` | Đổi cảm xúc màn hình ST7789 | `{"cmd":"face","ok":true,"emotion":"happy"}` |
| `sfx <loại_âm_thanh>` | Phát hiệu ứng âm thanh ra loa | `{"cmd":"sfx","ok":true,"played":"boot"}` |
| `voice <lệnh>` | Kích hoạt giả lập khẩu lệnh thoại | `{"cmd":"voice","ok":true,"cmd_name":"tien"}` |
| `us <số_lần>` | Đọc cảm biến khoảng cách siêu âm | `{"cmd":"us","ok":true,"cm":[24.5,24.6,24.5]}` |
| `ir` | Đọc trạng thái 2 mắt dò line | `{"cmd":"ir","ok":true,"l":0,"r":0}` |
| `cal show` | Xem bảng thông số hiệu chuẩn NVS | `{"cmd":"cal show","ok":true,"ver":1,"valid":true}` |

### Bảng Lệnh Bổ Sung Cho Bản Thú Cưng (Pet Edition v0.3.0):
| Lệnh | Ý nghĩa | Mô tả phản xạ thú cưng |
| :--- | :--- | :--- |
| `pet` | Kích hoạt sự kiện vuốt ve đầu | Kêu rừ rừ, tim đập, tăng quấn chủ |
| `belly_up` | Kích hoạt sự kiện lật ngửa bụng | Báo động còi réo, bánh quẫy cầu cứu |
| `shake` | Kích hoạt sự kiện lắc liên tục | Mắt xoắn ốc chóng mặt rồi cáu gắt |
| `fall` | Kích hoạt sự kiện robot bị té ngã | Mắt ngấn lệ khóc, tăng điểm stress |
| `knock` | Kích hoạt sự kiện bị gõ mạnh | Giật mình, thảng thốt, lùi bánh |
| `noise <0-100>` | Giả lập mức tiếng ồn môi trường | Quá 70% trong 2.5s sẽ nhăn nhó thở dài |
| `disco` | Kích hoạt nhảy Disco vỗ tay | Kính neon, beat 8-bit, Moonwalk |
| `achoo` | Kích hoạt phản xạ hắt xì hơi | Rùng mình, hắt xì cực mạnh, lùi 5cm |
| `fly` | Kích hoạt chế độ máy bay | Kính phi công rẽ gió, cánh quạt quay |
| `tickle` | Kích hoạt cù lét nhột | Mắt > < má hồng, cười khúc khích |
| `highfive` | Bắt đầu thử thách đập tay | Chờ chạm trong 1.2s để bắn pháo hoa |
| `nudge` | Kích hoạt hành động húc nịnh | Mắt long lanh, khẽ bò lại gần cọ tay |
| `cool` | Kính đen Thug Life | Kính râm pixel, nhạc hip-hop |
| `cat` / `dog` / `mecha` | Đổi lốt tính cách (Persona) | Thay đổi thái độ và phong cách |
| `stats` | Xem chỉ số tâm trạng (Mood HUD) | Trả về điểm Affection, Stress, Energy |

*Danh sách biểu cảm (`face`): `idle`, `happy`, `listening`, `thinking`, `speaking`, `fwd`, `rev`, `left`, `right`, `dizzy`, `obstacle`, `sleepy`, `disco`, `sneeze`, `airplane`, `tickle`, `high_five`, `nudge`, `cool_glasses`.*  
*Danh sách âm thanh (`sfx`): `boot`, `beep`, `happy`, `listen`, `think`, `ack`, `obstacle`, `error`, `disco`, `sneeze`, `airplane`, `tickle`, `fanfare`, `nudge`, `thug_life`.*

---

## 9. Chẩn Đoán & Xử Lý Sự Cố Thường Gặp (FAQ)

### ❓ 1. Động cơ tự động quay ngay khi vừa bật nguồn dù chưa bấm nút gì?
- **Nguyên nhân**: Điểm dải chết (Deadband) của servo chưa được hiệu chuẩn nên xung mặc định 1500µs làm servo quay nhẹ.
- **Khắc phục**: Chạy lệnh `cal deadband l` và `cal deadband r` rồi gõ `cal save` để lưu điểm dừng vào flash.

### ❓ 2. Màn hình TFT 1.54" ST7789 không sáng đèn hoặc chỉ sáng trắng?
- **Nguyên nhân**: Dây chân BLK (Backlight) chưa nối lên 3.3V/GPIO 21, hoặc cắm nhầm chân SCL/SDA với chuẩn I2C.
- **Khắc phục**: Màn hình ST7789 này chạy giao thức **SPI**. Kiểm tra kỹ: SCL phải cắm vào **GPIO 42**, SDA cắm vào **GPIO 41**, BLK cắm vào **GPIO 21** hoặc nối thẳng lên chân 3.3V.

### ❓ 3. Robot tự khởi động lại (Reset) khi bấm cho 2 động cơ chạy?
- **Nguyên nhân**: Sụt áp nguồn (Brownout Reset) do dùng pin yếu hoặc cấp nguồn động cơ chung với chân 3.3V của ESP32-S3.
- **Khắc phục**:
  - Tuyệt đối không lấy nguồn 3.3V của ESP32-S3 cấp cho Servo. Cọc V+ của PCA9685 phải nối trực tiếp vào pin qua mạch TP4056 hoặc mạch tăng áp MT3608 (5V).
  - Nối cực âm GND của tất cả các mạch (ESP32-S3, PCA9685, Pin) lại với nhau (Common Ground).

### ❓ 4. Loa phát ra tiếng rè hoặc không nghe thấy âm thanh?
- **Nguyên nhân**: Chân DIN/LRC/BCLK của MAX98357A cắm lỏng hoặc chạm vào chân khác.
- **Khắc phục**: Kiểm tra đúng thứ tự: DIN $\to$ **GPIO 7**, LRC $\to$ **GPIO 15**, BCLK $\to$ **GPIO 16**. Đảm bảo mạch MAX98357A được cấp nguồn 5V sạch.

### ❓ 5. Không nhận diện được giọng nói từ Microphone INMP441?
- **Nguyên nhân**: Chân L/R chưa được nối xuống đất GND khiến mic không định tuyến đúng kênh I2S Mono (kênh Trái).
- **Khắc phục**: Hàn hoặc nối một sợi dây từ chân **L/R của INMP441 xuống chân GND**.
