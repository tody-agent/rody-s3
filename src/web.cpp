#include "web.h"
#include "drive.h"
#include "sensors.h"
#include "store.h"
#include "behaviors.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

namespace web {

static WebServer server(80);
static uint32_t lastClientPing = 0;
static bool webControlActive = false;

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>Otto S3 Control</title>
  <style>
    body { font-family: system-ui, sans-serif; text-align: center; background: #121212; color: #fff; margin: 0; padding: 20px; }
    h1 { margin-bottom: 5px; }
    .status { margin-bottom: 20px; font-size: 14px; color: #aaa; }
    .grid { display: grid; grid-template-columns: repeat(3, 80px); grid-gap: 15px; justify-content: center; margin: 20px auto; }
    button { background: #2563eb; color: #fff; border: none; border-radius: 12px; height: 80px; font-size: 24px; font-weight: bold; cursor: pointer; touch-action: manipulation; user-select: none; }
    button:active { background: #1d4ed8; }
    .stop-btn { background: #dc2626; grid-column: span 3; height: 50px; font-size: 18px; }
    .mode-select { margin-top: 20px; }
    select { padding: 8px 16px; border-radius: 8px; background: #333; color: #fff; border: 1px solid #555; font-size: 16px; }
  </style>
</head>
<body>
  <h1>🤖 Otto S3</h1>
  <div class="status" id="stat">Đang kết nối...</div>

  <div class="grid">
    <div></div>
    <button onpointerdown="sendDrive(60,60)" onpointerup="sendStop()">▲</button>
    <div></div>

    <button onpointerdown="sendDrive(-40,40)" onpointerup="sendStop()">◀</button>
    <button onclick="sendStop()">■</button>
    <button onpointerdown="sendDrive(40,-40)" onpointerup="sendStop()">▶</button>

    <div></div>
    <button onpointerdown="sendDrive(-60,-60)" onpointerup="sendStop()">▼</button>
    <div></div>

    <button class="stop-btn" onclick="sendStop()">STOP</button>
  </div>

  <div class="mode-select">
    <label>Chế độ: </label>
    <select id="modeSel" onchange="changeMode()">
      <option value="manual">Thủ công (Manual)</option>
      <option value="avoid">Tránh vật cản (Avoid)</option>
      <option value="line">Dò line (Line)</option>
    </select>
  </div>

  <script>
    let pingInterval;
    function sendDrive(l, r) {
      fetch(`/drive?l=${l}&r=${r}`).catch(()=>{});
      clearInterval(pingInterval);
      pingInterval = setInterval(() => fetch(`/drive?l=${l}&r=${r}`).catch(()=>{}), 400);
    }
    function sendStop() {
      clearInterval(pingInterval);
      fetch('/stop').catch(()=>{});
    }
    function changeMode() {
      const m = document.getElementById('modeSel').value;
      fetch(`/mode?m=${m}`).catch(()=>{});
    }
    setInterval(() => {
      fetch('/status').then(r=>r.json()).then(data => {
        document.getElementById('stat').innerHTML = `Pin: <b>${data.v.toFixed(2)}V</b> | Calib: <b>${data.cal ? 'Đạt' : 'Chưa'}</b> | Chế độ: <b>${data.mode}</b>`;
        document.getElementById('modeSel').value = data.mode;
      }).catch(()=>{});
    }, 1500);
  </script>
</body>
</html>
)rawliteral";

void init() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Otto-S3");
  IPAddress IP = WiFi.softAPIP();
  Serial.printf("# [web] SoftAP 'Otto-S3' started. IP: %s\n", IP.toString().c_str());

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", INDEX_HTML);
  });

  server.on("/drive", HTTP_GET, []() {
    if (behaviors::getMode() != behaviors::Mode::MANUAL) {
      server.send(400, "text/plain", "Not in manual mode");
      return;
    }
    float l = server.hasArg("l") ? server.arg("l").toFloat() : 0.0f;
    float r = server.hasArg("r") ? server.arg("r").toFloat() : 0.0f;
    const char* err = nullptr;
    if (drive::drive(l, r, 600, err)) {
      lastClientPing = millis();
      webControlActive = true;
      server.send(200, "text/plain", "OK");
    } else {
      server.send(500, "text/plain", err ? err : "Failed");
    }
  });

  server.on("/stop", HTTP_GET, []() {
    drive::stop();
    webControlActive = false;
    server.send(200, "text/plain", "OK");
  });

  server.on("/status", HTTP_GET, []() {
    float v = sensors::readBatteryVoltage();
    bool cal = store::gCalValid;
    const char* m = behaviors::getModeStr();
    String json = "{\"v\":" + String(v, 2) + ",\"cal\":" + (cal ? "true" : "false") + ",\"mode\":\"" + m + "\"}";
    server.send(200, "application/json", json);
  });

  server.on("/mode", HTTP_GET, []() {
    if (server.hasArg("m")) {
      behaviors::setMode(server.arg("m").c_str());
    }
    server.send(200, "text/plain", "OK");
  });

  server.begin();
}

void update() {
  server.handleClient();
  if (webControlActive && (millis() - lastClientPing > 1000)) {
    // Client connection timeout in manual mode
    drive::stop();
    webControlActive = false;
  }
}

}
