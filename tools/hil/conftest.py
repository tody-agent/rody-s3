import json, os, time
import pytest, serial

class Otto:
    def __init__(self, port, baud=115200):
        self.s = serial.Serial(port, baud, timeout=0.2)
        time.sleep(2.0)                      # board có thể reset khi mở cổng
        self.s.reset_input_buffer()

    def cmd(self, line, timeout=5.0):
        self.s.write((line + "\n").encode())
        key, end = line.split()[0], time.time() + timeout
        while time.time() < end:
            raw = self.s.readline().decode(errors="ignore").strip()
            if raw.startswith("#"):
                print(raw)
            elif raw.startswith("{"):
                msg = json.loads(raw)
                if msg.get("cmd") == key:
                    return msg
        raise TimeoutError(f"no reply: {line}")

    def close(self):
        try: self.cmd("stop", 1.0)
        finally: self.s.close()

Rody = Otto

@pytest.fixture(scope="session")
def rody():
    port = os.environ.get("RODY_PORT") or os.environ.get("OTTO_PORT")
    if not port:
        pytest.skip("Đặt biến RODY_PORT hoặc OTTO_PORT (vd /dev/ttyUSB0 hoặc COM5)")
    o = Rody(port)
    yield o
    o.close()

@pytest.fixture(scope="session")
def otto(rody):
    return rody
