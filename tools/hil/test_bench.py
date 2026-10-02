import time, pytest
pytestmark = pytest.mark.bench

def test_ping(otto):            assert otto.cmd("ping")["ok"]
def test_memory(otto):
    r = otto.cmd("info"); assert r["flash"] == 16777216 and r["psram"] > 7_000_000
def test_i2c_pca(otto):         assert 0x40 in otto.cmd("i2c")["devices"]
def test_battery(otto):         assert 3.0 <= otto.cmd("bat")["v"] <= 4.3
def test_ultrasonic(otto):
    r = otto.cmd("us 5"); ok = [c for c in r["cm"] if 2 <= c <= 400]
    assert len(ok) >= 4, f"siêu âm: {r['cm']} – kiểm tra shifter/HV 5V"
def test_ir_digital(otto):
    r = otto.cmd("ir"); assert r["l"] in (0, 1) and r["r"] in (0, 1)
def test_no_motion_at_boot(otto):
    r = otto.cmd("tach 1000", 3); assert r["l"] == 0 and r["r"] == 0
def test_unknown_cmd(otto):     assert otto.cmd("abc")["ok"] is False
