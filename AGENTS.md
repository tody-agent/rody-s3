# Quy tắc cho AI agent – Otto S3

## Bạn ĐƯỢC tự làm
- Viết/sửa code trong src/, lib/, test/, tools/.
- `pio run -e otto`, `pio test -e native`, `pio run -e otto -t upload --upload-port $OTTO_PORT`.
- `pytest tools/hil -m bench` và các marker khác SAU KHI người xác nhận gate tương ứng.
- Gửi lệnh serial qua tools/hil/conftest.py (class Otto).

## Bạn PHẢI DỪNG và hỏi người (GATE)
| Gate | Điều kiện người phải xác nhận |
|------|-------------------------------|
| G0 | Đã đi dây theo docs/hardware/WIRING.md, đã đo đủ checklist M1–M8 |
| G1 | Bật công tắc nguồn servo; robot KÊ BÁNH LÊN KHỎI MẶT BÀN; 2 module IR đã gắn soi vào đĩa sọc |
| G1-dir | Quan sát và trả lời: "bánh L/R quay theo chiều robot TIẾN hay LÙI" |
| G2 | Robot đặt trên sàn phẳng, có băng keo thẳng ≥120cm; IR đã gắn lại hướng xuống sàn |
| G2-drift | Người đo độ lệch ngang (cm) và quãng đường (cm) rồi báo lại |

## Bạn KHÔNG ĐƯỢC
- Yêu cầu người đổi dây khi công tắc nguồn đang bật.
- Gửi lệnh chuyển động khi chưa qua G1.
- Sửa pins.h sang GPIO 19, 20, 26–37, 43, 44, 0(output), 3, 45, 46.
- Bỏ qua test thất bại; phải chẩn đoán theo docs/hardware/DEBUG.md.

## Lệnh nhanh
export OTTO_PORT=/dev/ttyUSB0      # Windows: set OTTO_PORT=COM5
pio test -e native
pio run -e otto -t upload --upload-port $OTTO_PORT
pytest tools/hil -m bench -v
pytest tools/hil -m lifted -v      # sau G1
pytest tools/hil -m floor -v -s    # sau G2 (-s để nhập số đo)
