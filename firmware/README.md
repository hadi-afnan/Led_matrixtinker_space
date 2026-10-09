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
5. **Connect to Web App**:
   - On your phone or PC, connect to Wi-Fi: **`TinkerSpace-LED`** (password: `makeanything`).
   - Open your browser to: **`http://192.168.4.1`** (or `http://tinkerspace.local` on mDNS).
   - Enjoy live multi-line space sign creation, image/logo dithering, real-time 50×30 pixel painting, and visual effects!
