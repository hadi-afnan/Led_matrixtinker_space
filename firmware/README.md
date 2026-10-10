# TinkerSpace LED Display Firmware

This directory contains the production firmware and diagnostic test sketches for the **TinkerSpace Modular LED Display** powered by the **ESP32-S3**.

---

## 📂 Directory Structure

```text
firmware/
├── README.md
└── arduino/
    ├── led_display/               # Production firmware & embedded web app
    │   ├── led_display.ino        # Main setup, loop, button, and engine orchestrator
    │   ├── config.h               # Central configuration (pins, grid, brightness, Wi-Fi, limits)
    │   ├── display_mapping.h      # Bottom-left (X, Y) to physical row-serpentine mapping
    │   ├── display_mapping.cpp
    │   ├── buttons.h              # Non-blocking tactile button debouncing class
    │   ├── buttons.cpp
    │   ├── animations.h           # High-performance visual effects & test patterns
    │   ├── animations.cpp
    │   ├── text_renderer.h        # Multi-line signs, big header, badge, & marquee engine
    │   ├── text_renderer.cpp
    │   ├── web_server_handler.h   # SoftAP + Station Wi-Fi, REST API, & WebServer
    │   ├── web_server_handler.cpp
    │   └── web_ui.h               # Precompiled PROGMEM single-page web app bundle
    └── examples/                  # Diagnostic bring-up sketches
        ├── 01_single_module_test/
        ├── 02_matrix_serpentine_test/
        ├── 03_button_input_test/
        └── 04_corner_test/        # 4-corner coordinate validation sketch
```

---

## ⚙️ Key Architectural Highlights

1. **Logical 2D Canvas**: All visual effects, text layouts, and web simulator pixels use a Cartesian coordinate plane with origin `(0, 0)` at the **Bottom-Left** corner:
   - $X = 0 \dots 49$ (Left to Right)
   - $Y = 0 \dots 29$ (Bottom to Top)
2. **Row-Serpentine Hardware Abstraction**: The `DisplayMapping` class translates logical coordinates to the physical LED index:
   - Row 0 (bottom, $y=0$): Left to Right ($0 \dots 49$)
   - Row 1 ($y=1$): Right to Left ($99 \dots 50$)
   - Row $y$ even: $(y \times 50) + x$
   - Row $y$ odd: $(y \times 50) + (49 - x)$
3. **Embedded Web Controller**: Complete WLED-style single-page web application compiled into ESP32-S3 Flash (PROGMEM) requiring zero LittleFS/SPIFFS partition setup. Accessible offline via SoftAP or via local Wi-Fi.
4. **Non-Blocking Architecture**: 60 FPS animation loop with millisecond timing. No blocking `delay()` calls, ensuring instantaneous web server API and tactile button responsiveness.
5. **Electrical Safety Enforcement**: FastLED dynamic power limiting enforces a 25 A (or custom) current limit at 5V, preventing over-current conditions.

---

## 🚀 Flashing & Quick Start

1. **Open Sketch**: In Arduino IDE, select **File > Open** and choose `firmware/arduino/led_display/led_display.ino`.
   *(All 12 sibling `.h` and `.cpp` files will open automatically as tabs).*
2. **Board Settings**:
   - Board: **ESP32S3 Dev Module** (or your specific S3 board)
   - USB CDC On Boot: **Enabled** (for Serial Monitor output)
   - Flash Size: **8MB** or **16MB** (Default partitions work out of the box)
3. **Required Library**:
   - `FastLED` version 3.6.0+ (install via Arduino Library Manager)
4. **Flash & Run**:
   - Click **Upload**.
   - Open Serial Monitor at **115200 baud** to view system diagnostic output.
5. **Connect to Web App from Phone / PC**:
   - On your phone, tablet, or laptop, open Wi-Fi settings and select: **`TinkerSpace-LED`**
   - Enter Password: **`makeanything`**
   - **Android**: When prompted *"Wi-Fi has no internet access"*, tap the notification and select **"Keep Wi-Fi connection"** / **"Stay Connected"** (or temporarily toggle off mobile data so your browser doesn't route traffic to cellular).
   - **iPhone / iOS**: The Captive Portal Assistant will pop up automatically with the TinkerSpace Web UI. If you close it, select **"Use Without Internet"**.
   - Open your browser to: **`http://192.168.4.1`** (or `http://tinkerspace.local` on Apple/mDNS devices).
   - Enjoy live multi-line space signs, image/logo dithering, real-time 50×30 pixel painting, animations, and hardware diagnostic controls!

---

## 🔘 Physical Buttons & Controls

| Action | Control | Function |
| :--- | :--- | :--- |
| **Short Press Button 1** (GPIO 14) | Next Pattern | Advances to the next visual animation |
| **Short Press Button 2** (GPIO 15) | Pause / Resume | Freezes or unfreezes current animation |
| **Long Press Button 2 (> 1.5s)** | Hardware Reset | Resets display mode back to Mode 0 (Rainbow Cycle) and clears buffers |
| **Dual Button Hold (> 2.5s)** | Soft Reboot | Reboots the ESP32-S3 microcontroller cleanly |

---

## ⌨️ USB Serial Console Commands (115200 Baud)

When connected to a computer via USB-C, type the following single-character commands into the Arduino Serial Monitor:

- `'n'` or `'N'` : Next animation mode
- `'p'` or `'P'` : Pause / resume animation
- `'+'` : Increase display brightness (+16)
- `'-'` : Decrease display brightness (-16)
- `'r'` or `'R'` : Universal display reset (resets to Mode 0)
- `'c'` or `'C'` : Clear all LEDs to black
- `'m'` or `'M'` : Run 15-module sweep diagnostics (illuminates each 100-LED tile sequentially)
- `'0'`–`'9'` : Jump directly to animation mode 0 through 9

---

## 🌐 Embedded REST API Endpoints

The ESP32-S3 provides instant HTTP JSON endpoints (CORS-enabled):

- `GET  /api/status` : Current display power, brightness, active mode, IP addresses, wiring topology, free heap.
- `POST /api/power` : Toggle display power (`{"power": true/false}`).
- `POST /api/brightness` : Set brightness 0–255 (`{"brightness": 120}`).
- `POST /api/mode` : Set animation preset by index (`{"index": 0}`).
- `POST /api/text` : Display custom multi-line text (`{"line1":"...", "layout":"bigheader", "effect":"scroll", ...}`).
- `POST /api/frame` : Push complete 50×30 RGB frame (Base64 or JSON pixel array).
- `POST /api/wiring` : Toggle runtime wiring topology (`{"wiring": 0}` for row-serpentine, `1` for tile-serpentine).
- `POST /api/test_module` : Run 15-module diagnostic sweep.
- `POST /api/reset` : Reset display back to default animation mode 0.
- `POST /api/wifi` : Save home/makerspace router credentials (`{"ssid":"...", "password":"..."}`).
- `POST /api/restart` : Reboot ESP32-S3.

