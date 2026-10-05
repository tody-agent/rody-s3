#include "web.h"
#include "drive.h"
#include "sensors.h"
#include "imu_sensor.h"
#include "store.h"
#include "behaviors.h"
#include "emotion_gfx.h"
#include "oled_diag.h"
#include "audio_player.h"
#include "../include/debug_log.h"
#include "../include/display_policy.h"
#include "../include/pin_catalog.h"
#include "../include/pins.h"
#include "../include/wheel_cmd.h"
#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <vector>

namespace web {

static WebServer server(80);
static uint32_t lastClientPing = 0;
static bool webControlActive = false;
static bool rebootPending = false;
static uint32_t rebootAt = 0;

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no, viewport-fit=cover">
  <title>Rody S3 — Hardware Console</title>
  <style>
    :root {
      --bg: #f8fafc;
      --surface: #ffffff;
      --surface-subtle: #f1f5f9;
      --surface-active: #e2e8f0;
      --border: #e2e8f0;
      --border-strong: #cbd5e1;
      --fg: #0f172a;
      --fg-muted: #64748b;
      --fg-subtle: #94a3b8;

      --primary: #0284c7;
      --primary-hover: #0369a1;
      --primary-subtle: #e0f2fe;
      --primary-text: #0369a1;

      --success: #16a34a;
      --success-subtle: #dcfce7;
      --success-text: #15803d;

      --warning: #d97706;
      --warning-subtle: #fef3c7;
      --warning-text: #b45309;

      --danger: #dc2626;
      --danger-hover: #b91c1c;
      --danger-subtle: #fee2e2;
      --danger-text: #b91c1c;

      --shadow-sm: 0 1px 2px rgba(15, 23, 42, 0.04);
      --shadow-md: 0 2px 6px -1px rgba(15, 23, 42, 0.06), 0 1px 4px -1px rgba(15, 23, 42, 0.04);
      --radius-sm: 8px;
      --radius-md: 12px;
      --radius-lg: 16px;
    }

    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      -webkit-tap-highlight-color: transparent;
    }

    html, body {
      max-width: 100%;
      overflow-x: hidden;
      background: var(--bg);
      color: var(--fg);
      font-family: -apple-system, BlinkMacSystemFont, "SF Pro Text", "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
      -webkit-font-smoothing: antialiased;
    }

    body {
      max-width: 480px;
      margin: 0 auto;
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      padding-bottom: 24px;
      user-select: none;
      border-left: 1px solid var(--border);
      border-right: 1px solid var(--border);
      background: var(--bg);
    }

    /* Top Sticky Header */
    header {
      background: color-mix(in srgb, var(--surface) 92%, transparent);
      backdrop-filter: blur(16px);
      -webkit-backdrop-filter: blur(16px);
      border-bottom: 1px solid var(--border);
      position: sticky;
      top: 0;
      z-index: 50;
      padding: 10px 14px 8px;
    }

    .header-main {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 8px;
    }

    .brand-wrap {
      display: flex;
      align-items: center;
      gap: 8px;
    }

    .brand-icon {
      width: 28px;
      height: 28px;
      border-radius: var(--radius-sm);
      background: var(--primary-subtle);
      color: var(--primary-text);
      display: inline-flex;
      align-items: center;
      justify-content: center;
      flex-shrink: 0;
    }

    .brand-title {
      font-size: 15px;
      font-weight: 700;
      letter-spacing: -0.02em;
      color: var(--fg);
    }

    .brand-pill {
      font-size: 10.5px;
      font-weight: 600;
      color: var(--fg-muted);
      background: var(--surface-subtle);
      border: 1px solid var(--border);
      padding: 2px 6px;
      border-radius: 999px;
      font-variant-numeric: tabular-nums;
    }

    /* Status Badge */
    .badge {
      display: inline-flex;
      align-items: center;
      gap: 5px;
      font-size: 11px;
      font-weight: 600;
      padding: 3px 8px;
      border-radius: 999px;
      white-space: nowrap;
      flex-shrink: 0;
    }
    .status-dot {
      width: 6px;
      height: 6px;
      border-radius: 50%;
    }
    .badge.pass {
      background: var(--success-subtle);
      color: var(--success-text);
    }
    .badge.pass .status-dot { background: var(--success); }

    .badge.warn {
      background: var(--warning-subtle);
      color: var(--warning-text);
    }
    .badge.warn .status-dot { background: var(--warning); }

    .badge.fail {
      background: var(--danger-subtle);
      color: var(--danger-text);
    }
    .badge.fail .status-dot { background: var(--danger); }

    .badge.neutral {
      background: var(--surface-subtle);
      color: var(--fg-muted);
      border: 1px solid var(--border);
    }
    .badge.neutral .status-dot { background: var(--fg-subtle); }

    .badge.info {
      background: var(--primary-subtle);
      color: var(--primary-text);
      border: 1px solid var(--primary);
    }
    .badge.info .status-dot { background: var(--primary); }

    /* Display rows: one column, equal height */
    .display-select-grid {
      display: grid;
      grid-template-columns: 1fr;
      gap: 8px;
    }
    .display-option-card {
      box-sizing: border-box;
      height: 76px;
      border: 1px solid var(--border);
      background: var(--surface-subtle);
      border-radius: var(--radius-sm);
      padding: 8px 10px;
      cursor: pointer;
      display: flex;
      flex-direction: column;
      gap: 2px;
      overflow: hidden;
      user-select: none;
    }
    .display-option-card.selected {
      border-color: var(--primary);
      background: var(--primary-subtle);
      box-shadow: 0 0 0 1px var(--primary);
    }
    .disp-row {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 8px;
      min-height: 18px;
    }
    .disp-title {
      font-size: 13px;
      font-weight: 700;
      color: var(--fg);
      white-space: nowrap;
      overflow: hidden;
      text-overflow: ellipsis;
    }
    .disp-now {
      flex-shrink: 0;
      font-size: 11px;
      font-weight: 700;
      color: var(--success-text);
      background: var(--success-subtle);
      border-radius: 999px;
      padding: 1px 8px;
    }
    .disp-sub {
      font-size: 11px;
      color: var(--fg-muted);
      line-height: 1.2;
      white-space: nowrap;
    }
    .disp-desc {
      font-size: 11.5px;
      color: var(--fg-muted);
      line-height: 1.25;
      display: -webkit-box;
      -webkit-line-clamp: 2;
      -webkit-box-orient: vertical;
      overflow: hidden;
    }
    .action-line {
      min-height: 20px;
      margin: 10px 0 0;
      font-size: 13px;
      line-height: 1.35;
      color: var(--fg-muted);
    }
    .action-line.ok { color: var(--success-text); }
    .action-line.err { color: var(--danger-text); font-weight: 600; }

    /* Telemetry Horizontal Ribbon */
    .telemetry-bar {
      display: flex;
      gap: 6px;
      overflow-x: auto;
      scrollbar-width: none;
      padding-bottom: 2px;
    }
    .telemetry-bar::-webkit-scrollbar { display: none; }

    .stat-chip {
      background: var(--surface);
      border: 1px solid var(--border);
      border-radius: var(--radius-sm);
      padding: 4px 8px;
      display: flex;
      align-items: center;
      gap: 5px;
      font-size: 11.5px;
      white-space: nowrap;
      flex-shrink: 0;
      box-shadow: var(--shadow-sm);
    }
    .stat-chip svg {
      color: var(--fg-muted);
      flex-shrink: 0;
    }
    .stat-chip span {
      color: var(--fg-muted);
      font-size: 11px;
    }
    .stat-chip strong {
      color: var(--fg);
      font-weight: 700;
      font-variant-numeric: tabular-nums;
    }

    /* iOS Segmented Navigation */
    .nav-wrapper {
      padding: 8px 14px 6px;
      max-width: 600px;
      margin: 0 auto;
      width: 100%;
    }
    .segmented-control {
      background: var(--surface-subtle);
      border: 1px solid var(--border);
      border-radius: var(--radius-md);
      padding: 3px;
      display: grid;
      grid-template-columns: repeat(4, minmax(0, 1fr));
      gap: 3px;
    }
    .segment-btn {
      background: transparent;
      border: none;
      padding: 8px 4px;
      font-size: 11px;
      font-weight: 600;
      color: var(--fg-muted);
      border-radius: var(--radius-sm);
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 4px;
      transition: all 0.15s ease;
      white-space: nowrap;
      min-height: 36px;
    }
    .segment-btn svg {
      width: 14px;
      height: 14px;
      flex-shrink: 0;
    }
    .segment-btn.active {
      background: var(--surface);
      color: var(--fg);
      box-shadow: var(--shadow-sm);
      font-weight: 700;
    }

    /* Main Content Panels */
    main {
      flex: 1;
      padding: 6px 14px 14px;
      max-width: 600px;
      margin: 0 auto;
      width: 100%;
    }
    .tab-panel {
      display: none;
      flex-direction: column;
      gap: 12px;
    }
    .tab-panel.active {
      display: flex;
    }

    /* Lean White Cards */
    .card {
      background: var(--surface);
      border: 1px solid var(--border);
      border-radius: var(--radius-md);
      padding: 14px;
      box-shadow: var(--shadow-sm);
    }
    .card-header {
      display: flex;
      align-items: center;
      justify-content: space-between;
      margin-bottom: 12px;
    }
    .card-title {
      font-size: 13px;
      font-weight: 700;
      color: var(--fg);
      letter-spacing: -0.01em;
      display: flex;
      align-items: center;
      gap: 6px;
      text-wrap: balance;
    }
    .card-title svg {
      color: var(--primary);
      width: 16px;
      height: 16px;
      flex-shrink: 0;
    }

    /* Key-Value Metric Rows */
    .metric-row {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 8px 0;
      border-bottom: 1px solid var(--border);
      font-size: 13px;
    }
    .metric-row:last-child {
      border-bottom: none;
      padding-bottom: 0;
    }
    .metric-key {
      color: var(--fg-muted);
      display: flex;
      align-items: center;
      gap: 6px;
    }
    .metric-key svg {
      width: 14px;
      height: 14px;
      color: var(--fg-subtle);
      flex-shrink: 0;
    }
    .metric-val {
      font-weight: 700;
      color: var(--fg);
      font-variant-numeric: tabular-nums;
      text-align: right;
      max-width: 64%;
    }

    /* Action Buttons & Grid */
    .btn {
      min-height: 44px;
      padding: 8px 14px;
      border-radius: var(--radius-sm);
      font-size: 13px;
      font-weight: 600;
      cursor: pointer;
      display: inline-flex;
      align-items: center;
      justify-content: center;
      gap: 6px;
      transition: all 0.12s ease;
      border: 1px solid var(--border);
      background: var(--surface);
      color: var(--fg);
    }
    .btn:active {
      transform: scale(0.98);
      background: var(--surface-active);
    }
    .btn:disabled {
      opacity: 0.5;
      cursor: default;
    }
    .btn-primary {
      background: var(--primary);
      border-color: var(--primary);
      color: var(--surface);
    }
    .btn-primary:active {
      background: var(--primary-hover);
      border-color: var(--primary-hover);
    }
    .btn-danger {
      background: var(--danger);
      border-color: var(--danger);
      color: var(--surface);
    }
    .btn-danger:active {
      background: var(--danger-hover);
      border-color: var(--danger-hover);
    }
    .btn-subtle {
      background: var(--surface-subtle);
      border-color: var(--border);
      color: var(--fg);
    }
    .btn-subtle:active {
      background: var(--surface-active);
    }
    .btn-grid-2 {
      display: grid;
      grid-template-columns: repeat(2, minmax(0, 1fr));
      gap: 8px;
    }
    #card-parts .btn-grid-2 > .btn,
    #card-display-settings .btn-grid-2 > .btn {
      width: 100%;
      height: 44px;
      min-height: 44px;
      padding: 0 8px;
    }
    .btn-grid-3 {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 8px;
    }

    /* Minimalist Precision D-Pad */
    .dpad-frame {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 8px;
      max-width: 230px;
      margin: 4px auto 14px;
      width: 100%;
    }
    .dpad-btn {
      height: 54px;
      border-radius: var(--radius-md);
      border: 1px solid var(--border);
      background: var(--surface);
      color: var(--fg);
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      box-shadow: var(--shadow-sm);
      transition: all 0.1s ease;
      touch-action: manipulation;
    }
    .dpad-btn:active {
      background: var(--primary-subtle);
      border-color: var(--primary);
      color: var(--primary);
      transform: scale(0.96);
    }
    .dpad-btn.stop {
      background: var(--danger-subtle);
      border-color: var(--danger);
      color: var(--danger-text);
    }
    .dpad-btn.stop:active {
      background: var(--danger);
      color: var(--surface);
    }

    /* Emergency Stop Full Bar */
    .stop-bar {
      width: 100%;
      min-height: 46px;
      border-radius: var(--radius-sm);
      background: var(--danger);
      border: 1px solid var(--danger);
      color: var(--surface);
      font-weight: 700;
      font-size: 14px;
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 6px;
      box-shadow: var(--shadow-sm);
      transition: transform 0.08s ease;
    }
    .stop-bar:active {
      transform: scale(0.98);
      background: var(--danger-hover);
    }

    /* Form Controls */
    .control-row {
      display: flex;
      flex-direction: column;
      gap: 6px;
      margin-top: 10px;
    }
    .control-label {
      font-size: 11px;
      font-weight: 600;
      color: var(--fg-muted);
    }
    select, input[type="range"] {
      width: 100%;
      min-height: 42px;
      padding: 6px 10px;
      border-radius: var(--radius-sm);
      border: 1px solid var(--border);
      background: var(--surface-subtle);
      color: var(--fg);
      font-size: 13.5px;
      outline: none;
    }
    select:focus {
      border-color: var(--primary);
      background: var(--surface);
    }

    /* Sensor Meter Bars */
    .meter-item {
      display: flex;
      align-items: center;
      gap: 8px;
      margin-top: 8px;
    }
    .meter-label {
      font-size: 11px;
      font-weight: 600;
      color: var(--fg-muted);
      width: 60px;
      flex-shrink: 0;
    }
    .meter-track {
      flex: 1;
      height: 10px;
      background: var(--surface-subtle);
      border-radius: 999px;
      overflow: hidden;
      border: 1px solid var(--border);
    }
    .meter-fill {
      height: 100%;
      width: 15%;
      background: var(--fg-subtle);
      border-radius: 999px;
      transition: width 0.15s ease, background 0.15s ease;
    }
    .meter-val {
      font-size: 12px;
      font-weight: 700;
      font-variant-numeric: tabular-nums;
      width: 55px;
      text-align: right;
      color: var(--fg);
    }

    /* Self-Test Report Container */
    .report-box {
      margin-top: 10px;
      display: flex;
      flex-direction: column;
      gap: 6px;
    }
    .report-card {
      border: 1px solid var(--border);
      background: var(--surface-subtle);
      border-radius: var(--radius-sm);
      padding: 10px 12px;
    }
    .report-header {
      display: flex;
      align-items: center;
      justify-content: space-between;
      font-size: 12.5px;
      font-weight: 600;
      color: var(--fg);
      margin-bottom: 4px;
    }
    .report-desc {
      font-size: 11.5px;
      color: var(--fg-muted);
      line-height: 1.4;
    }

    /* Terminal Console */
    .terminal-box {
      background: var(--surface-subtle);
      border: 1px solid var(--border);
      border-radius: var(--radius-sm);
      font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, monospace;
      font-size: 11px;
      padding: 10px;
      max-height: 260px;
      overflow-y: auto;
      display: flex;
      flex-direction: column;
      gap: 4px;
      color: var(--fg);
    }
    .terminal-line {
      display: flex;
      align-items: flex-start;
      gap: 6px;
      line-height: 1.4;
      word-break: break-word;
    }
    .term-ts {
      color: var(--fg-subtle);
      flex-shrink: 0;
    }
    .term-ok { color: var(--success-text); }
    .term-warn { color: var(--warning-text); }
    .term-err { color: var(--danger-text); }
  </style>
