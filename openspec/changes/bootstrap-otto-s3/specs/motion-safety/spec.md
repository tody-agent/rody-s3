## ADDED Requirements

### Requirement: Mặc định an toàn
Khi boot, firmware MUST không xuất xung servo cho tới khi có lệnh chuyển động.

#### Scenario: Sau khi bật nguồn
- **WHEN** board vừa boot
- **THEN** `tach 1000` cho l = 0 và r = 0

### Requirement: Chuyển động có thời hạn
Lệnh drive SHALL tự dừng sau đúng ms đã yêu cầu (tối đa 5000); lệnh pwm SHALL tự tắt sau 10s.

#### Scenario: Watchdog
- **WHEN** gửi `drive 50 50 300` rồi chờ 800ms
- **THEN** `tach 500` cho l = 0 và r = 0

### Requirement: Bảo vệ pin
Khi pin < 3.4V, firmware SHALL dừng động cơ và từ chối lệnh chuyển động.

#### Scenario: Pin yếu
- **WHEN** bat đọc < 3.4V
- **THEN** drive trả ok=false, err="low_battery"
