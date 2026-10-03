# GAME DESIGN DOCUMENT (GDD)
# RODY CYBER-PROVING GROUND / RODY TECH ARENA 3D
**Phiên bản:** 1.0.0 – Studio Production Blueprint  
**Vai trò:** Creative Director & Game Lead  
**Đối tượng:** Đội ngũ Kỹ thuật, 3D Artists, AI Gameplay Engineers & Lead Planner  
**Dự án gốc:** Otto S3 HP / Xiaozhi Mochi AI Companion (ESP32-S3 DevKit N16R8, TFT 1.54" ST7789, I2S INMP441, MAX98357A, PCA9685 Dual MG90S 360°)

---

## 1. TỔNG QUAN DỰ ÁN & Ý TƯỞNG CHỦ ĐẠO (EXECUTIVE SUMMARY & CORE CONCEPT)

### 1.1. Tuyên ngôn Dự án (High Concept Statement)
**"Rody Cyber-Proving Ground: The Autonomous Desktop Companion"** là một trải nghiệm 3D tương tác thời gian thực trên nền tảng WebGL / Three.js, tái hiện sinh động robot trợ lý để bàn **Rody** (phiên bản số hóa - Digital Twin của robot Otto S3 HP AI). Dự án kết hợp giữa **mô phỏng vật lý chân thực** của robot 2 bánh vi sai (differential drive), **hệ thống biểu cảm khuôn mặt động học** trên màn hình TFT 1.54", **tương tác giọng nói AI song ngữ** (Web Speech API), và **hệ thống âm thanh điện tử tổng hợp** (Web Audio Synthesizer).

```
                      ┌──────────────────────────────────────┐
                      │      RODY CYBER-PROVING GROUND       │
                      │  (3D Digital Twin & Neural Arena)    │
                      └──────────────────┬───────────────────┘
                                         │
       ┌──────────────────┬──────────────┴─────┬──────────────────┐
       ▼                  ▼                    ▼                  ▼
┌──────────────┐   ┌──────────────┐     ┌──────────────┐   ┌──────────────┐
│  Free Roam   │   │   Obstacle   │     │Voice Academy │   │ Sound Dance  │
│& Exploration │   │Agility Course│     │ (Bilingual)  │   │ Party (Beat) │
└──────────────┘   └──────────────┘     └──────────────┘   └──────────────┘
```

### 1.2. Trụ cột Trải nghiệm (Core Pillars)
1. **Physical Authenticity (Tính chân thực cơ học):** Bám sát 100% kích thước, trọng lượng, tỷ số truyền động và quán tính của robot thực tế (động cơ servo MG90S 360°, bán kính quay tại chỗ, cảm biến siêu âm quét nón, cảm biến hồng ngoại).
2. **Emotional Connection (Kết nối cảm xúc Mochi):** Rody không phải là chiếc xe đồ chơi vô tri, mà là một sinh linh công nghệ để bàn có tính cách (Desk Pet) với đôi mắt biểu cảm biến đổi linh hoạt (Vui vẻ, Ngạc nhiên, Bối rối, Ngái ngủ, Tinh nghịch) và âm thanh beep-boop giàu cảm xúc kiểu R2-D2 và WALL-E.
3. **Seamless Multimodal Interaction (Tương tác Đa phương thức mượt mà):** Người chơi điều khiển bằng Gamepad, Bàn phím/Chuột, Cảm ứng di động (D-pad/Joystick) và đặc biệt là **Khẩu lệnh giọng nói tự nhiên thời gian thực** (Voice Commands).
4. **Digital Twin Extensibility (Khả năng đồng bộ song sinh phần cứng):** Sẵn sàng kết nối 1:1 với robot phần cứng Otto S3 ngoài đời qua chuẩn Web Serial API (Serial 115200 baud, giao thức JSON dòng lệnh).

### 1.3. Phong cách Mỹ thuật & Thị giác (Art Direction & Visual Aesthetics)
- **Chủ đề thiết kế:** **Neo-Tech Cyber-Minimalism** kết hợp **Aperture Science High-Tech Lab** và **Astro Bot Cyber-Playground**.
- **Chất liệu nhân vật Rody:**
  - Vỏ thân: Nhựa kỹ thuật Matte White Polymer (nhám mịn cao cấp, chống bám vân tay) phối các mảng đen nhám Dark Slate `#131a27`.
  - Khung kính mặt (Visor): Acrylic đen khói mờ chống lóa, ẩn sau là màn hình TFT 1.54" IPS phát sáng rực rỡ (`Emissive CanvasTexture`).
  - Bánh xe & Lốp: Vành hợp kim nhôm Anodized màu xám titan, lốp cao su silicone mềm có rãnh bám sọc sần chống trượt.
  - Vòng phát sáng trạng thái (Status Ring): Dải LED Halo RGB chạy viền quanh đầu và ngực robot, nhấp nháy đồng bộ theo nhịp thở và trạng thái kết nối.
- **Môi trường Đấu trường (The Cyber-Proving Ground):**
  - Mặt sàn: Sàn gạch lục giác công nghệ cao (High-tech Hexagonal Grid Tiles) phủ lớp sơn phản xạ nhẹ, có các đường neon cyan (`#22d3ee`) và mint (`#34d399`) dẫn hướng phát quang trong bóng tối.
  - Hiệu ứng ánh sáng: Volumetric spot lights chiếu rọi khu vực trung tâm, Subtle Screen-Space Ambient Occlusion (SSAO), và Unreal Bloom nhẹ nhàng trên các điểm phát sáng LED.
  - Tỷ lệ hiển thị: Tỷ lệ bàn làm việc thu nhỏ (Miniature Desk Scale) mang lại cảm giác thân thương, ấm áp nhưng đầy tính công nghệ tương lai.

---

## 2. BỐI CẢNH THẾ GIỚI & CỐT TRUYỆN (WORLD LORE & NARRATIVE DESIGN)

### 2.1. Bối cảnh: "HexaCore Neural Proving Ground - Sector 7"
Vào năm 2042, Tập đoàn Công nghệ Robotics Tiên tiến **HexaCore Dynamics** phát triển dòng robot trợ lý để bàn thế hệ mới mang mã hiệu **Rody (Autonomous Neural Companion - Series S3)**. Trước khi được phân phối đến bàn làm việc của các kỹ sư, lập trình viên và gia đình, mỗi cá thể Rody đều phải vượt qua bài kiểm tra toàn diện tại **Cyber-Proving Ground (Đấu trường Huấn luyện Ảo)**.

Đấu trường này là một buồng thử nghiệm vật lý lượng tử đa chiều, nơi Rody học cách định hướng không gian, luồn lách qua chướng ngại vật của môi trường văn phòng, thấu hiểu ngữ điệu giọng nói con người và thể hiện cảm xúc chân thành qua ánh mắt.

```
       ┌────────────────────────────────────────────────────────┐
       │     HEXACORE DYNAMICS - NEURAL PROVING FACILITY        │
       │                                                        │
       │    [Kỹ Sư Huấn Luyện] ◄── Khẩu Lệnh Giọng Nói ──┐      │
       │             │                                   │      │
       │             ▼ (D-Pad / Voice Prompt)            │      │
       │    ┌───────────────────────────────────────┐    │      │
       │    │           RODY UNIT S3-HP             │    │      │
       │    │  - 2 Bánh Vi Sai MG90S                │    │      │
       │    │  - Mắt Mochi TFT 1.54"               │    │      │
       │    │  - Bộ Lọc Âm Thanh I2S INMP441        ├────┘      │
       │    │  - Hộp Loa Phản Ứng MAX98357A         │           │
       │    └───────────────────────────────────────┘           │
       │                                                        │
       │    [Môi Trường Thử Nghiệm: Cọc Tiêu, Radar, Nhạc Beat] │
       └────────────────────────────────────────────────────────┘
```

### 2.2. Nhân vật Rody (Personality & Character Profile)
- **Tên:** Rody (R-01 Dynamic Youth)
- **Mã định danh:** ESP32-S3-N16R8-MOCHI
- **Tính cách:**
  - *Tò mò & Hào hứng (Curious & Eager):* Luôn ngước nhìn xung quanh khi ở chế độ rảnh (`idle`), mắt đảo tròn quan sát huấn luyện viên.
  - *Hậu đậu đáng yêu (Lovably Clumsy):* Khi suýt đâm vào tường hoặc xoay vòng quá nhanh, mắt Rody lập tức chuyển sang trạng thái xoắn ốc (`dizzy`) và phát tiếng kêu ngơ ngác.
  - *Nhiệt tình & Yêu âm nhạc (Rhythmic Soul):* Bất cứ khi nào bắt được giai điệu hoặc beat nhạc có bass mạnh, Rody sẽ lắc lư 2 bánh và phát chùm âm thanh chirping vui nhướng mí mắt.
  - *Tuyệt đối trung thành:* Chăm chú lắng nghe khi nhận khẩu lệnh (`listening`) với thanh đo sóng âm nhấp nháy trên khóe mắt.

---

## 3. THIẾT KẾ CÁC CHẾ ĐỘ CHƠI (GAME MODES & SHOWCASE SUITE)

Hệ thống cung cấp **4 Chế Độ Chơi (Showcase Modes)** độc đáo, phục vụ cả mục đích giải trí, biểu diễn công nghệ lẫn kiểm thử chất lượng điều khiển robot:

```
┌─────────────────────────────────────────────────────────────────────────┐
│                       DANH SÁCH 4 CHẾ ĐỘ CHƠI                           │
├─────────────────────┬───────────────────┬───────────────────────────────┤
│ Chế độ              │ Thể loại          │ Cơ chế chính                  │
├─────────────────────┼───────────────────┼───────────────────────────────┤
│ 1. Free Roam        │ Sandbox / Thám    │ Lái tự do, tương tác bệ nhún, │
│    & Exploration    │ hiểm thế giới mở  │ docking station, 3 góc camera │
├─────────────────────┼───────────────────┼───────────────────────────────┤
│ 2. Obstacle Agility │ Time-Attack / Kỹ  │ Luồn lách cọc Slalom, tính giờ│
│    Course           │ năng phản xạ      │ phạt khi chạm vạch, Radar HUD │
├─────────────────────┼───────────────────┼───────────────────────────────┤
│ 3. Voice Command    │ Tương tác AI /    │ Huấn luyện nhận diện khẩu     │
│    Academy          │ Speech Training   │ lệnh thời gian thực, combo    │
├─────────────────────┼───────────────────┼───────────────────────────────┤
│ 4. Sound-Reactive   │ Âm nhạc /         │ Rody nhảy theo beat Web Audio,│
│    Dance Party      │ Rhythm Visualizer │ ánh sáng sàn disco, chiptune  │
└─────────────────────┴───────────────────┴───────────────────────────────┘
```

---

### 3.1. Chế độ 1: Free Roam & Cyber Exploration (Tự Do Thám Hiểm)
- **Mục tiêu:** Cho phép người chơi tự do điều khiển Rody khám phá toàn bộ khuôn viên đấu trường công nghệ cao đa tầng mà không có giới hạn thời gian.
- **Cơ chế Điều khiển (Controls):**
  - Bàn phím: `W/A/S/D` hoặc `Phím Mũi Tên` (W: Tiến, S: Lùi, A: Xoay trái, D: Xoay phải, Space: Phanh gấp).
  - Màn hình cảm ứng: Virtual Analog Joystick hoặc D-pad 4 hướng có hỗ trợ Haptic Feedback (rung phản hồi).
  - Gamepad (Xbox/PlayStation): Cần gạt Left Stick điều hướng, Nút `A/X` để bấm còi chíp.
- **Tính năng Môi trường Tương tác:**
  - *Speed Boost Pads (Thảm tăng tốc photon):* Khi Rody chạy qua vạch phát sáng màu vàng cam, tốc độ bánh xe tăng vọt 200% trong 2 giây, Rody kích hoạt biểu cảm mắt phấn khích (`happy`).
  - *Wireless Docking Pod (Trạm sạc từ tính):* Khi đỗ Rody vào ô trạm sạc trung tâm, robot sẽ tự động hạ phanh, màn hình TFT chuyển sang hiệu ứng ngáp ngủ và chữ `Z z z` bay lên (`sleepy`), phát tiếng ngáy robot êm dịu.
  - *Interactive Diagnostic Pillars (Trụ chẩn đoán dữ liệu):* Tiến lại gần trụ để quét thông số phần cứng (RAM PSRAM 8MB, Flash 16MB, Điện áp pin Li-ion 4.12V, Tần số CPU 240MHz).
- **Hệ thống Camera Đa góc nhìn (Dynamic Camera Switcher):**
  - **Camera 1 - Third-Person Chase (Mặc định):** Góc nhìn thứ ba mượt mà phía sau robot với giảm chấn chuyển động (smooth lerping damping).
  - **Camera 2 - FPV "Rody Visor":** Góc nhìn người thứ nhất gắn ngay trên đỉnh trán Rody, tích hợp lưới ngắm holographic quét khoảng cách chướng ngại vật.
  - **Camera 3 - Cinematic Orbit / Drone Cam:** Camera tự động xoay quanh Rody, chụp lại những góc máy trình diễn đậm chất điện ảnh.

---

### 3.2. Chế độ 2: Obstacle Agility Course (Đường Đua Cọc Tiêu Tính Giờ)
- **Mục tiêu:** Thử thách kỹ năng lái xe vi sai chính xác của người chơi. Rody phải xuất phát từ vạch `START`, luồn lách qua hàng loạt cọc tiêu Slalom, né tránh các khối hộp laser động, và cán đích tại vạch `FINISH` trong thời gian ngắn nhất.
- **Cơ chế Tính điểm & Đánh giá (Scoring & Rank Matrix):**
  - Thời gian gốc: Đồng hồ bấm giờ tính từng mili-giây (00:00.000).
  - *Hình phạt va chạm (Penalties):*
    - Quẹt cọc tiêu hoặc tường chắn: **+3.00 giây** phạt / lần va chạm.
    - Cảm biến siêu âm HC-SR05 3D mô phỏng chùm tia quét nón (Sonar Detection Cone). Khi phát hiện vật cản dưới khoảng cách an toàn 15cm, Rody lập tức hú còi cảnh báo `OBSTACLE_ALARM`, mắt TFT chuyển thành hai chữ `X X` màu đỏ rực.
  - *Bảng xếp hạng danh hiệu (Certification Tiers):*
    - **S-Rank (Master Navigator):** Dưới 25 giây, 0 lỗi va chạm. Rody nhảy múa ăn mừng rực rỡ.
    - **A-Rank (Expert Bot):** Từ 25 đến 35 giây, ≤ 1 lỗi va chạm.
    - **B-Rank (Certified Companion):** Từ 36 đến 50 giây.
    - **C-Rank (Needs Recalibration):** Trên 50 giây hoặc trên 4 lỗi va chạm. Rody hiện biểu cảm chóng mặt bối rối (`dizzy`).
- **Chướng ngại vật đa dạng (Dynamic Hazards):**
  - *Slalom Pylons:* Dãy cọc tiêu neon phát sáng yêu cầu đánh võng zigzag nhịp nhàng giữa 2 bánh.
  - *Moving Scanner Barriers:* Rào chắn laser quét qua lại theo chu kỳ 3 giây, buộc người chơi phải phanh chờ thời điểm an toàn.
  - *Narrow Bridge (Cầu hẹp công nghệ):* Đoạn đường hẹp 15cm thử thách khả năng cân bằng hướng thẳng của 2 động cơ servo.

---

### 3.3. Chế độ 3: Voice Command Academy (Học Viện Khẩu Lệnh Giọng Nói)
- **Mục tiêu:** Chế độ huấn luyện và trình diễn khả năng nhận diện tiếng nói AI thông minh. Hệ thống đưa ra các bài sát hạch phản xạ để người chơi tương tác với Rody bằng chính giọng nói thật của mình.
- **Luồng Tương Tác Giọng Nói (Speech Interaction Loop):**

```
┌─────────────────┐       ┌─────────────────┐       ┌─────────────────┐
│  Người Huấn Luyện│ ────► │  Web Speech API │ ────► │ Khớp Lệnh & Xử  │
│  Phát Khẩu Lệnh │       │  (ASR Engine)   │       │ Lý Ý Định (NLU) │
└─────────────────┘       └─────────────────┘       └────────┬────────┘
                                                             │
         ┌───────────────────────────────────────────────────┘
         ▼
┌──────────────────┐      ┌──────────────────┐      ┌──────────────────┐
│  Mắt Rody Lắng   │      │  Âm Báo Xác Nhận │      │  Thực Thi Động Cơ│
│  Nghe (Listening)│ ───► │  (Command ACK)   │ ───► │  (MG90S Servos)  │
└──────────────────┘      └──────────────────┘      └──────────────────┘
```

- **Hệ thống Bài tập & Thử thách (Academy Missions):**
  - *Bài 1 - Basic Locomotion Drill:* "Rody, tiến lên!" -> Rody di chuyển tới trước 1.5m -> "Rody, dừng lại!" -> Rody phanh bánh và kêu tiếng `BEEP`.
  - *Bài 2 - Navigation Test:* Thực hiện chuỗi lệnh rẽ trái, rẽ phải và lùi chuẩn xác vào ô đỗ xe định vị.
  - *Bài 3 - Emotional Empathy Check:* Nói các câu khích lệ "Chào bạn", "Vui vẻ lên nào", "Rody giỏi lắm" để mở khóa biểu cảm hạnh phúc `HAPPY_CHIRP`.
  - *Bài 4 - Voice Combo Frenzy:* Hoàn thành 5 khẩu lệnh ngẫu nhiên xuất hiện trên màn hình trong vòng 30 giây mà không bị nhận diện sai.
- **Thước đo Sóng Âm (Acoustic Waveform HUD):**
  - Trên màn hình TFT của Rody hiển thị cột sóng âm 7 vạch (`Equalizer Bars`) nhảy múa theo biên độ âm lượng giọng nói thu được từ micro.
  - Hệ thống chấm điểm độ tự tin nhận diện (`Confidence Score %`).

---

### 3.4. Chế độ 4: Sound-Reactive Dance Party (Vũ Hội Âm Nhạc & Nhịp Điệu Beat)
- **Mục tiêu:** Biến Rody Cyber-Proving Ground thành một sàn nhảy ánh sáng tương tác âm thanh (Cyber Disco / Rhythm Playground). Rody sẽ cảm nhận nhịp beat và tự động biểu diễn các bước nhảy cơ điện đẹp mắt.
- **Cơ chế Phân tích Tần số (Audio Analyser Architecture):**
  - Sử dụng `AnalyserNode` trong Web Audio API (FFT Size = 256) phân tách 3 dải tần số chính:
    1. **Bass Sub (40Hz - 160Hz - Tiếng Trống Kick & Bass Drop):**
       - Kích hoạt động tác lắc hông bánh xe: Bánh trái tiến, bánh phải lùi và đảo chiều liên tục với tần số 120BPM (Wiggle Dance).
       - Các vòng neon trên mặt sàn đập xung kích quang học (Pulse Rings) lan tỏa từ chân Rody ra khắp phòng.
    2. **Mid Range (300Hz - 2500Hz - Giai điệu & Giọng Hát):**
       - Màn hình TFT của Rody chuyển sang chế độ ca hát (`speaking` / `happy`), khuôn miệng sóng âm uốn lượn theo biên độ âm thanh.
    3. **Treble Highs (4000Hz - 12000Hz - Hi-hat & Tiếng Chiptune):**
       - Rody thực hiện động tác xoay tròn 360 độ (Pirouette Spin) và bắn pháo hoa hạt photon (Particle Confetti) xung quanh.
- **Thư viện Âm nhạc Sẵn có & Tải nhạc ngoài:**
  - Tích hợp sẵn 3 bài nhạc Synthwave / Chiptune 8-bit bản quyền mở chất lượng cao.
  - Hỗ trợ chế độ **Live Microphone Dance Mode:** Người chơi bật mic, vỗ tay hoặc bật bất kỳ bài hát nào từ điện thoại/máy tính, Rody sẽ nghe qua mic và nhảy múa theo đúng nhịp điệu ngoài đời thực!

---

## 4. THIẾT KẾ HỆ THỐNG ÂM THANH & LỜI THOẠI (AUDIO & DIALOGUE SPECIFICATION)

### 4.1. Kho Khẩu Lệnh Giọng Nói Song Ngữ (Bilingual Voice Command Lexicon)
Hệ thống hỗ trợ cơ chế nhận dạng từ khóa (Keyword Spotting) song ngữ Tiếng Việt & Tiếng Anh với khả năng khớp linh hoạt các từ đồng nghĩa và biến thể phát âm:

```
┌───────────────────────────────────────────────────────────────────────────────────────────┐
│                     KHO LỆNH GIỌNG NÓI & ÁNH XẠ HÀNH VI RODY                             │
├────┬────────────┬─────────────────────────────┬──────────────────────────┬────────────────┤
│ ID │ Ý định     │ Từ khóa Tiếng Việt (VN)     │ Từ khóa Tiếng Anh (EN)   │ Hành vi Robot │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 01 │ FORWARD    │ 'tiến', 'tiến lên', 'chạy   │ 'forward', 'ahead', 'go',│ Tiến thẳng     │
│    │            │ tới', 'thẳng', 'đi tới'     │ 'straight', 'move'       │ 50% tốc độ    │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 02 │ BACKWARD   │ 'lùi', 'lùi lại', 'thụt     │ 'backward', 'back',      │ Lùi xe         │
│    │            │ lùi', 'đi lùi'              │ 'reverse', 'retreat'     │ 40% tốc độ    │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 03 │ TURN_LEFT  │ 'trái', 'quẹo trái', 'rẽ    │ 'left', 'turn left',     │ Quay trái      │
│    │            │ trái', 'sang trái'          │ 'steer left'             │ 35% tốc độ    │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 04 │ TURN_RIGHT │ 'phải', 'quẹo phải', 'rẽ    │ 'right', 'turn right',   │ Quay phải     │
│    │            │ phải', 'sang phải'          │ 'steer right'            │ 35% tốc độ    │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 05 │ STOP       │ 'dừng', 'dừng lại', 'đứng   │ 'stop', 'halt', 'brake', │ Phanh 2 bánh,  │
│    │            │ yên', 'thôi', 'hãm'         │ 'freeze', 'wait'         │ về thế nghỉ    │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 06 │ SPIN       │ 'xoay tròn', 'xoay', 'quay  │ 'spin', 'twirl', 'turn   │ Xoay vòng 360° │
│    │            │ múa', 'lộn vòng'            │ around', 'rotate'        │ mắt dizzy      │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 07 │ DANCE      │ 'nhảy múa', 'quẩy lên',     │ 'dance', 'party',        │ Vũ đạo theo    │
│    │            │ 'múa đi', 'quẩy'            │ 'groove', 'disco'        │ beat nhịp điệu │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 08 │ GREETING   │ 'chào bạn', 'xin chào',     │ 'hello', 'hi rody', 'hey │ Nháy mắt,      │
│    │            │ 'rody ơi', 'ê rody'         │ buddy', 'greetings'      │ gật đầu chào   │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 09 │ HAPPY      │ 'vui vẻ', 'khen ngợi',      │ 'happy', 'good boy',     │ Mắt cười cong, │
│    │            │ 'giỏi lắm', 'tuyệt vời'     │ 'cheer', 'awesome'       │ chirp reo vui  │
├────┼────────────┼─────────────────────────────┼──────────────────────────┼────────────────┤
│ 10 │ SLEEP      │ 'ngủ đi', 'đi ngủ', 'tắt    │ 'sleep', 'go to sleep',  │ Nhắm mắt lim   │
│    │            │ máy', 'nghỉ ngơi'           │ 'goodnight', 'nap'       │ dim, ngáy Zzz  │
└────┴────────────┴─────────────────────────────┴──────────────────────────┴────────────────┘
```

---

### 4.2. Kiến Trúc Bộ Tổng Hợp Âm Thanh Điện Tử (Web Audio Procedural Synthesizer)
Để đạt độ trung thực tối đa với phần cứng ESP32-S3 (Amply MAX98357A + Loa 3W) và tạo nên bản sắc âm thanh độc đáo, toàn bộ hiệu ứng SFX được sinh theo thuật toán **Procedural Audio Synthesis** (dao động sóng Sine, Triangle, Sawtooth kết hợp ADSR Envelope):

```
                        ┌───────────────────────────────┐
                        │   AUDIO CONTEXT MASTER BUS    │
                        └───────────────▲───────────────┘
                                        │ (Gain Node / DynamicsCompressor)
          ┌─────────────────────────────┼─────────────────────────────┐
          │                             │                             │
┌──────────────────┐          ┌──────────────────┐          ┌──────────────────┐
│  Tone Generator  │          │   Noise Shaper   │          │  Envelope ADSR   │
│ (Sine/Tri/Saw)   │          │  (White/Pink)    │          │ (Attack, Decay)  │
└──────────────────┘          └──────────────────┘          └──────────────────┘
```

#### Chi tiết Thuật toán 8 Hiệu ứng Âm thanh Đặc trưng:
1. **Boot Sequence (`sfx_boot`):**
   - *Mô tả:* Tiếng khởi động chào mừng của Rody khi bật nguồn.
   - *Cấu trúc:* Hợp âm 3 nốt Arpeggio tăng dần: A4 (440Hz, 70ms) -> E5 (659Hz, 80ms) -> A5 (880Hz, 140ms). Sóng Sine trong trẻo với Attack 10ms, Decay 60ms.
2. **Servo Motor Whine (`sfx_servo`):**
   - *Mô tả:* Tiếng rít cơ khí êm tai của 2 cụm bánh răng kim loại servo MG90S khi tăng tốc.
   - *Cấu trúc:* Tần số điều chế dao động kép (Dual-Oscillator FM) từ 140Hz đến 380Hz theo tốc độ bánh xe thực tế, kết hợp bộ lọc Low-pass 800Hz.
3. **Sonar Radar Ping (`sfx_sonar`):**
   - *Mô tả:* Tiếng ping phát xạ của cảm biến khoảng cách siêu âm HC-SR05.
   - *Cấu trúc:* Xung âm tần số cao 2200Hz cực ngắn (duration 45ms), decay dốc nhanh tạo tiếng "ping" sắc bén của radar ngầm.
4. **Beep Confirmation (`sfx_beep`):**
   - *Mô tả:* Tiếng tít ngắn xác nhận thao tác bấm nút hoặc dừng lại an toàn.
   - *Cấu trúc:* B5 (987Hz, 60ms) nối tiếp E6 (1318Hz, 90ms), sóng Triangle êm ái.
5. **Happy Chirp (`sfx_happy` - Wall-E / R2-D2 Style):**
   - *Mô tả:* Tiếng reo vui líu lo của Rody khi được khen ngợi hoặc nhảy múa.
   - *Cấu trúc:* Chuỗi Glissando 12 bước sóng trượt từ 600Hz vút lên 1600Hz (bước 80Hz, 18ms/bước), kết thúc bằng nốt A6 (1760Hz, 100ms).
6. **Command Acknowledged (`sfx_ack`):**
   - *Mô tả:* Tiếng bíp đôi xác nhận Rody đã nghe và hiểu khẩu lệnh vừa phát.
   - *Cấu trúc:* C6 (1046Hz, 50ms) -> G6 (1567Hz, 80ms), nảy vui tươi.
7. **Obstacle Hazard Alarm (`sfx_obstacle`):**
   - *Mô tả:* Tiếng còi báo động khẩn cấp khi gặp vật cản trong cự ly nguy hiểm.
   - *Cấu trúc:* 2 chu kỳ nốt còi xe cứu thương xen kẽ: 1200Hz (80ms) đập xuống 600Hz (100ms) dạng sóng Sawtooth thô ráp.
8. **Sleepy Snooze (`sfx_sleep`):**
   - *Mô tả:* Tiếng thở ngáy đều đặn của Rody khi đi ngủ.
   - *Cấu trúc:* Lọc ồn hồng (Pink Noise) kết hợp LFO chậm 0.4Hz quét từ 120Hz đến 80Hz tạo nhịp thở nhẹ nhàng.

---

## 5. THIẾT KẾ BIỂU CẢM & HOẠT HỌA KHUÔN MẶT (FACIAL RIGGING & MOCHI EMOTIONS)

Khuôn mặt Rody là linh hồn của robot. Trong game 3D, khuôn mặt được tạo bởi một **Dynamic 2D Canvas (240x240 px)** đóng vai trò là `CanvasTexture` phát sáng (`MeshBasicMaterial` hoặc `emissiveMap`) trên bề mặt màn hình IPS cong:

```
┌────────────────────────────────────────────────────────────────────────┐
│             HỆ THỐNG BIỂU CẢM MẮT MOCHI (src/emotion_gfx.cpp)          │
├──────────────┬────────────────────────┬────────────────────────────────┤
│ Tên Biểu Cảm │ Màu Sắc Sáng           │ Đặc Điểm Hoạt Họa              │
├──────────────┼────────────────────────┼────────────────────────────────┤
│ 1. IDLE      │ Cyan Neon (`#22d3ee`)   │ Mắt chữ nhật bo góc tròn to,   │
│              │                        │ tự động chớp mắt & liếc tự do  │
├──────────────┼────────────────────────┼────────────────────────────────┤
│ 2. HAPPY     │ Mint Green (`#34d399`) │ Mắt hình bán nguyệt cầu vồng,  │
│              │                        │ nhấp nhô nhún nhảy + má hồng   │
├──────────────┼────────────────────────┼────────────────────────────────┤
│ 3. LISTENING │ Sky Blue (`#38bdf8`)   │ Mắt mở to tập trung, thanh     │
│              │                        │ Equalizer 7 vạch rung đáy mắt  │
├──────────────┼────────────────────────┼────────────────────────────────┤
│ 4. THINKING  │ Amber Gold (`#fbbf24`) │ Mắt liếc góc trên, điểm tròn   │
│              │                        │ xoay quỹ đạo vệ tinh ở giữa    │
├──────────────┼────────────────────────┼────────────────────────────────┤
│ 5. SPEAKING  │ Purple Lilac (`#a78bfa`)│ Đôi mắt ấm áp kèm khuôn miệng  │
│              │                        │ phát âm co giãn theo lời nói   │
├──────────────┼────────────────────────┼────────────────────────────────┤
│ 6. DRIVE_FWD │ Cyan Flow (`#38bdf8`)  │ Mắt rướn nhẹ về trước, vạch    │
│              │                        │ photon mũi tên trượt tiến      │
├──────────────┼────────────────────────┼────────────────────────────────┤
│ 7. DIZZY     │ Rose Pink (`#f43f5e`)  │ Hai vòng xoáy ốc xoay ngược    │
│              │                        │ chiều nhau khi quay cuồng      │
├──────────────┼────────────────────────┼────────────────────────────────┤
│ 8. OBSTACLE  │ Danger Red (`#ef4444`) │ Hai dấu chéo `X X` nhấp nháy   │
│              │                        │ báo động va chạm               │
├──────────────┼────────────────────────┼────────────────────────────────┤
│ 9. SLEEPY    │ Muted Slate (`#94a3b8`)│ Mắt nhắm dạng 2 sợi chỉ ngang, │
│              │                        │ các chữ `Z z Z` bay lên cao    │
└──────────────┴────────────────────────┴────────────────────────────────┘
```

---

## 6. KIẾN TRÚC KỸ THUẬT & CÔNG NGHỆ 3D (TECHNICAL SPECIFICATIONS & ARCHITECTURE)

```
┌────────────────────────────────────────────────────────────────────────┐
│                        KIẾN TRÚC BỘ GAME ENGINE 3D                     │
├────────────────────────────────────────────────────────────────────────┤
│  1. Rendering Core: Three.js r160+ (WebGL 2.0 / WebGPU Ready)           │
│  2. Physics Engine: Cannon-es / Rapier3D (RigidBody 2-wheel vi sai)    │
│  3. Procedural Screen: 2D OffscreenCanvas -> Three.js CanvasTexture    │
│  4. Audio System: Web Audio API (Master Bus, BiquadFilter, Analyser)   │
│  5. Speech Recognition: Web Speech API (vi-VN & en-US multi-engine)    │
│  6. Controls & Input: PointerEvents, Virtual Joystick, Gamepad API     │
│  7. Hardware Bridge: Web Serial API (Giao tiếp với ESP32 thực tế)      │
└────────────────────────────────────────────────────────────────────────┘
```

### 6.1. Chi tiết Mô hình 3D Robot Rody (Hierarchical Scene Graph)
Robot được ghép nối từ các khối hình học nguyên bản (Procedural CSG Primitives) hoặc tệp GLTF xuất từ CAD thiết kế 3D:
- `Rody_Root` (Object3D - Tọa độ không gian x, y, z)
  - `Chassis_Base` (Mesh rounded-box, chất liệu nhám trắng)
    - `Battery_Compartment` (Khoang chứa pin dưới đáy)
    - `Front_Bumper` (Cản trước bảo vệ)
  - `Wheel_Left_Pivot` (Trục quay bánh xe trái)
    - `Wheel_Left_Mesh` (Vành titan + Lốp gai cao su)
  - `Wheel_Right_Pivot` (Trục quay bánh xe phải)
    - `Wheel_Right_Mesh` (Vành titan + Lốp gai cao su)
  - `Caster_Ball` (Bánh xe dẫn hướng bi lăn phía sau)
  - `Head_Assembly` (Cụm đầu robot có thể nghiêng nhẹ tilt ±8°)
    - `Visor_Glass` (Kính vát cong màu khói mờ)
    - `TFT_Screen_Plane` (Mặt phẳng hiển thị Texture 240x240 phát sáng)
    - `Sonar_Eyes` (2 hốc tròn mô phỏng bộ phát/thu siêu âm)
    - `Status_Halo_Ring` (Vòng sáng neon phát hào quang)

### 6.2. Mô hình Vật Lý Hai Bánh Vi Sai (Differential Drive Kinematics)
Hệ thống tính toán chuyển động chuẩn xác theo công thức vi sai hai bánh:
$$\text{Vận tốc tịnh tiến } v = \frac{v_L + v_R}{2}$$
$$\text{Vận tốc góc } \omega = \frac{v_R - v_L}{L}$$
Trong đó:
- $v_L, v_R$: Vận tốc tiếp tuyến của bánh trái và bánh phải (tỷ lệ với xung PWM servo).
- $L$: Khoảng cách giữa 2 tâm bánh xe ($L = 68.0\text{ mm} = 0.068\text{ m}$).
- Bán kính bánh xe: $R = 21.0\text{ mm} = 0.021\text{ m}$.
- Quán tính trượt (Slip Friction) và lực cản lăn (Rolling Resistance) được áp dụng để mang lại cảm giác lái đầm chắc, chống giật lố.

---

## 7. LỘ TRÌNH TRIỂN KHAI CHO STUDIO (DEVELOPMENT ROADMAP & MILESTONES)

| Giai đoạn | Mục tiêu chính | Đầu ra (Deliverables) |
|:---|:---|:---|
| **Phase 1: 3D Core & Modeling** | Xây dựng khung hình 3D Three.js, model Rody hoàn chỉnh với vật liệu PBR, sàn đấu trường Proving Ground. | Scene Three.js có ánh sáng, bóng đổ, Rody di chuyển trơn tru với bàn phím và gamepad. |
| **Phase 2: Screen & Sound Synth** | Tích hợp Texture Canvas động cho màn hình TFT 1.54", phát triển module Web Audio Synth với đủ 8 SFX. | Mắt Mochi chớp liếc tự nhiên theo cảm xúc, tiếng bíp boop R2-D2 vang chuẩn xác khi tương tác. |
| **Phase 3: 4 Game Modes Integration** | Lập trình logic cho Free Roam, Obstacle Course tính giờ, Voice Academy và Sound-Reactive Dance. | Cả 4 chế độ chơi hoạt động mượt mà, có bảng điểm, vạch đích, nhạc beat và nhận diện mic. |
| **Phase 4: UI/UX & Mobile Optimization** | Hoàn thiện giao diện điều khiển hiện đại, D-Pad cảm ứng, tối ưu 60 FPS trên cả iPhone/Android/Desktop. | Giao diện Responsive chuẩn WCAG AA, không giật lag, haptic vibration nhạy bén. |
| **Phase 5: Hardware Twin Bridge** | Kết nối Web Serial API trực tiếp tới kit ESP32-S3 qua cáp USB để điều khiển đồng bộ robot thật. | Điều khiển 3D trên màn hình thì robot trên bàn chạy y hệt thời gian thực. |

---

## 8. KẾT LUẬN & ĐỀ XUẤT CHO LEAD PLANNER

Tài liệu GDD này đã thiết lập nền tảng toàn diện và vững chắc từ **Ý tưởng chủ đạo**, **Cốt truyện truyền cảm hứng**, **4 Chế độ chơi đột phá**, **Kho khẩu lệnh song ngữ**, cho đến **Kiến trúc kỹ thuật 3D & Âm thanh Web Audio**.

Dự án không chỉ là một trò chơi 3D bắt mắt mà còn là một **công cụ trình diễn đẳng cấp (Flagship Showcase)** cho robot Rody / Otto S3 HP, chứng minh tiềm năng thương mại hóa và ứng dụng giáo dục STEM/AI vượt trội.

*Được biên soạn bởi: Creative Director & Game Lead - Rody 3D Game Studio.*