</head>
<body>

  <header>
    <div class="header-main">
      <div class="brand-wrap">
        <div class="brand-icon">
          <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8" stroke-linecap="round" stroke-linejoin="round">
            <rect x="4" y="4" width="16" height="16" rx="2"></rect>
            <rect x="9" y="9" width="6" height="6"></rect>
            <path d="M9 1v3M15 1v3M9 20v3M15 20v3M20 9h3M20 14h3M1 9h3M1 14h3"></path>
          </svg>
        </div>
        <div>
          <span class="brand-title">Rody S3</span>
          <span class="brand-pill whitespace-nowrap shrink-0">v0.2.1</span>
        </div>
      </div>
      <div id="conn-badge" class="badge pass whitespace-nowrap shrink-0">
        <span class="status-dot"></span>
        <span id="conn-text">Trực Tuyến</span>
      </div>
      <span id="sta-count" class="brand-pill whitespace-nowrap shrink-0">0 máy</span>
    </div>
  </header>

  <!-- Segmented Navigation (4 Equal Tabs) -->
  <div class="nav-wrapper">
    <div class="segmented-control">
      <button class="segment-btn active" onclick="switchTab('remote')">
        <!-- Gamepad icon -->
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
          <line x1="6" y1="12" x2="10" y2="12"></line>
          <line x1="8" y1="10" x2="8" y2="14"></line>
          <line x1="15" y1="13" x2="15.01" y2="13"></line>
          <line x1="18" y1="11" x2="18.01" y2="11"></line>
          <rect x="2" y="6" width="20" height="12" rx="6"></rect>
        </svg>
        <span>Lái Xe</span>
      </button>
      <button class="segment-btn" onclick="switchTab('diagnostics')">
        <!-- Activity icon -->
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
          <polyline points="22 12 18 12 15 21 9 3 6 12 2 12"></polyline>
        </svg>
        <span>Đo Kiểm</span>
      </button>
      <button class="segment-btn" onclick="switchTab('scenarios')">
        <!-- Layers icon -->
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
          <polygon points="12 2 2 7 12 12 22 7 12 2"></polygon>
          <polyline points="2 17 12 22 22 17"></polyline>
          <polyline points="2 12 12 17 22 12"></polyline>
        </svg>
        <span>Kịch Bản</span>
      </button>
      <button class="segment-btn" onclick="switchTab('terminal')">
        <!-- Terminal icon -->
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
          <polyline points="4 17 10 11 4 5"></polyline>
          <line x1="12" y1="19" x2="20" y2="19"></line>
        </svg>
        <span>Logs</span>
      </button>
    </div>
  </div>

  <main>
    <!-- TAB 1: REMOTE CONTROLLER -->
    <section id="tab-remote" class="tab-panel active" data-od-id="remote-controller">
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
              <circle cx="12" cy="12" r="10"></circle>
              <polygon points="16.24 7.76 14.12 14.12 7.76 16.24 9.88 9.88 16.24 7.76"></polygon>
            </svg>
            <span>Bàn Điều Khiển Hướng</span>
          </div>
          <span id="txt-speed" style="font-size:12px; font-weight:700; color:var(--primary);">50%</span>
        </div>

        <!-- 3x3 Precision Direction Pad -->
        <div class="dpad-frame">
          <div></div>
          <button class="dpad-btn" onpointerdown="startDrive(currentSpeed, currentSpeed)" onpointerup="stopDrive()">
            <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2"><polyline points="18 15 12 9 6 15"></polyline></svg>
          </button>
          <div></div>

          <button class="dpad-btn" onpointerdown="startDrive(-currentSpeed*0.7, currentSpeed*0.7)" onpointerup="stopDrive()">
            <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2"><polyline points="15 18 9 12 15 6"></polyline></svg>
          </button>
          <button class="dpad-btn stop" onclick="stopDrive()">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="currentColor"><rect x="4" y="4" width="16" height="16" rx="2"></rect></svg>
          </button>
          <button class="dpad-btn" onpointerdown="startDrive(currentSpeed*0.7, -currentSpeed*0.7)" onpointerup="stopDrive()">
            <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2"><polyline points="9 18 15 12 9 6"></polyline></svg>
          </button>

          <div></div>
          <button class="dpad-btn" onpointerdown="startDrive(-currentSpeed, -currentSpeed)" onpointerup="stopDrive()">
            <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2"><polyline points="6 9 12 15 18 9"></polyline></svg>
          </button>
          <div></div>
        </div>

        <button class="stop-bar" onclick="stopDrive()">
          <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor"><rect x="4" y="4" width="16" height="16" rx="2"></rect></svg>
          <span>Dừng Khẩn Cấp (Stop)</span>
        </button>

        <div class="control-row">
          <label class="control-label" for="rng-speed">Tốc độ điều khiển:</label>
          <input type="range" id="rng-speed" min="20" max="100" value="50" oninput="updateSpeedVal(this.value)">
        </div>

        <div class="control-row">
          <label class="control-label" for="sel-mode">Chế độ vận hành robot:</label>
          <select id="sel-mode" onchange="setMode(this.value)">
            <option value="manual">Thủ công (Manual)</option>
            <option value="avoid">Tự hành Tránh Vật Cản (Avoid)</option>
            <option value="line">Tự hành Dò Đường (Line)</option>
          </select>
        </div>
      </div>

      <div class="card" id="card-parts">
        <div class="card-header">
          <div class="card-title"><span>Thử linh kiện</span></div>
        </div>
        <p style="font-size:12px; color:var(--fg-muted); margin:0 0 10px;">Kê bánh lên khỏi mặt bàn trước khi nhích.</p>
        <div class="btn-grid-2">
          <button class="btn btn-subtle" onclick="testEmotion('idle','Bình thường')">Bình thường</button>
          <button class="btn btn-subtle" onclick="testEmotion('happy','Vui')">Vui</button>
          <button class="btn btn-subtle" onclick="testEmotion('listen','Nghe')">Nghe</button>
          <button class="btn btn-subtle" onclick="testEmotion('think','Nghĩ')">Nghĩ</button>
          <button class="btn btn-subtle" onclick="testEmotion('speak','Nói')">Nói</button>
          <button class="btn btn-subtle" onclick="testEmotion('sleep','Ngủ')">Ngủ</button>
          <button class="btn btn-subtle" onclick="testBuzzer()">Loa bíp</button>
          <button class="btn btn-subtle" onclick="testUltrasonic()">Đo khoảng cách</button>
          <button class="btn btn-subtle" onclick="testMotor('left', 40, 500)">Nhích trái</button>
          <button class="btn btn-subtle" onclick="testMotor('right', 40, 500)">Nhích phải</button>
          <button class="btn btn-subtle" onclick="testMotor('both', 40, 1000)">Cả hai</button>
        </div>
        <p id="part-status" class="action-line"></p>
        <div class="metric-row">
          <span class="metric-key">IR trái</span>
          <span class="metric-val" id="drive-ir-l">—</span>
        </div>
        <div class="metric-row">
          <span class="metric-key">IR phải</span>
          <span class="metric-val" id="drive-ir-r">—</span>
        </div>
      </div>
    </section>

    <!-- TAB 2: HARDWARE DIAGNOSTICS -->
    <section id="tab-diagnostics" class="tab-panel" data-od-id="hardware-diagnostics">
      <!-- Power & Thermal Card -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
              <polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"></polygon>
            </svg>
            <span>Nguồn Điện & Nhiệt Độ Chip</span>
          </div>
          <button class="btn btn-subtle" style="min-height:30px; padding:4px 8px; font-size:11.5px;" onclick="refreshTelemetry()">Cập Nhật</button>
        </div>
        <div class="metric-row">
          <span class="metric-key">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8"><path d="M12 2v20M17 5H9.5a3.5 3.5 0 0 0 0 7h5a3.5 3.5 0 0 1 0 7H6"></path></svg>
            Điện áp đo được:
          </span>
          <span class="metric-val" id="diag-v">0.00 V</span>
        </div>
        <div class="metric-row">
          <span class="metric-key">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8"><path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z"></path></svg>
            Trạng thái nguồn:
          </span>
          <span id="diag-pwr-status" class="badge pass whitespace-nowrap shrink-0">USB Cắm Máy Tính</span>
        </div>
        <div class="metric-row">
          <span class="metric-key">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"></path></svg>
            Nhiệt độ lõi CPU:
          </span>
          <span class="metric-val" id="diag-temp">0.0 °C</span>
        </div>
        <div class="metric-row">
          <span class="metric-key">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8"><rect x="2" y="2" width="20" height="8" rx="2"></rect><rect x="2" y="14" width="20" height="8" rx="2"></rect><line x1="6" y1="6" x2="6.01" y2="6"></line><line x1="6" y1="18" x2="6.01" y2="18"></line></svg>
            Bộ nhớ RAM khả dụng:
          </span>
          <span class="metric-val" id="diag-heap">0 KB</span>
        </div>
        <div class="metric-row">
          <span class="metric-key">Thời gian chạy:</span>
          <span class="metric-val" id="diag-uptime">00:00</span>
        </div>
      </div>

      <div class="card" id="card-links">
        <div class="card-header">
          <div class="card-title">
            <span>Trạng Thái Kết Nối</span>
          </div>
        </div>
        <p style="font-size:12px; color:var(--fg-muted); margin-bottom:8px;">
          Wi-Fi AP, chip đã biết trên I2C, IMU và màn hình. Làm mới mỗi 2 giây. Không quét full bus.
        </p>
        <div id="link-box" style="font-size:12.5px;">Đang đọc...</div>
      </div>

      <div class="card" id="card-pins">
        <div class="card-header">
          <div class="card-title">
            <span>Chân Pin</span>
          </div>
        </div>
        <p style="font-size:12px; color:var(--fg-muted); margin-bottom:8px;">
          Chỉ đọc. Chân bus (I2C, I2S, SPI, LED) không bị đổi mode. IR là IO10 và IO11.
        </p>
        <div id="pin-box" style="font-size:12.5px;">Đang đọc...</div>
      </div>

      <!-- I2C Bus Scanner -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
              <circle cx="11" cy="11" r="8"></circle>
              <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
            </svg>
            <span>Quét Mạng Ngoại Vi I2C</span>
          </div>
          <button class="btn btn-primary" style="min-height:30px; padding:4px 10px; font-size:11.5px;" onclick="scanI2C()">Quét Ngay</button>
        </div>
        <p style="font-size:12px; color:var(--fg-muted); margin-bottom:8px;">
          Chân SDA = IO8, SCL = IO9. Tự động nhận diện PCA9685, MPU6050/6500/9250, AK8963, SSD1306.
        </p>
        <div id="i2c-result" style="background:var(--surface-subtle); border:1px solid var(--border); border-radius:var(--radius-sm); padding:10px; font-size:12px;">
          Chưa quét. Bấm nút Quét Ngay để tìm linh kiện cắm trên bus.
        </div>
      </div>

      <!-- Motor Bench Test -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
              <circle cx="12" cy="12" r="3"></circle>
              <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path>
            </svg>
            <span>Nhích Thử Động Cơ Bánh Xe</span>
          </div>
        </div>
        <p style="font-size:12px; color:var(--fg-muted); margin-bottom:10px;">
          Bánh trái PCA9685 CH0, bánh phải CH1. Xung tự ngắt. Kê bánh lên trước khi nhích.
        </p>
        <div class="btn-grid-3">
          <button class="btn btn-subtle" onclick="testMotor('left', 40, 500)">Nhích Trái (0.5s)</button>
          <button class="btn btn-subtle" onclick="testMotor('right', 40, 500)">Nhích Phải (0.5s)</button>
          <button class="btn btn-subtle" onclick="testMotor('both', 40, 1000)">Chạy Cả 2 (1.0s)</button>
        </div>
      </div>

      <!-- IR Line Tracking Sensors -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
              <path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"></path>
              <circle cx="12" cy="12" r="3"></circle>
            </svg>
            <span>Mắt Cảm Biến Hồng Ngoại (IR)</span>
          </div>
        </div>
        <div class="meter-item">
          <span class="meter-label">Trái (IO10):</span>
          <div class="meter-track"><div id="meter-ir-l" class="meter-fill"></div></div>
          <span class="meter-val" id="val-ir-l">XA (1)</span>
        </div>
        <div class="meter-item">
          <span class="meter-label">Phải (IO11):</span>
          <div class="meter-track"><div id="meter-ir-r" class="meter-fill"></div></div>
          <span class="meter-val" id="val-ir-r">XA (1)</span>
        </div>
        <p style="font-size:11.5px; color:var(--fg-muted); margin-top:8px;">
          Đưa tay lại gần mắt đọc: thanh bar sẽ nhảy và giá trị chuyển về GẦN (0).
        </p>
      </div>

      <!-- Ultrasonic & Buzzer Card -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
              <path d="M4.9 19.1C1 15.2 1 8.8 4.9 4.9M7.8 16.2c-2.3-2.3-2.3-6.1 0-8.5M12 12h.01M16.2 7.8c2.3 2.3 2.3 6.1 0 8.5M19.1 4.9c3.9 3.9 3.9 10.3 0 14.2"></path>
            </svg>
            <span>Cảm Biến Siêu Âm & Còi Báo</span>
          </div>
          <button class="btn btn-subtle" style="min-height:30px; padding:4px 10px; font-size:11.5px;" onclick="testUltrasonic()">Đo Thử</button>
        </div>
        <div class="metric-row" style="border-bottom:none; margin-bottom:10px;">
          <span class="metric-key">Khoảng cách vật cản:</span>
          <span class="metric-val" style="font-size:18px; color:var(--primary);" id="us-dist">-- cm</span>
        </div>
        <div class="btn-grid-2">
          <button class="btn btn-subtle" onclick="testBuzzer()">
            <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8"><path d="M18 8A6 6 0 0 0 6 8c0 7-3 9-3 9h18s-3-2-3-9"></path><path d="M13.73 21a2 2 0 0 1-3.46 0"></path></svg>
            Kêu Bíp Loa I2S
          </button>
          <button class="btn btn-subtle" onclick="testEmotion('happy')">
            <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8"><circle cx="12" cy="12" r="10"></circle><path d="M8 14s1.5 2 4 2 4-2 4-2"></path><line x1="9" y1="9" x2="9.01" y2="9"></line><line x1="15" y1="9" x2="15.01" y2="9"></line></svg>
            Mặt Vui Vẻ
          </button>
        </div>
      </div>

      <div class="card" id="card-display-settings" data-od-id="display-settings-card">
        <div class="card-header">
          <div class="card-title"><span>Màn hình</span></div>
          <span class="badge info whitespace-nowrap shrink-0" id="disp-active-badge">Đang tải</span>
        </div>
        <p style="font-size:12px; color:var(--fg-muted); margin:0 0 10px;">Chạm một dòng để chọn. Lưu ghi vào bộ nhớ rồi khởi động lại.</p>
        <div class="display-select-grid" id="display-options-container"></div>
        <div class="btn-grid-2" style="margin-top:12px;">
          <button class="btn btn-subtle" onclick="scanOledUi()">Quét</button>
          <button class="btn btn-subtle" onclick="testDisplayPatternUI()">Thử hình</button>
          <button class="btn btn-subtle" onclick="loadDisplayConfig(true)">Bỏ chọn</button>
          <button class="btn btn-primary" id="btn-save-display" onclick="applyDisplayAndReset()">Lưu</button>
        </div>
        <p id="disp-action-line" class="action-line">Đang đọc cấu hình màn hình...</p>
      </div>
    </section>

    <!-- TAB 3: AUTOMATED SCENARIOS -->
    <section id="tab-scenarios" class="tab-panel" data-od-id="automated-scenarios">
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
              <path d="M22 11.08V12a10 10 0 1 1-5.93-9.14"></path>
              <polyline points="22 4 12 14.01 9 11.01"></polyline>
            </svg>
            <span>Kịch Bản Tự Kiểm Tra Toàn Diện</span>
          </div>
        </div>
        <p style="font-size:12.5px; color:var(--fg-muted); margin-bottom:12px; line-height:1.5;">
          Kiểm tra tự động tuần tự: Vi điều khiển CPU & RAM ➔ Cảm biến nhiệt độ ➔ Nguồn điện & rủi ro Brownout ➔ Bus I2C ➔ Mắt dò hồng ngoại ➔ Còi báo Buzzer.
        </p>
        <button class="btn btn-primary" style="width:100%; min-height:46px; font-size:13.5px;" onclick="runSelfTest()">
          <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"></polygon></svg>
          Kiểm Tra Hệ Thống (5s)
        </button>
        <div id="selftest-results" class="report-box"></div>
      </div>
    </section>

    <!-- TAB 4: REALTIME LOGS -->
    <section id="tab-terminal" class="tab-panel" data-od-id="realtime-logs">
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8">
              <polyline points="4 17 10 11 4 5"></polyline>
              <line x1="12" y1="19" x2="20" y2="19"></line>
            </svg>
            <span>Nhật Ký Hoạt Động (Console Logs)</span>
          </div>
          <button class="btn btn-subtle" style="min-height:30px; padding:4px 8px; font-size:11.5px;" onclick="clearLogs()">Xóa Log</button>
        </div>
        <div id="term-logs" class="terminal-box">
          <div class="terminal-line"><span class="term-ts">[00:00]</span> <span>Khởi động console chẩn đoán Rody S3.</span></div>
        </div>
      </div>
    </section>
  </main>

  <script>
    let currentSpeed = 50;
    let driveInterval = null;

    function log(msg, type = 'info') {
      const box = document.getElementById('term-logs');
      if (!box) return;
      const now = new Date();
      const ts = `[${String(now.getMinutes()).padStart(2,'0')}:${String(now.getSeconds()).padStart(2,'0')}]`;
      const line = document.createElement('div');
      line.className = 'terminal-line';
      const cls = type === 'ok' ? 'term-ok' : (type === 'warn' ? 'term-warn' : (type === 'err' ? 'term-err' : ''));
      line.innerHTML = `<span class="term-ts">${ts}</span> <span class="${cls}">${msg}</span>`;
      box.appendChild(line);
      box.scrollTop = box.scrollHeight;
    }

    function clearLogs() {
      const box = document.getElementById('term-logs');
      if (box) box.innerHTML = '';
      log("Đã dọn sạch nhật ký.");
    }

    function switchTab(tabId) {
      document.querySelectorAll('.tab-panel').forEach(p => p.classList.remove('active'));
      document.querySelectorAll('.segment-btn').forEach(b => b.classList.remove('active'));

      const target = document.getElementById(`tab-${tabId}`);
      if (target) target.classList.add('active');

      const tabs = ['remote', 'diagnostics', 'scenarios', 'terminal'];
      const idx = tabs.indexOf(tabId);
      if (idx >= 0) {
        document.querySelectorAll('.segment-btn')[idx].classList.add('active');
      }
    }

    function updateSpeedVal(val) {
      currentSpeed = parseInt(val);
      document.getElementById('txt-speed').textContent = `${currentSpeed}%`;
    }

    function sendDriveCmd(l, r) {
      fetch(`/drive?l=${l.toFixed(1)}&r=${r.toFixed(1)}`)
        .catch(err => log(`Lỗi gửi lái: ${err}`, 'err'));
    }

    function startDrive(l, r) {
      sendDriveCmd(l, r);
      if (driveInterval) clearInterval(driveInterval);
      driveInterval = setInterval(() => sendDriveCmd(l, r), 350);
    }

    function stopDrive() {
      if (driveInterval) {
        clearInterval(driveInterval);
        driveInterval = null;
      }
      fetch('/stop')
        .then(() => log("Đã gửi lệnh dừng robot (STOP)", 'ok'))
        .catch(err => log(`Lỗi STOP: ${err}`, 'err'));
    }

    function setMode(m) {
      fetch(`/mode?m=${m}`)
        .then(r => r.text())
        .then(() => log(`Đã đổi chế độ sang: ${m}`, 'ok'))
        .catch(err => log(`Lỗi đổi chế độ: ${err}`, 'err'));
    }

    function refreshTelemetry() {
      fetch('/status')
        .then(r => r.json())
        .then(data => {
          const badge = document.getElementById('conn-badge');
          badge.className = 'badge pass whitespace-nowrap shrink-0';
          document.getElementById('conn-text').textContent = 'Trực Tuyến';

          const mins = Math.floor(data.uptime / 60);
          const secs = data.uptime % 60;
          const uptime = document.getElementById('diag-uptime');
          if (uptime) uptime.textContent = `${String(mins).padStart(2,'0')}:${String(secs).padStart(2,'0')}`;

          document.getElementById('diag-v').textContent = `${data.v.toFixed(2)} V`;
          document.getElementById('diag-temp').textContent = `${data.temp.toFixed(1)} °C`;
          document.getElementById('diag-heap').textContent = `${Math.round(data.heap / 1024)} KB (${data.heap} bytes)`;

          const pwrBadge = document.getElementById('diag-pwr-status');
          if (data.v < 1.0) {
            pwrBadge.className = 'badge pass whitespace-nowrap shrink-0';
            pwrBadge.innerHTML = '<span class="status-dot"></span> USB Cắm Máy Tính (Bypass)';
          } else if (data.v >= 3.6 && data.v <= 4.25) {
            pwrBadge.className = 'badge pass whitespace-nowrap shrink-0';
            pwrBadge.innerHTML = `<span class="status-dot"></span> Pin 1S Khỏe (${Math.round((data.v - 3.4)/0.8 * 100)}%)`;
          } else if (data.v < 3.6) {
            pwrBadge.className = 'badge warn whitespace-nowrap shrink-0';
            pwrBadge.innerHTML = '<span class="status-dot"></span> Pin Yếu - Cần Sạc';
          } else {
            pwrBadge.className = 'badge fail whitespace-nowrap shrink-0';
            pwrBadge.innerHTML = '<span class="status-dot"></span> Quá Áp (>4.25V)';
          }

          if (data.ir_l !== undefined) {
            document.getElementById('meter-ir-l').style.width = data.ir_l === 0 ? '100%' : '15%';
            document.getElementById('meter-ir-l').style.background = data.ir_l === 0 ? 'var(--success)' : 'var(--fg-subtle)';
            document.getElementById('val-ir-l').textContent = data.ir_l === 0 ? 'GẦN (0)' : 'XA (1)';
          }
          if (data.ir_r !== undefined) {
            document.getElementById('meter-ir-r').style.width = data.ir_r === 0 ? '100%' : '15%';
            document.getElementById('meter-ir-r').style.background = data.ir_r === 0 ? 'var(--success)' : 'var(--fg-subtle)';
            document.getElementById('val-ir-r').textContent = data.ir_r === 0 ? 'GẦN (0)' : 'XA (1)';
          }
          const driveL = document.getElementById('drive-ir-l');
          const driveR = document.getElementById('drive-ir-r');
          if (driveL && data.ir_l !== undefined) driveL.textContent = data.ir_l === 0 ? 'GẦN' : 'XA';
          if (driveR && data.ir_r !== undefined) driveR.textContent = data.ir_r === 0 ? 'GẦN' : 'XA';

          document.getElementById('sel-mode').value = data.mode;
        })
        .catch(err => {
          const badge = document.getElementById('conn-badge');
          badge.className = 'badge fail whitespace-nowrap shrink-0';
          document.getElementById('conn-text').textContent = 'Mất Kết Nối';
        });
    }

    function scanI2C() {
      log("Bắt đầu quét bus I2C (SDA=IO8, SCL=IO9)...");
      const box = document.getElementById('i2c-result');
      box.innerHTML = '<i>Đang quét...</i>';
      fetch('/scan_i2c')
        .then(r => r.json())
        .then(data => {
          if (data.devices.length === 0) {
            box.innerHTML = '<span class="badge neutral whitespace-nowrap shrink-0"><span class="status-dot"></span> 0 thiết bị</span> Chưa tìm thấy linh kiện I2C nào cắm trên bus. (Bình thường nếu chưa cắm PCA9685/IMU)';
            log("I2C Scan: 0 thiết bị tìm thấy.", 'warn');
          } else {
            let html = `<div style="margin-bottom:6px;"><span class="badge pass whitespace-nowrap shrink-0"><span class="status-dot"></span> ${data.devices.length} Thiết Bị Đã Nhận Diện:</span></div>`;
            data.devices.forEach((addr, idx) => {
              const name = data.names[idx] || 'Thiết bị không rõ';
              html += `<div style="padding:4px 0;">• <b>0x${addr.toString(16).toUpperCase()} (${addr})</b>: <span style="color:var(--primary); font-weight:600;">${name}</span></div>`;
            });
            box.innerHTML = html;
            log(`I2C Scan tìm thấy: ${data.devices.map(a=>'0x'+a.toString(16)).join(', ')}`, 'ok');
          }
        })
        .catch(err => {
          box.innerHTML = `<span class="badge fail whitespace-nowrap shrink-0"><span class="status-dot"></span> Lỗi quét I2C: ${err}</span>`;
          log(`Lỗi quét I2C: ${err}`, 'err');
        });
    }

    function setPartStatus(text, bad) {
      const el = document.getElementById('part-status');
      if (!el) return;
      el.textContent = text;
      el.className = 'action-line' + (bad ? ' err' : ' ok');
    }

    function testMotor(wheel, speed, ms) {
      const names = { left: 'trái', right: 'phải', both: 'cả hai' };
      const label = names[wheel] || wheel;
      log(`Test động cơ: Bánh=${wheel}, Tốc độ=${speed}%, Thời gian=${ms}ms`);
      fetch(`/test_motor?wheel=${wheel}&speed=${speed}&ms=${ms}`)
        .then(r => r.json())
        .then(data => {
          if (data.ok) {
            setPartStatus(`Đã nhích bánh ${label}`, false);
            log(`Động cơ ${wheel} nhích thành công`, 'ok');
          } else {
            setPartStatus(`Không nhích được: ${data.err || 'lỗi'}`, true);
            log(`Động cơ ${wheel} lỗi: ${data.err}`, 'warn');
          }
        })
        .catch(err => {
          setPartStatus('Không gửi được lệnh nhích', true);
          log(`Lỗi gọi test motor: ${err}`, 'err');
        });
    }

    function testUltrasonic() {
      log("Bắn xung kiểm tra khoảng cách siêu âm...");
      setPartStatus('Đang đo khoảng cách...', false);
      const diag = document.getElementById('us-dist');
      if (diag) diag.textContent = 'Đang đo...';
      fetch('/test_us')
        .then(r => r.json())
        .then(data => {
          if (data.ok && data.cm > 0) {
            const text = `Khoảng cách ${data.cm.toFixed(1)} cm`;
            setPartStatus(text, false);
            if (diag) diag.textContent = `${data.cm.toFixed(1)} cm`;
            log(`Khoảng cách đo được: ${data.cm.toFixed(1)} cm`, 'ok');
          } else {
            setPartStatus('Không đo được khoảng cách', true);
            if (diag) diag.textContent = 'Chưa cắm / Xa';
            log("Siêu âm không phản hồi (chưa cắm cảm biến hoặc ngoài tầm)", 'warn');
          }
        })
        .catch(err => {
          setPartStatus('Không gửi được lệnh đo', true);
          log(`Lỗi siêu âm: ${err}`, 'err');
        });
    }

    function testBuzzer() {
      log("Phát âm thanh còi Buzzer (2000Hz, 150ms)");
      fetch('/test_buzzer')
        .then(r => r.json())
        .then(() => {
          setPartStatus('Đã kêu bíp', false);
          log("Còi đã kêu beep", 'ok');
        })
        .catch(err => {
          setPartStatus('Không kêu được loa', true);
          log(`Lỗi còi: ${err}`, 'err');
        });
    }

    function testEmotion(emo, label) {
      const name = label || emo;
      log(`Đổi biểu cảm khuôn mặt AI: ${emo}`);
      fetch(`/test_face?emo=${emo}`)
        .then(r => r.json())
        .then(() => {
          setPartStatus(`Đã đổi mặt: ${name}`, false);
          log(`Đã đổi mặt sang: ${emo}`, 'ok');
        })
        .catch(err => {
          setPartStatus('Không đổi được mặt', true);
          log(`Lỗi đổi mặt: ${err}`, 'err');
        });
    }

    function runSelfTest() {
      const container = document.getElementById('selftest-results');
      container.innerHTML = '<div style="padding:14px; text-align:center; color:var(--fg-muted);"><i>Đang thực hiện kịch bản kiểm tra toàn diện...</i></div>';
      log("Bắt đầu chạy kịch bản kiểm tra toàn hệ thống...", 'ok');

      fetch('/selftest')
        .then(r => r.json())
        .then(data => {
          let html = `<div style="font-weight:700; color:var(--fg); margin:8px 0 4px;">Kết Quả Kiểm Thử:</div>`;
          data.report.forEach(item => {
            const badgeCls = item.status === 'PASS' ? 'pass' : (item.status === 'WARN' ? 'warn' : (item.status === 'FAIL' ? 'fail' : 'neutral'));
            html += `
              <div class="report-card">
                <div class="report-header">
                  <span>${item.name}</span>
                  <span class="badge ${badgeCls} whitespace-nowrap shrink-0"><span class="status-dot"></span> ${item.status}</span>
                </div>
                <div class="report-desc">${item.msg}</div>
              </div>
            `;
            log(`[${item.status}] ${item.name}: ${item.msg}`, item.status === 'PASS' ? 'ok' : 'warn');
          });
          container.innerHTML = html;
          log("Kết thúc kịch bản kiểm tra toàn diện.", 'ok');
        })
        .catch(err => {
          container.innerHTML = `<div class="badge fail whitespace-nowrap shrink-0"><span class="status-dot"></span> Lỗi chạy kịch bản: ${err}</div>`;
          log(`Lỗi kịch bản: ${err}`, 'err');
        });
    }

    let currentDisplayId = 0;
    let selectedDisplayId = 0;

    const DISPLAY_MODELS = [
      { id: 0, name: "1.54\" ST7789 Vuông", res: "240x240 (SPI)", desc: "Mặc định chuẩn, mắt to góc bo nét" },
      { id: 1, name: "1.28\" GC9A01 Tròn", res: "240x240 (SPI)", desc: "Màn tròn kính cong, mắt Mochi co cụm chống lẹm viền" },
      { id: 2, name: "1.8\" ST7735 Chữ Nhật", res: "160x128 (SPI)", desc: "Màn hình chữ nhật nằm ngang tỉ lệ 5:4" },
      { id: 3, name: "0.96\" ST7735 Nhỏ", res: "160x80 (SPI IPS)", desc: "Màn màu IPS 80x160 cắm cổng SPI (7-8 chân)" },
      { id: 4, name: "0.96\" SSD1306 OLED I2C", res: "128x64 (I2C)", desc: "Màn OLED đen trắng cắm cổng I2C (4 chân 0x3C)" },
      { id: 5, name: "0.96\" SSD1306 OLED SPI", res: "128x64 (SPI)", desc: "Màn OLED đen trắng cắm cổng SPI (7 chân)" }
    ];

    function setDispLine(text, kind) {
      const line = document.getElementById('disp-action-line');
      if (!line) return;
      line.textContent = text;
      line.className = 'action-line' + (kind ? ' ' + kind : '');
    }

    function saveErrorText(err) {
      if (err === 'nvs_save_failed') return 'Không ghi được cấu hình vào bộ nhớ.';
      if (err === 'invalid_id') return 'Loại màn hình không hợp lệ.';
      if (err === 'missing_id') return 'Thiếu mã loại màn hình.';
      return err ? `Lỗi lưu: ${err}` : 'Lỗi lưu cấu hình.';
    }

    function renderDisplayCards() {
      const container = document.getElementById('display-options-container');
      if (!container) return;
      let html = '';
      DISPLAY_MODELS.forEach(m => {
        const isSel = (m.id === selectedDisplayId);
        const isCur = (m.id === currentDisplayId);
        html += `
          <div class="display-option-card ${isSel ? 'selected' : ''}" onclick="selectDisplayOption(${m.id})">
            <div class="disp-row">
              <span class="disp-title">${m.name}</span>
              ${isCur ? '<span class="disp-now">Đang dùng</span>' : ''}
            </div>
            <div class="disp-sub">${m.res}</div>
            <div class="disp-desc">${m.desc}</div>
          </div>
        `;
      });
      container.innerHTML = html;
    }

    function selectDisplayOption(id) {
      selectedDisplayId = id;
      renderDisplayCards();
    }

    function loadDisplayConfig(fromDiscard) {
      fetch('/display')
        .then(r => r.json())
        .then(data => {
          if (!data.ok) {
            setDispLine('Không đọc được cấu hình màn hình.', 'err');
            renderDisplayCards();
            return;
          }
          currentDisplayId = data.current_id;
          selectedDisplayId = data.current_id;
          const badge = document.getElementById('disp-active-badge');
          if (badge) badge.textContent = data.source === 'saved' ? 'Đã lưu' : 'Tự chọn';
          renderDisplayCards();
          if (fromDiscard) setDispLine('Đã trả về loại đang chạy.', 'ok');
          else setDispLine(`Đang chạy: ${data.current_name}`, '');
          log(`Màn hình hiện tại: ${data.current_name} (${data.width}x${data.height})`, 'ok');
        })
        .catch(err => {
          renderDisplayCards();
          setDispLine('Không đọc được cấu hình màn hình.', 'err');
          log(`Lỗi đọc cấu hình màn hình: ${err}`, 'err');
        });
    }

    function applyDisplayAndReset() {
      const targetId = selectedDisplayId;
      const model = DISPLAY_MODELS.find(m => m.id === targetId);
      const modelName = model ? model.name : `ID ${targetId}`;
      const btn = document.getElementById('btn-save-display');
      if (btn) btn.disabled = true;
      setDispLine('Đang lưu...', '');
      log(`Đang lưu cấu hình màn hình: ${modelName}...`);

      fetch(`/set_display?id=${targetId}`)
        .then(r => r.json().then(d => ({ status: r.status, d }), () => ({ status: r.status, d: null })))
        .then(({ status, d }) => {
          if (!d) {
            setDispLine('Robot trả về dữ liệu lỗi.', 'err');
            if (btn) btn.disabled = false;
            return;
          }
          if (status >= 400 || !d.ok) {
            setDispLine(saveErrorText(d.err), 'err');
            log(`Lỗi lưu: ${d.err || status}`, 'err');
            if (btn) btn.disabled = false;
            return;
          }
          setDispLine('Đã lưu, robot đang khởi động lại', 'ok');
          log(`Đã lưu ${modelName}. Robot đang khởi động lại.`, 'ok');
          let attempts = 0;
          let sawDrop = false;
          const checkTimer = setInterval(() => {
            attempts++;
            if (attempts >= 15) {
              clearInterval(checkTimer);
              window.location.reload();
              return;
            }
            fetch('/display')
              .then(res => res.json())
              .then(data => {
                if (data.ok && sawDrop) {
                  clearInterval(checkTimer);
                  window.location.reload();
                }
              })
              .catch(() => { sawDrop = true; });
          }, 1000);
        })
        .catch(err => {
          setDispLine('Không gửi được lệnh lưu.', 'err');
          log(`Lỗi kết nối lưu cấu hình: ${err}`, 'err');
          if (btn) btn.disabled = false;
        });
    }

    let lastEventSeq = 0;

    function refreshLinks() {
      fetch('/links')
        .then(r => r.json())
        .then(data => {
          const sta = document.getElementById('sta-count');
          if (sta && data.wifi) sta.textContent = `${data.wifi.clients} máy`;
          const box = document.getElementById('link-box');
          if (!box || !data.wifi) return;
          const imu = data.imu || {};
          const disp = data.display || {};
          const imuText = imu.avail
            ? `${imu.chip} pitch ${Number(imu.pitch).toFixed(1)} roll ${Number(imu.roll).toFixed(1)}`
            : (data.imu_addr ? 'có địa chỉ, driver chưa sẵn' : 'không ACK');
          const rows = [
            ['Wi-Fi AP', `${data.wifi.ssid} · ${data.wifi.ip} · ${data.wifi.clients} máy`],
            ['PCA9685', data.pca ? 'ACK 0x40' : 'không ACK'],
            ['IMU', imuText],
            ['OLED', data.oled_boot ? `ACK lúc boot ${data.oled_addr} SDA=${data.oled_sda} SCL=${data.oled_scl}` : (data.oled_bus ? 'ACK trên I2C IO8/IO9' : 'không ACK')],
            ['Màn hình', `${disp.name || '—'} · ${disp.w}x${disp.h} · ${disp.init_ok ? 'init OK' : 'init chưa OK'} · ${disp.source || ''}`],
            ['Nguồn', `${Number(data.v).toFixed(2)} V · ${Number(data.temp).toFixed(1)} °C · RAM ${Math.round(data.heap / 1024)} KB`]
          ];
          box.innerHTML = rows.map(([k, v]) => `<div class="metric-row"><span class="metric-key">${k}</span><span class="metric-val">${v}</span></div>`).join('');
        })
        .catch(() => {
          const box = document.getElementById('link-box');
          if (box) box.textContent = 'Chưa đọc được trạng thái kết nối.';
        });
    }

    function refreshPins() {
      fetch('/pins')
        .then(r => r.json())
        .then(data => {
          const box = document.getElementById('pin-box');
          if (!box || !data.pins) return;
          box.innerHTML = data.pins.map(p => {
            let val = p.bus;
            if (p.level !== undefined) val = `${p.level} · ${p.bus}`;
            else if (p.mv !== undefined) val = `${(p.mv / 1000).toFixed(2)} V`;
            return `<div class="metric-row"><span class="metric-key">IO${p.gpio} ${p.name}</span><span class="metric-val">${val}<div style="font-weight:500;color:var(--fg-muted);font-size:11px;">${p.note || ''}</div></span></div>`;
          }).join('');
        })
        .catch(() => {});
    }

    function pullEvents() {
      fetch(`/events?since=${lastEventSeq}`)
        .then(r => r.json())
        .then(data => {
          (data.events || []).forEach(ev => {
            if (ev.seq > lastEventSeq) lastEventSeq = ev.seq;
            log(`[fw] ${ev.text}`, 'info');
          });
        })
        .catch(() => {});
    }

    function scanOledUi() {
      setDispLine('Đang quét...', '');
      log('Quét OLED trên header I2C, TFT, IR, siêu âm.');
      fetch('/scan_oled')
        .then(r => r.json())
        .then(data => {
          const text = data.found
            ? `Thấy OLED ${data.addr}, SDA ${data.sda}, SCL ${data.scl}. Chưa đổi loại màn.`
            : 'Không thấy OLED trên các header đã biết.';
          setDispLine(text, data.found ? 'ok' : '');
          log(text, data.found ? 'ok' : 'warn');
        })
        .catch(err => {
          setDispLine('Không quét được.', 'err');
          log(`Lỗi quét OLED: ${err}`, 'err');
        });
    }

    function testDisplayPatternUI() {
      log("Đang gửi lệnh kiểm thử hiển thị (Color Bars & Backlight)...");
      fetch('/test_display')
        .then(r => r.json())
        .then(d => {
          if (d.ok) {
            setDispLine('Đã gửi hình thử.', 'ok');
            log("Đã kích hoạt chu trình kiểm thử màn hình thành công!", 'ok');
          } else {
            setDispLine('Không thử được hình.', 'err');
            log("Không thể thực hiện kiểm thử màn hình.", 'err');
          }
        })
        .catch(err => {
          setDispLine('Không gửi được lệnh thử hình.', 'err');
          log(`Lỗi kết nối kiểm thử: ${err}`, 'err');
        });
    }

    setInterval(refreshTelemetry, 1500);
    setInterval(refreshLinks, 2000);
    setInterval(refreshPins, 1000);
    setInterval(pullEvents, 3000);
    window.addEventListener('DOMContentLoaded', () => {
      refreshTelemetry();
      refreshLinks();
      refreshPins();
      loadDisplayConfig();
      pullEvents();
      log("Hệ thống chẩn đoán Rody S3 đã sẵn sàng.");
    });
  </script>
