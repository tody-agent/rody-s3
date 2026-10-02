import pytest
pytestmark = pytest.mark.floor

def test_straight_line(otto):
    assert otto.cmd("info")["cal_valid"]
    for it in range(3):
        input(f"[Vòng {it+1}] Đặt robot ở đầu băng keo, Enter để chạy...")
        otto.cmd("drive 60 60 4000", timeout=6)
        D = float(input("Quãng đường đã đi (cm): "))
        d = float(input("Lệch ngang (cm, TRÁI dương, PHẢI âm): "))
        if abs(d) / D * 100 <= 2.0:
            assert otto.cmd("cal save")["ok"]; return
        r = otto.cmd(f"cal drift {d} {D}")
        print("trim:", r["trim_l"], r["trim_r"])
    pytest.fail("Sau 3 vòng vẫn lệch >2cm/100cm – xem DEBUG.md (lốp, bi, ma sát)")
