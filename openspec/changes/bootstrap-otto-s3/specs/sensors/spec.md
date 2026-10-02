## ADDED Requirements

### Requirement: Siêu âm
Firmware SHALL đo khoảng cách 2–400cm, trả 999 khi timeout 30ms.

#### Scenario: Vật cản trước mặt
- **WHEN** đặt tấm bìa cách 20cm và gửi `us 5`
- **THEN** ≥4/5 mẫu trong khoảng 18–22cm

### Requirement: Đo pin
Firmware SHALL tính VBAT = ADC_mV × BAT_DIV / 1000 với BAT_DIV mặc định 5.0.

#### Scenario: So với đồng hồ
- **WHEN** gửi `bat`
- **THEN** v lệch ≤ 0.1V so với đồng hồ đo ở VBAT_SW

### Requirement: Tachometer
Firmware SHALL đếm cạnh IR bằng ngắt với debounce 2ms.

#### Scenario: Quay tay
- **WHEN** quay bánh bằng tay 1 vòng trong khi `tach 3000`
- **THEN** số cạnh ≈ 8 (±1)