</body>
</html>
)rawliteral";

static String jsonEscape(const char* raw) {
  String out;
  if (!raw) return out;
  for (const char* p = raw; *p; ++p) {
    if (*p == '\\' || *p == '"') out += '\\';
    out += *p;
  }
  return out;
}

void init() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Rody-S3");
  IPAddress IP = WiFi.softAPIP();
  Serial.printf("# [web] SoftAP 'Rody-S3' started. IP: %s\n", IP.toString().c_str());

  // 1. Root Dashboard Web App
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", INDEX_HTML);
  });

  // 2. Realtime Telemetry & Health Status
  server.on("/status", HTTP_GET, []() {
    float v = sensors::readBatteryVoltage();
    float temp = temperatureRead();
    uint32_t heap = ESP.getFreeHeap();
    uint32_t uptime = millis() / 1000;
    bool cal = store::gCalValid;
    const char* m = behaviors::getModeStr();

    int irL = 1, irR = 1;
    sensors::readLine(irL, irR);

    bool imuAvail = imu_sensor::isAvailable();
    float pitch = 0.0f, roll = 0.0f;
    bool fallen = false, bellyUp = false;
    if (imuAvail) {
      const auto& st = imu_sensor::getState();
      pitch = st.pitch;
      roll = st.roll;
      fallen = st.isFallen;
      bellyUp = st.isBellyUp;
    }

    String json = "{";
    json += "\"v\":" + String(v, 2) + ",";
    json += "\"temp\":" + String(temp, 1) + ",";
    json += "\"heap\":" + String(heap) + ",";
    json += "\"uptime\":" + String(uptime) + ",";
    json += "\"cal\":" + String(cal ? "true" : "false") + ",";
    json += "\"mode\":\"" + String(m) + "\",";
    json += "\"ir_l\":" + String(irL) + ",";
    json += "\"ir_r\":" + String(irR) + ",";
    json += "\"imu\":{";
    json += "\"avail\":" + String(imuAvail ? "true" : "false") + ",";
    json += "\"chip\":\"" + String(imu_sensor::getChipName()) + "\",";
    json += "\"pitch\":" + String(pitch, 1) + ",";
    json += "\"roll\":" + String(roll, 1) + ",";
    json += "\"fallen\":" + String(fallen ? "true" : "false") + ",";
    json += "\"belly_up\":" + String(bellyUp ? "true" : "false");
    json += "}";
    json += "}";
    server.send(200, "application/json", json);
  });

  // 3. I2C Bus Live Scanner
  server.on("/scan_i2c", HTTP_GET, []() {
    std::vector<int> found;
    std::vector<String> names;
    for (uint8_t addr = 1; addr < 127; addr++) {
      Wire.beginTransmission(addr);
      if (Wire.endTransmission() == 0) {
        found.push_back(addr);
        if (addr == 0x40) names.push_back("PCA9685 (16-Ch Servo Driver)");
        else if (addr == 0x68) names.push_back("MPU6050 / GY-6500 / GY-9250 (IMU 6/9-DOF)");
        else if (addr == 0x69) names.push_back("MPU6050 / GY-6500 / GY-9250 (IMU - Chân AD0 = HIGH)");
        else if (addr == 0x0C) names.push_back("AK8963 (La Bàn Số Magnetometer của MPU9250)");
        else if (addr == 0x3C || addr == 0x3D) names.push_back("OLED / Màn Hình I2C");
        else if (addr == 0x70) names.push_back("TCA9548A / PCA9685 All-Call");
        else names.push_back("Linh kiện I2C khác");
      }
    }
    String json = "{\"ok\":true,\"devices\":[";
    for (size_t i = 0; i < found.size(); i++) {
      json += String(found[i]);
      if (i + 1 < found.size()) json += ",";
    }
    json += "],\"names\":[";
    for (size_t i = 0; i < names.size(); i++) {
      json += "\"" + names[i] + "\"";
      if (i + 1 < names.size()) json += ",";
    }
    json += "]}";
    char line[96];
    snprintf(line, sizeof(line), "I2C scan %u thiet bi", (unsigned)found.size());
    debug_log::push(line);
    server.send(200, "application/json", json);
  });

  // 4. Remote Drive Endpoint
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
      server.send(500, "text/plain", err ? err : "Drive Failed");
    }
  });

  // 5. Emergency Stop
  server.on("/stop", HTTP_GET, []() {
    drive::stop();
    webControlActive = false;
    server.send(200, "text/plain", "OK");
  });

  // 6. Mode Switch
  server.on("/mode", HTTP_GET, []() {
    if (server.hasArg("m")) {
      behaviors::setMode(server.arg("m").c_str());
    }
    server.send(200, "text/plain", "OK");
  });

  // 7. Isolated Motor Test (Left / Right / All)
  server.on("/test_motor", HTTP_GET, []() {
    String wheel = server.hasArg("wheel") ? server.arg("wheel") : "both";
    float speed = server.hasArg("speed") ? server.arg("speed").toFloat() : 35.0f;
    uint32_t ms = server.hasArg("ms") ? server.arg("ms").toInt() : 400;
    if (ms > 2000) ms = 2000;

    wheel_cmd::Side side = wheel_cmd::Side::Both;
    if (!wheel_cmd::parse(wheel.c_str(), &side)) {
      server.send(400, "application/json", "{\"ok\":false,\"err\":\"bad_wheel\"}");
      return;
    }

    float spdL = 0.0f, spdR = 0.0f;
    if (side == wheel_cmd::Side::Left || side == wheel_cmd::Side::Both) spdL = speed;
    if (side == wheel_cmd::Side::Right || side == wheel_cmd::Side::Both) spdR = speed;

    const char* err = nullptr;
    bool ok = false;
    if (store::gCalValid) {
      ok = drive::drive(spdL, spdR, ms, err);
    } else {
      // Uncalibrated bench test mode: generate safe pulses around neutral 1500us
      if (side == wheel_cmd::Side::Left || side == wheel_cmd::Side::Both) {
        uint16_t usL = (uint16_t)(1500 + (int)(spdL * 3.5f));
        drive::setPwmRaw(CH_L, usL, ms);
      }
      if (side == wheel_cmd::Side::Right || side == wheel_cmd::Side::Both) {
        // Continuous servo right side is mounted opposite, pulse moves wheel
        uint16_t usR = (uint16_t)(1500 - (int)(spdR * 3.5f));
        drive::setPwmRaw(CH_R, usR, ms);
      }
      ok = true;
    }

    if (ok) {
      server.send(200, "application/json", "{\"ok\":true}");
    } else {
      String json = "{\"ok\":false,\"err\":\"" + String(err ? err : "motor_error") + "\"}";
      server.send(200, "application/json", json);
    }
  });

  // 8. Ultrasonic Distance Probe
  server.on("/test_us", HTTP_GET, []() {
    float cm = sensors::measureDistanceCmOnce();
    String json = "{\"ok\":true,\"cm\":" + String(cm, 1) + "}";
    server.send(200, "application/json", json);
  });

  // 9. Buzzer Audio Test
  server.on("/test_buzzer", HTTP_GET, []() {
    audio_player::playTone(2000.0f, 150);
    server.send(200, "application/json", "{\"ok\":true}");
  });

  // 10. Emotion Face Switch
  server.on("/test_face", HTTP_GET, []() {
    if (server.hasArg("emo")) {
      emotion_gfx::setEmotionByName(server.arg("emo").c_str());
    }
    server.send(200, "application/json", "{\"ok\":true}");
  });

  // 11. Automated Self-Test Diagnostic Suite
  server.on("/selftest", HTTP_GET, []() {
    float v = sensors::readBatteryVoltage();
    float temp = temperatureRead();
    uint32_t heap = ESP.getFreeHeap();

    // Check I2C devices
    bool hasPca = false;
    bool hasMpu = false;
    uint8_t mpuAddr = 0;
    bool hasMag = false;
    for (uint8_t addr = 1; addr < 127; addr++) {
      Wire.beginTransmission(addr);
      if (Wire.endTransmission() == 0) {
        if (addr == 0x40) hasPca = true;
        if (addr == 0x68 || addr == 0x69) {
          hasMpu = true;
          mpuAddr = addr;
        }
        if (addr == 0x0C) hasMag = true;
      }
    }

    // Check IR lines
    int irL = 1, irR = 1;
    sensors::readLine(irL, irR);

    // Beep confirm self test
    audio_player::playTone(2400.0f, 100);

    String json = "{\"ok\":true,\"report\":[";

    // Item 1: CPU & Thermals
    json += "{\"name\":\"1. Vi Xử Lý & Nhiệt Độ Chip\",";
    if (temp < 60.0f) {
      json += "\"status\":\"PASS\",\"msg\":\"Chip ESP32-S3 mát mẻ (" + String(temp, 1) + "°C). RAM tự do: " + String(heap/1024) + "KB dồi dào.\"}";
    } else if (temp < 75.0f) {
      json += "\"status\":\"WARN\",\"msg\":\"Nhiệt độ chip hơi ấm (" + String(temp, 1) + "°C). Đảm bảo thông thoáng vỏ máy.\"}";
    } else {
      json += "\"status\":\"FAIL\",\"msg\":\"CẢNH BÁO QUÁ NHIỆT (" + String(temp, 1) + "°C)! Ngắt nguồn kiểm tra chạm chập ngay.\"}";
    }
    json += ",";

    // Item 2: Power & Voltage
    json += "{\"name\":\"2. Hệ Thống Nguồn Điện\",";
    if (v < 1.0f) {
      json += "\"status\":\"PASS\",\"msg\":\"Đang nuôi qua cáp USB (" + String(v, 2) + "V). Chế độ Auto-Bypass bảo vệ an toàn kích hoạt.\"}";
    } else if (v >= 3.6f && v <= 4.25f) {
      json += "\"status\":\"PASS\",\"msg\":\"Pin Li-ion 1S hoạt động lý tưởng (" + String(v, 2) + "V). Sẵn sàng chạy động cơ.\"}";
    } else if (v < 3.6f) {
      json += "\"status\":\"WARN\",\"msg\":\"Điện áp pin yếu (" + String(v, 2) + "V). Cần cắm sạc TP4056 trước khi test động cơ tải nặng.\"}";
    } else {
      json += "\"status\":\"FAIL\",\"msg\":\"QUÁ ÁP NGUY HIỂM (" + String(v, 2) + "V > 4.25V). Ngắt pin kiểm tra mạch sạc!\t\"}";
    }
    json += ",";

    // Item 3: PCA9685 Servo Driver
    json += "{\"name\":\"3. Mạch Điều Khiển Servo PCA9685\",";
    if (hasPca) {
      json += "\"status\":\"PASS\",\"msg\":\"Phát hiện chip PCA9685 tại địa chỉ I2C 0x40. Sẵn sàng điều khiển servo.\"}";
    } else {
      json += "\"status\":\"WARN\",\"msg\":\"Chưa thấy PCA9685 (0x40). (Bình thường nếu đang test bo trần chưa cắm dây I2C).\"}";
    }
    json += ",";

    // Item 4: IMU Accelerometer / Gyro (MPU6050 / GY-6500 / GY-9250)
    json += "{\"name\":\"4. Cảm Biến Gia Tốc IMU (MPU6050/6500/9250)\",";
    if (imu_sensor::isAvailable()) {
      String msg = "Phát hiện " + String(imu_sensor::getChipName()) + " (0x" + String(imu_sensor::getActiveAddress(), HEX) + ")";
      if (imu_sensor::hasMagnetometer()) {
        msg += " + AK8963 Magnetometer (0x0C)";
      }
      msg += ". Pitch: " + String(imu_sensor::getState().pitch, 1) + "°, Roll: " + String(imu_sensor::getState().roll, 1) + "°";
      json += "\"status\":\"PASS\",\"msg\":\"" + msg + "\"}";
    } else if (hasMpu) {
      json += "\"status\":\"PASS\",\"msg\":\"Phát hiện chip IMU tại địa chỉ I2C 0x" + String(mpuAddr, HEX) + "\"}";
    } else {
      json += "\"status\":\"WARN\",\"msg\":\"Chưa thấy IMU (0x68/0x69). (Lắp đặt sau khi hoàn thiện cơ khí).\"}";
    }
    json += ",";

    // Item 5: IR Line Sensors
    json += "{\"name\":\"5. Cảm Biến Dò Đường Hồng Ngoại\",";
    json += "\"status\":\"PASS\",\"msg\":\"IR trai IO" + String(pins::IR_L) + "=" + String(irL) +
            ", IR phai IO" + String(pins::IR_R) + "=" + String(irR) + ".\"}";
    json += "]}";

    server.send(200, "application/json", json);
  });

  // 12. Display Configuration API
  server.on("/display", HTTP_GET, []() {
    uint8_t curId = (uint8_t)store::getDisplayType();
    String name = jsonEscape(store::getDisplayTypeName((store::DisplayType)curId));
    int w = emotion_gfx::getWidth();
    int h = emotion_gfx::getHeight();
    const char* source = store::getDisplaySourceName();
    int savedId = (strcmp(source, "saved") == 0) ? (int)curId : -1;
    String json = "{\"ok\":true,\"current_id\":" + String(curId) +
                  ",\"saved_id\":" + String(savedId) +
                  ",\"current_name\":\"" + name + "\"" +
                  ",\"source\":\"" + String(source) + "\"" +
                  ",\"width\":" + String(w) +
                  ",\"height\":" + String(h) +
                  ",\"init_ok\":" + String(emotion_gfx::isReady() ? "true" : "false") +
                  ",\"oled_detected\":" + String(store::isOledDetected() ? "true" : "false") +
                  ",\"oled_sda\":" + String(store::getOledSdaPin()) +
                  ",\"oled_scl\":" + String(store::getOledSclPin()) +
                  ",\"oled_addr\":\"0x" + String(store::getOledAddr(), HEX) + "\"}";
    server.send(200, "application/json", json);
  });

  // 13. Set Display Configuration & Auto Reset
  server.on("/set_display", HTTP_GET, []() {
    if (!server.hasArg("id")) {
      server.send(400, "application/json", "{\"ok\":false,\"err\":\"missing_id\"}");
      return;
    }
    int id = server.arg("id").toInt();
    if (!display_policy::isValidId(id)) {
      server.send(400, "application/json", "{\"ok\":false,\"err\":\"invalid_id\"}");
      return;
    }
    bool ok = store::setDisplayType((store::DisplayType)id);
    if (!ok) {
      server.send(500, "application/json", "{\"ok\":false,\"err\":\"nvs_save_failed\"}");
      return;
    }
    String name = jsonEscape(store::getDisplayTypeName((store::DisplayType)id));
    debug_log::push((String("Luu man hinh id ") + String(id)).c_str());
    server.send(200, "application/json",
                "{\"ok\":true,\"id\":" + String(id) + ",\"name\":\"" + name + "\"}");
    rebootPending = true;
    rebootAt = millis() + 1200;
  });

  // 14. Test Display Pattern
  server.on("/test_display", HTTP_GET, []() {
    emotion_gfx::testDisplayPattern();
    server.send(200, "application/json", "{\"ok\":true,\"msg\":\"test_pattern_executed\"}");
  });

  // 14b. Test OLED Directly (Hardware Force Light-Up)
  server.on("/oled_test", HTTP_GET, []() {
    auto scan = oled_diag::scanAllCandidatePins();
    String report = oled_diag::getPinDiagnosticsReport();
    report.replace("\n", "\\n");
    report.replace("\"", "\\\"");
    bool spiHeader = oled_diag::spiHeaderConnected();
    int sda = scan.found ? scan.sdaPin : store::getOledSdaPin();
    int scl = scan.found ? scan.sclPin : store::getOledSclPin();
    uint8_t addr = scan.found ? scan.address : store::getOledAddr();
    store::DisplayType before = store::getDisplayType();
    bool ok = false;
    const char* bus = "none";
    if (scan.found) {
      bus = "i2c";
      ok = oled_diag::runVisualTest(sda, scl, addr);
    } else if (spiHeader) {
      bus = "spi";
      ok = oled_diag::forceSpiPanelOn();
      delay(1000);
    }
    bool switched = false;
    if (!scan.found && spiHeader && before == store::DisplayType::SSD1306_096) {
      switched = store::setDisplayType(store::DisplayType::SSD1306_096_SPI);
    }
    int disp = (int)store::getDisplayType();
    String json = "{\"ok\":true,\"found\":" + String(scan.found ? "true" : "false") +
                  ",\"bus\":\"" + String(bus) + "\"" +
                  ",\"spi_header\":" + String(spiHeader ? "true" : "false") +
                  ",\"display_id\":" + String(disp) +
                  ",\"sda\":" + String(sda) + ",\"scl\":" + String(scl) +
                  ",\"addr\":\"0x" + String(addr, HEX) + "\"" +
                  ",\"tested\":" + String(ok ? "true" : "false") +
                  ",\"report\":\"" + report + "\"" +
                  ",\"help\":\"Cum SPI IO38-42 co dien. I2C khong thay. Lua chon OLED I2C cu doi sang 0.96 OLED SPI. Neu kinh la IPS mau, chon 0.96 ST7735 roi Luu. Giu nguyen day.\"}";
    server.send(200, "application/json", json);
    if (switched) {
      rebootPending = true;
      rebootAt = millis() + 400;
    } else if (!scan.found && spiHeader) {
      emotion_gfx::init();
    }
  });

  // 15. System Reboot
  server.on("/reboot", HTTP_GET, []() {
    server.send(200, "application/json", "{\"ok\":true,\"msg\":\"rebooting\"}");
    rebootPending = true;
    rebootAt = millis() + 400;
  });

  server.on("/links", HTTP_GET, []() {
    auto ack = [](uint8_t addr) {
      Wire.beginTransmission(addr);
      return Wire.endTransmission() == 0;
    };
    bool pca = ack(0x40);
    bool imu68 = ack(0x68);
    bool imu69 = ack(0x69);
    bool mag = ack(0x0C);
    bool oled = ack(0x3C) || ack(0x3D);
    bool imuAvail = imu_sensor::isAvailable();
    float pitch = 0.0f, roll = 0.0f;
    if (imuAvail) {
      const auto& st = imu_sensor::getState();
      pitch = st.pitch;
      roll = st.roll;
    }
    uint8_t curId = (uint8_t)store::getDisplayType();
    String json = "{\"ok\":true";
    json += ",\"wifi\":{\"ssid\":\"" + WiFi.softAPSSID() + "\",\"ip\":\"" + WiFi.softAPIP().toString() +
            "\",\"clients\":" + String(WiFi.softAPgetStationNum()) + "}";
    json += ",\"pca\":" + String(pca ? "true" : "false");
    json += ",\"imu_addr\":" + String((imu68 || imu69) ? "true" : "false");
    json += ",\"mag\":" + String(mag ? "true" : "false");
    json += ",\"oled_bus\":" + String(oled ? "true" : "false");
    json += ",\"oled_boot\":" + String(store::isOledDetected() ? "true" : "false");
    json += ",\"oled_sda\":" + String(store::getOledSdaPin());
    json += ",\"oled_scl\":" + String(store::getOledSclPin());
    json += ",\"oled_addr\":\"0x" + String(store::getOledAddr(), HEX) + "\"";
    json += ",\"imu\":{\"avail\":" + String(imuAvail ? "true" : "false");
    json += ",\"chip\":\"" + String(imu_sensor::getChipName()) + "\"";
    json += ",\"pitch\":" + String(pitch, 1);
    json += ",\"roll\":" + String(roll, 1) + "}";
    json += ",\"display\":{\"id\":" + String(curId);
    json += ",\"name\":\"" + jsonEscape(store::getDisplayTypeName((store::DisplayType)curId)) + "\"";
    json += ",\"w\":" + String(emotion_gfx::getWidth());
    json += ",\"h\":" + String(emotion_gfx::getHeight());
    json += ",\"init_ok\":" + String(emotion_gfx::isReady() ? "true" : "false");
    json += ",\"source\":\"" + String(store::getDisplaySourceName()) + "\"}";
    json += ",\"v\":" + String(sensors::readBatteryVoltage(), 2);
    json += ",\"temp\":" + String(temperatureRead(), 1);
    json += ",\"heap\":" + String(ESP.getFreeHeap());
    json += ",\"uptime\":" + String(millis() / 1000);
    json += "}";
    server.send(200, "application/json", json);
  });

  server.on("/pins", HTTP_GET, []() {
    size_t count = 0;
    const pin_catalog::Entry* rows = pin_catalog::entries(&count);
    float bat = sensors::readBatteryVoltage();
    String json = "{\"ok\":true,\"pins\":[";
    for (size_t i = 0; i < count; i++) {
      if (i) json += ",";
      const pin_catalog::Entry& entry = rows[i];
      json += "{\"gpio\":" + String(entry.gpio);
      json += ",\"name\":\"" + String(entry.name) + "\"";
      json += ",\"bus\":\"" + String(pin_catalog::busName(entry.bus)) + "\"";
      if (pin_catalog::readsLevel(entry.bus)) {
        json += ",\"level\":" + String(digitalRead(entry.gpio));
        json += ",\"note\":\"muc logic, khong doi mode\"";
      } else if (entry.bus == pin_catalog::Bus::Adc) {
        json += ",\"mv\":" + String((int)(bat * 1000.0f));
        json += ",\"note\":\"dien ap pin\"";
      } else if (entry.bus == pin_catalog::Bus::Led) {
        json += ",\"note\":\"WS2812, khong doc muc\"";
      } else {
        json += ",\"note\":\"bus dang giu chan\"";
      }
      json += "}";
    }
    json += "]}";
    server.send(200, "application/json", json);
  });

  server.on("/events", HTTP_GET, []() {
    uint32_t since = server.hasArg("since") ? (uint32_t)server.arg("since").toInt() : 0;
    debug_log::View views[debug_log::kCap];
    int n = debug_log::copySince(since, views, debug_log::kCap);
    String json = "{\"ok\":true,\"events\":[";
    for (int i = 0; i < n; i++) {
      if (i) json += ",";
      json += "{\"seq\":" + String(views[i].seq) + ",\"text\":\"" + String(views[i].text) + "\"}";
    }
    json += "]}";
    server.send(200, "application/json", json);
  });

  server.on("/scan_oled", HTTP_GET, []() {
    auto scan = oled_diag::scanKnownHeaders();
    if (scan.found) {
      store::setOledConfig(scan.sdaPin, scan.sclPin, scan.address);
    }
    char line[96];
    if (scan.found) {
      snprintf(line, sizeof(line), "Quet OLED ACK 0x%02X SDA %d SCL %d", scan.address, scan.sdaPin, scan.sclPin);
    } else {
      snprintf(line, sizeof(line), "Quet OLED khong thay");
    }
    debug_log::push(line);
    String json = "{\"ok\":true,\"found\":" + String(scan.found ? "true" : "false") +
                  ",\"sda\":" + String(scan.found ? scan.sdaPin : -1) +
                  ",\"scl\":" + String(scan.found ? scan.sclPin : -1) +
                  ",\"addr\":\"0x" + String(scan.found ? scan.address : 0, HEX) + "\"}";
    server.send(200, "application/json", json);
  });

  server.begin();
}

void update() {
  server.handleClient();
  if (rebootPending && (int32_t)(millis() - rebootAt) >= 0) {
    rebootPending = false;
    ESP.restart();
  }
  if (webControlActive && (millis() - lastClientPing > 1000)) {
    // Client connection timeout in manual mode
    drive::stop();
    webControlActive = false;
  }
}

}
