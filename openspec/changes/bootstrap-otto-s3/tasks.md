## 1. Repo & build (agent, không cần board)
- [x] 1.1 Tạo cấu trúc repo, platformio.ini, pins.h, pytest.ini
- [x] 1.2 Viết lib/otto_calib theo design §4; `pio test -e native` PASS
- [x] 1.3 Viết firmware theo design §1–§6; `pio run -e otto` không warning nghiêm trọng
- [x] 1.4 Tạo tools/tach_disc.svg (Ø30mm, 4 đen/4 trắng, tâm có lỗ Ø6mm)

## GATE G0 – người đi dây + checklist M1–M8 trong WIRING.md

## 2. Bench (USB, công tắc TẮT)
- [ ] 2.1 Nạp firmware; `info` đúng flash/psram
- [ ] 2.2 `pytest tools/hil -m bench` PASS (ping, info, i2c, bat, us, ir, motion-off)
- [ ] 2.3 Nếu FAIL → DEBUG.md, sửa, lặp lại

## GATE G1 – bật công tắc, kê bánh, gắn IR vào đĩa sọc

## 3. Hiệu chỉnh bánh
- [ ] 3.1 `cal deadband L`, `cal deadband R`
- [ ] 3.2 `cal spin L` → hỏi người (G1-dir) → `cal dir L ±1`; tương tự R
- [ ] 3.3 `cal sweep L`, `cal sweep R`; rpm max mỗi hướng > 30
- [ ] 3.4 `cal save`; `pytest tools/hil -m lifted` PASS (balance ≤5%, watchdog, no brownout)

## GATE G2 – gắn IR hướng xuống sàn, đặt robot lên sàn có băng keo

## 4. Sàn
- [ ] 4.1 Người đo cầu bánh W (tâm lốp-tâm lốp) → `cal wheelbase W`
- [ ] 4.2 `pytest tools/hil -m floor -s` (vòng lặp drift ≤3 lần) → `cal save`
- [ ] 4.3 Kiểm tra reboot → cal_valid

## 5. Hành vi
- [ ] 5.1 mode avoid: dừng trước vật ≤ OBSTACLE_CM (20cm) trong 5/5 lần thử
- [ ] 5.2 mode line: hoàn thành 1 vòng đường đen rộng 18–25mm
- [ ] 5.3 Web SoftAP: điều khiển tay, stop khi nhả nút / mất kết nối
- [ ] 5.4 Cập nhật specs, `/opsx:archive`
