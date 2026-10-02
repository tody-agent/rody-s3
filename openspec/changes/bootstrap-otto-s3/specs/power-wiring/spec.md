## ADDED Requirements

### Requirement: Nguồn servo tách riêng
Servo SHALL chỉ nhận nguồn từ boost qua PCA9685 V+; ESP32 SHALL nhận 5V qua diode Schottky.

#### Scenario: USB và pin cùng lúc
- **WHEN** cắm USB trong khi công tắc đang bật và servo đang quay
- **THEN** board không reset (reset_reason ≠ BROWNOUT) và không có linh kiện nóng bất thường

### Requirement: Mức logic 3.3V
Mọi tín hiệu vào GPIO MUST ≤ 3.4V.

#### Scenario: Echo qua chuyển mức
- **WHEN** HC-SR05 phát xung Echo
- **THEN** điện áp tại GPIO5 ≤ 3.4V (checklist M7)
