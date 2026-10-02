import time, pytest
pytestmark = pytest.mark.lifted

@pytest.mark.parametrize("w", ["L", "R"])
def test_deadband(otto, w):
    r = otto.cmd(f"cal deadband {w}", timeout=90)
    assert r["ok"] and 1350 <= r["lo"] <= r["hi"] <= 1650 and r["hi"] - r["lo"] < 150

def test_direction_set(otto):
    c = otto.cmd("cal show")
    assert c["l"]["fwd_sign"] in (1, -1) and c["r"]["fwd_sign"] in (1, -1), \
        "Chưa qua G1-dir: chạy `cal spin L/R`, hỏi người, rồi `cal dir`"

@pytest.mark.parametrize("w", ["L", "R"])
def test_sweep(otto, w):
    r = otto.cmd(f"cal sweep {w}", timeout=90)
    assert r["ok"] and max(r["fwd"]) > 30 and max(r["rev"]) > 30

@pytest.mark.parametrize("s", [30, 60, 90, -60])
def test_balance(otto, s):
    r = otto.cmd(f"meas {s}", timeout=10)
    assert r["err_pct"] <= 5, f"lệch {r['err_pct']}% (L={r['rpm_l']}, R={r['rpm_r']})"

def test_watchdog(otto):
    otto.cmd("drive 50 50 300"); time.sleep(0.8)
    r = otto.cmd("tach 500", 3); assert r["l"] == 0 and r["r"] == 0

def test_no_brownout(otto):
    t0 = otto.cmd("info")["uptime_ms"]
    otto.cmd("drive 100 100 1500"); time.sleep(2)
    i = otto.cmd("info")
    assert i["uptime_ms"] > t0 and i["reset_reason"] != "BROWNOUT"

def test_save(otto):            assert otto.cmd("cal save")["ok"]
