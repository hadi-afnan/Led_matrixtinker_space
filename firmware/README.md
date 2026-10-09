# TinkerSpace LED Display Firmware

This directory contains the production firmware and diagnostic test sketches for the **TinkerSpace Modular LED Display** powered by the **ESP32-S3**.

---

## 📂 Directory Structure

```text
firmware/
├── README.md
└── arduino/
    ├── led-display/               # Production firmware
    │   ├── led-display.ino        # Main setup, loop, and timing orchestrator
    │   ├── config.h               # Central configuration (pins, grid, brightness, limits)
    │   ├── display_mapping.h      # 2D (X, Y) to 1D physical serpentine index abstraction
    │   ├── display_mapping.cpp
    │   ├── buttons.h              # Non-blocking tactile button debouncing class
    │   ├── buttons.cpp
    │   ├── animations.h           # High-performance visual effects & test patterns
    │   └── animations.cpp
    └── examples/                  # Diagnostic bring-up sketches
        ├── 01_single_module_test/
        ├── 02_matrix_serpentine_test/
        └── 03_button_input_test/
```

---

## ⚙️ Key Architectural Highlights

1. **Logical 2D Canvas**: All visual effects are written against a standard `(X, Y)` Cartesian coordinate plane with origin `(0, 0)` at the top-left corner.
2. **Double-Serpentine Hardware Abstraction**: The `DisplayMapping` class handles both:
   - Module-to-module serpentine routing across rows.
   - Internal 10×10 panel serpentine trace routing.
3. **Non-Blocking Architecture**: Animations use millisecond timing. No `delay()` calls are used, ensuring instantaneous button response.
4. **Safety Enforcement**: Hardware current limits are monitored at runtime via FastLED's dynamic power management to safeguard the power supply and distribution bus.

---

## 🚀 Quick Flashing Instructions

1. Install the ESP32 board definitions in Arduino IDE (Tools > Board > Boards Manager > `esp32`).
2. Install `FastLED` version 3.6.0+ via the Library Manager.
3. Open `arduino/led-display/led-display.ino`.
4. Check `config.h` to ensure your active layout (`CONFIG_LAYOUT_5X3` or `CONFIG_LAYOUT_4X3`) matches your build.
5. Select board: **ESP32S3 Dev Module** (USB CDC On Boot: Enabled).
6. Upload the sketch and open Serial Monitor at **115200 baud**.
