# 📜 Nhật Ký Thay Đổi (Changelog) – Robot Rody S3

Tất cả các thay đổi đáng chú ý của dự án **Robot Rody S3** được ghi nhận chi tiết tại tài liệu này.  
Định dạng dựa trên [Keep a Changelog](https://keepachangelog.com/vi/1.0.0/), và tuân thủ [Semantic Versioning](https://semver.org/lang/vi/).

---

## [0.2.1-debug] - 2026-10-03

### 🩺 Trung Tâm Đo Kiểm Phần Cứng & Giao Diện OpenDesign Light Mode
- **Giao diện chẩn đoán Web hoàn toàn mới (Light Mode & Lean)**:
  - Thiết kế lại từ đầu theo hệ thống **OpenDesign** (Linear / Vercel style) đạt chuẩn 0 lỗi `od lint` và 0 lỗi `ux_inspect.py`.
  - Bố cục Segmented Control (iOS style) 4 tab mượt mà: `Lái Xe`, `Đo Kiểm`, `Kịch Bản`, `Logs`.
  - Triệt tiêu hoàn toàn emoji và gradient tối, thay bằng bộ icon monoline SVG sắc nét (1.8px stroke).
  - Sử dụng font số `tabular-nums` triệt tiêu giật khung số liệu khi cập nhật thời gian thực mỗi 1.5s.
- **Giám sát Sức khỏe Vi điều khiển & An toàn Phần cứng**:
  - Đo nhiệt độ lõi CPU ESP32-S3 tức thời qua `temperatureRead()`, cảnh báo khi nhiệt độ >60°C.
  - Tự động nhận diện nguồn điện ADC: chế độ pin 1S (3.4V–4.2V) hoặc chế độ Bypass an toàn khi cắm cáp USB máy tính.
  - Quét bus I2C tự động dò tìm và nhận dạng tên linh kiện (`0x40`: PCA9685, `0x68`: MPU6050, `0x3C`: SSD1306).
  - Nhích thử từng động cơ servo có Watchdog tự ngắt xung PWM sau 0.5s–1s để bảo vệ động cơ.
  - Thước đo mắt hồng ngoại IR trực quan thời gian thực.
  - Kịch bản **Self-Test 5s** tự động kiểm tra tuần tự 6 tiêu chuẩn an toàn phần cứng.
- **Cầu nối USB Serial Bridge (`tools/web_preview_server.py`)**:
  - Cho phép người dùng cắm USB máy tính là điều khiển và chẩn đoán board ngay trên `http://localhost:8080` mà không cần ngắt mạng hay đổi Wi-Fi.

---

## [0.3.0-pet] - 2026-10-03

### 🌟 Nâng Cấp Cảm Biến Xúc Giác & Gia Tốc Thú Cưng (Pet Sensory & Emotion Edition)
- **Tách biệt hoàn toàn (Full Isolation) trong thư mục `firmware_pet/`**:
  - Không ảnh hưởng và giữ nguyên 100% mã nguồn phiên bản robot tự hành hiện tại (`v0.2.0-rody` trong `src/`).
  - Môi trường biên dịch chuyên biệt `[env:rody-pet]` trong root `platformio.ini` và `firmware_pet/platformio.ini`.
- **Cảm biến Gia tốc kế & Con quay hồi chuyển IMU (MPU6050)**:
  - Chia sẻ bus I2C (GPIO 8 SDA, GPIO 9 SCL) với PCA9685 ở địa chỉ `0x68` (tiết kiệm chân, hoàn toàn không tốn chân phụ).
  - Driver thanh ghi nhẹ, đo gia tốc 3 trục $A_x, A_y, A_z$ và vận tốc góc $G_x, G_y, G_z$ ở tần số 50Hz.
  - Tự động nhận diện 4 trạng thái quán tính: Rơi tự do & va đập (`fall`), Bị lật ngửa bụng (`belly_up` khi $A_z < -0.45g$), Bị lắc liên tục (`shake` qua dao động gyro), Gõ mạnh vào thân (`knock` qua xung gia tốc $Z$).
- **Cảm biến Chạm Vuốt Ve (Capacitive Touch Sensor)**:
  - Sử dụng chân an toàn **GPIO 2**, tương thích cả cảm biến cảm ứng rời TTP223 và Touch pad nội tại của ESP32-S3.
  - Thuật toán chống rung (debounce) phân biệt chạm nhanh (Tap $50-280\text{ms}$) và vuốt ve liên tục (Petting $>300\text{ms}$).
- **Giám sát Tiếng ồn Môi trường (Acoustic Monitor qua I2S0 INMP441)**:
  - Đo mức năng lượng âm thanh RMS liên tục ở 16kHz; phát hiện khi môi trường quá ồn ào ($>70\%$ trong $>2.5\text{s}$).
- **Hệ thống 6 Phản xạ Cảm xúc Thú Cưng (Pet Emotional State Machine & Brain)**:
  1. *Được vuốt ve đầu (Petted)*: Rody phát tiếng rừ rừ (purr 90–120Hz), mắt cười trăng khuyết + tim hồng đập, tăng điểm tình cảm (Affection).
  2. *Bị té / rơi (Tripped / Fallen)*: Mắt rủ xuống ngấn lệ rơi, khóc thút thít, tăng căng thẳng (Stress).
  3. *Bị lật ngửa bụng (Belly Up)*: Mắt đảo liên hồi báo động, còi cấp cứu réo nhịp nhanh, hai bánh xe quẫy liên tục cầu cứu.
  4. *Bị lắc liên tục (Shaken)*: Mắt xoắn ốc chóng mặt (dizzy), nếu lắc lâu chuyển sang gắt gỏng đỏ mặt (angry growl).
  5. *Bị gõ mạnh vào thân (Strong Knock)*: Giật mình đồng tử co giãn, kêu thảng thốt, lùi nhẹ phòng thủ.
  6. *Môi trường quá ồn ào (Loud Ambient Noise)*: Mắt nhăn nhó cụp tai khó chịu, phát tiếng thở dài (sigh).
- **Hệ thống Phản xạ & Tương Tác Sáng Tạo Mới (Creative Behaviors & Personas)**:
  1. *Vỗ tay Disco (Clap-Clap Dance)*: Mic INMP441 nhận diện nhịp vỗ tay đôi (160–550ms) $\to$ Mắt kính râm neon nhấp nháy, phát beat funky 8-bit disco chiptune, bánh xe nhảy Moonwalk lùi và lắc hông theo nhịp.
  2. *Thổi gió hắt xì (Achoo! 🤧)*: Nhận diện luồng gió thổi vào mic $\to$ Mắt co giật run rẩy, tiếng hít sâu rồi bùng nổ hắt xì cực mạnh, bánh xe giật lùi 5cm.
  3. *Nhấc bay lượn máy bay (Airplane Flight Mode)*: Cảm biến IMU nhận diện trạng thái nhấc bổng lượn sóng trên không $\to$ Mắt biến thành kính phi công cổ điển kèm vệt gió rẽ mây, loa phát tiếng động cơ rền vang, hai bánh quay tít như cánh quạt.
  4. *Cù lét nhột (Tickle Monster 😆)*: Chạm ngón tay nhanh 3+ lần liên tiếp (multi-tap < 600ms) $\to$ Mắt híp tịt `> <`, hai má ửng hồng phúng phính, phát tiếng cười khúc khích chiptune, bánh xe rung lắc nhột ngặt nghẽo.
  5. *Trò chơi đập tay (High-Five Challenge ✋)*: Rody giơ bàn tay vàng mời gọi $\to$ nếu người dùng chạm tay trong 1.2s $\to$ Khúc khải hoàn kèn đồng vang dội, pháo hoa bắn rực rỡ và quay vòng 360 độ ăn mừng!
  6. *Húc tay đòi nịnh (The Gentle Nudge 🥺)*: Khi bị bỏ rơi lâu $\to$ Mắt cún long lanh to tròn, phát tiếng kêu nũng nịu, khẽ bò lại gần húc nhẹ vào tay chủ.
  7. *Chuyển đổi 3 Lốt tính cách (Personas)*: 🐱 **Mèo Lười** (đỏng đảnh, thích vuốt ve), 🐶 **Cún Cưng** (tăng động, thích đập tay), 🤖 **Mecha Robot** (quét radar, kính đen Thug Life).
- **Bộ Giả Lập Trải Nghiệm Thú Cưng Web Simulator (`docs/pet_simulator.html`)**:
  - Giao diện Mobile Native App theo chuẩn SaaS Design Master & Krug Usability, đạt 0 lỗi kiểm toán UX trên Desktop, Tablet và Mobile.
  - Màn hình Canvas Mochi 240x240 60 FPS, live HUD 3 chỉ số Mood (Affection, Stress, Energy), chuyển đổi 3 Persona, lưới 6 nút hành vi sáng tạo và tổng hợp âm thanh Web Audio procedural không cần file mp3 ngoài.
- **Tài liệu đấu nối & Kiểm thử chuyên biệt**:
  - `firmware_pet/docs/WIRING_PET.md`: Hướng dẫn đấu nối MPU6050 và Touch sensor.
  - `firmware_pet/test/test_pet_brain/test_main.cpp`: 8/8 test cases kiểm thử logic não và phản xạ thú cưng PASSED 100%.

---

## [0.2.0-rody] - 2026-10-03

### 🌟 Tính Năng Mới Nổi Bật (Major Features)
- **Đổi tên chính thức thành Robot Rody S3**:
  - Chuyển đổi toàn diện tên gọi, thông điệp khởi động, giao diện Web SoftAP (`Rody-S3`), cấu hình NVS namespace (`"rody"` với cơ chế tự động fallback `"otto"` nếu đã lưu trước đó), và bộ công cụ kiểm thử.
  - Cập nhật file cấu hình [`platformio.ini`](platformio.ini) với môi trường mặc định `[env:rody]`, đồng thời duy trì alias `[env:otto]` (`extends = env:rody`) để đảm bảo khả năng tương thích ngược 100%.
- **Màn hình biểu cảm đôi mắt Mochi / Xiaozhi (TFT 1.54" ST7789 IPS 240x240)**:
  - Tích hợp driver đồ họa phần cứng qua LovyanGFX với bộ nhớ đệm kép (Double-buffered 16-bit Sprite), đạt tốc độ khung hình **60 FPS** mượt mà, triệt tiêu hoàn toàn hiện tượng nhấp nháy (flicker-free).
  - Thiết kế 12 trạng thái cảm xúc động: `IDLE` (chớp mắt ngẫu nhiên), `HAPPY` (mắt cười trăng khuyết), `LISTENING` (đồng tử mở rộng), `THINKING` (mắt đảo lơ đãng), `SPEAKING` (mắt nhấp nhô theo giọng), `DRIVE_FWD` (tập trung tiến tới), `DRIVE_REV` (mắt lùi quan sát), `TURN_LEFT` / `TURN_RIGHT` (liếc góc cua), `DIZZY` (hoa mắt quay vòng), `OBSTACLE` (mắt mở to báo động), `SLEEPY` (mắt lim dim chìm vào giấc ngủ).
- **Hệ thống âm thanh I2S hai chiều (Mic INMP441 + Loa MAX98357A)**:
  - **Phát âm thanh Loa I2S1 (MAX98357A)**: Tích hợp bộ tổng hợp âm thanh kỹ thuật số procedural tạo ra 8 hiệu ứng SFX robot độc đáo: Âm thanh khởi động (Boot chime), Tiếng bíp xác nhận (Confirm beep), Tiếng hót chim R2-D2 (Happy chirp), Chuông bắt đầu lắng nghe (Listen start), Xung nhịp suy nghĩ (Thinking pulse), Lệnh hoàn thành (Ack), Còi báo vật cản nguy hiểm (Obstacle siren), và Tiếng lỗi cảnh báo (Error buzz).
  - **Thu âm Micro I2S0 (INMP441)**: Thu âm thời gian thực tần số 16kHz, xử lý mẫu 24-bit/32-bit slot, tính toán năng lượng âm thanh RMS chuyển đổi sang thang đo 0–100%.
  - **Nhận diện giọng nói tiếng Việt**: Nhận diện tức thời các khẩu lệnh: `"rody"` / `"chao"` (Chào robot Rody), `"tien"` (Tiến lên), `"lui"` (Lùi lại), `"trai"` (Rẽ trái), `"phai"` (Rẽ phải), `"dung"` (Dừng lại), `"quay"` (Quay vòng), `"vui"` (Cảm xúc vui vẻ).
- **Bộ Giả Lập Trải Nghiệm Robot Web Simulator Mobile-First (`docs/robot_simulator.html`)**:
  - Tái thiết kế toàn bộ theo phong cách **Mobile Native App** với thanh điều hướng đáy cố định (Sticky Bottom Navigation Bar - Fitts's Law) gồm 4 Tab:
    1. 🖥️ **Biểu Cảm**: Màn hình ST7789 240x240 render trực tiếp qua HTML5 Canvas, nháy mắt liên tục, lưới 12 biểu cảm và 6 nút âm thanh SFX.
    2. 🎮 **Lái Xe**: Sa bàn đường đua 2D giả lập kèm cụm Gamepad D-Pad đặt ngay tầm với ngón tay cái; hỗ trợ **chạm giữ liên tục (Touch-Hold)** và **Rung phản hồi xúc giác (Haptic Feedback)**.
    3. 🎙️ **Giọng Nói**: Nút Micro to rõ, thanh đo năng lượng âm thanh RMS live, và 8 thẻ chạm nhanh mô phỏng lệnh thoại.
    4. 📟 **Thông Số**: Bảng phần cứng ESP32-S3 và nhật ký nối tiếp Serial Console streaming JSON thời gian thực.
  - Vượt qua kiểm toán trải nghiệm tự động (SaaS Visual UX Audit) với **0 lỗi** trên cả 3 kích thước: Desktop (1440x900), Tablet (768x1024), và Mobile (375x812).
- **Sơ đồ đấu nối tương tác 2D (`docs/hardware/wiring_interactive.html`)**:
  - Giao diện trực quan mô phỏng đầy đủ bo mạch ESP32-S3, ST7789, INMP441, MAX98357A, PCA9685, 2 Servo, 2 Cảm biến IR, TP4056 và MT3608.
  - Hỗ trợ thao tác cảm ứng phóng to/thu nhỏ, kéo thả bản vẽ, lọc dây theo phân hệ, xem thông số chân và từ khóa mua linh kiện trên Shopee.

### ⚡ Cải Tiến & Tối Ưu Hóa (Enhancements)
- **Tái cấu trúc sơ đồ chân an toàn (Pin Mapping Architecture)**:
  - Dành riêng cổng SPI cho màn hình: SCLK (42), MOSI (41), DC (40), RST (39), CS (38), BLK (21).
  - Dành riêng cổng I2S0 cho Mic: SCK (4), WS (5), SD (6).
  - Dành riêng cổng I2S1 cho Loa: DIN (7), LRC (15), BCLK (16).
  - Dời 2 chân cảm biến hồng ngoại IR từ GPIO 6, 7 sang **GPIO 10 & 11** để tránh xung đột chân I2S mà vẫn giữ nguyên tính năng ngắt cạnh phần cứng (Hardware Interrupt ISR) phục vụ đếm xung vòng quay.
- **Tập lệnh Serial Console mở rộng**:
  - Bổ sung lệnh `face <tên>`: Đổi trực tiếp biểu cảm khuôn mặt qua cổng Serial.
  - Bổ sung lệnh `sfx <loại>`: Phát tức thì hiệu ứng âm thanh ra loa.
  - Bổ sung lệnh `voice <lệnh>`: Kích hoạt giả lập khẩu lệnh giọng nói để kiểm thử tự động.

### 🧪 Kiểm Thử (Testing)
- Bổ sung bộ Unit Test cho firmware (`test/test_firmware/test_main.cpp`):
  - Kiểm thử tự động bypass pin khi không gắn module đo áp.
  - Kiểm thử logic đọc pin khi kết nối mạch chia áp.
  - Kiểm thử bộ ánh xạ tên biểu cảm cảm xúc `emotion_gfx`.
  - Kiểm thử phân tích từ khóa giọng nói tiếng Việt & tiếng Anh (bao gồm từ khóa `"rody"`).
  - Kiểm thử thuật toán chuẩn hóa biên độ năng lượng âm thanh RMS.
- Tỷ lệ vượt qua kiểm thử: **13/13 test cases PASSED** trên môi trường native PC.

---

## [0.1.5] - 2026-10-02

### 🌟 Tính Năng Mới
- **Hướng dẫn lắp ráp siêu tối giản (`docs/hardware/WIRING_MINIMAL.md`)**:
  - Lược bỏ mạch tăng áp MT3608, tụ lọc, diode và module chia áp đo pin.
  - Rút ngắn thời gian lắp ráp, chỉ cần ~10 sợi dây jumper cắm trực tiếp từ pin 1S (3.7V) vào PCA9685 và ESP32-S3.
- **Cơ chế tự phát hiện phần cứng thông minh (Hardware Auto-Detection)**:
  - Khi điện áp đọc tại chân ADC1 (GPIO 1) nhỏ hơn 1.0V, firmware tự động hiểu người dùng không gắn module đo áp $\to$ Bật cờ bypass an toàn, không ngắt nguồn servo hay cảnh báo pin yếu ảo.

---

## [0.1.0] - 2026-09-30

### 🚀 Phiên Bản Khởi Tạo Ban Đầu (Initial Release)
- **Kiến trúc Robot Vi Sai 2 Bánh Dùng ESP32-S3 N16R8**:
  - Giao tiếp điều khiển động cơ qua I2C với IC PCA9685 (địa chỉ 0x40), điều khiển xung PWM cho 2 Servo liên tục MG90S 360 độ.
  - Cảm biến khoảng cách siêu âm HC-SR05 đo khoảng cách vật cản từ 2cm đến 400cm.
  - 2 Cảm biến hồng ngoại IR dò vạch đen và đĩa sọc đo tốc độ bánh xe (Tachometer).
- **Thư viện hiệu chuẩn toán học thuần túy (`lib/otto_calib`)**:
  - Thuật toán đo dải chết (Deadband Calibration) tìm xung dừng chính xác của từng động cơ.
  - Thuật toán cân bằng tốc độ (Trim Balance) giúp robot chạy thẳng đều.
  - Thuật toán bù góc lệch (Drift Calibration) triệt tiêu độ lệch ngang sau quãng đường dài.
  - Lưu trữ dữ liệu calib vào bộ nhớ NVS Flash, có mã kiểm tra tính toàn vẹn CRC-32 IEEE 802.3.
- **Giao diện điều khiển Web SoftAP**:
  - Phát Wi-Fi nội bộ không cần router, trang điều khiển web trực tiếp qua điện thoại.
  - Chuyển đổi 3 chế độ vận hành: Thủ công (Manual), Tự tránh vật cản (Avoidance), và Dò đường theo vạch (Line Tracking).
- **Hệ thống an toàn (Safety Watchdog)**:
  - Tự động ngắt xung động cơ sau 600ms nếu mất kết nối hoặc không nhận được lệnh tiếp theo.
  - Ngăn ngừa hiện tượng sụt áp (Brownout Reset) khi khởi động tải nặng.
