# TinkerSpace Modular LED Display

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](file:///e:/Embedded/Projects/led_metrix/LICENSE)
[![Hardware License: CERN-OHL-P v2](https://img.shields.io/badge/Hardware-CERN--OHL--P%20v2-blue.svg)](file:///e:/Embedded/Projects/led_metrix/LICENSE)
[![ESP32-S3](https://img.shields.io/badge/Controller-ESP32--S3-red.svg)](https://www.espressif.com/en/products/socs/esp32-s3)
[![FastLED Compatible](https://img.shields.io/badge/Library-FastLED-brightgreen.svg)](https://github.com/FastLED/FastLED)
[![Printer: Bambu Lab A1](https://img.shields.io/badge/3D%20Print-Bambu%20Lab%20A1-black.svg)](https://bambulab.com/en/a1)

> **A modular, repairable, open-source RGB LED display system built with 10×10 WS2812B modules, 3D-printed interlocking frames, Bambu Lab-tuned optical diffusers, and an ESP32-S3 animation engine.**

---

## 🌟 Overview

The **TinkerSpace Modular LED Display** turns standard 10×10 WS2812B flexible LED panels into a scalable large-format display for makerspaces, classrooms, creative studios, and hobbyists.

Unlike monolithic, closed-source commercial LED displays, this project emphasizes:
- **Modular Repairability**: If a single LED burns out, replace or service only that 10×10 unit without rebuilding the entire matrix.
- **Continuous 2D Canvas**: Write graphics, text, games, and animations in standard `(X, Y)` coordinates. The firmware handles all hardware-level serpentine conversions seamlessly.
- **Open-Source Mechanical Design**: 3D-printable snap-fit frames and diffusers optimized for standard desktop 3D printers like the Bambu Lab A1.
- **Industrial Safety Standards**: Engineered power distribution architecture designed to safely handle high-current 5 V power supplies (up to 100 A / 500 W) with branch fuses, busbars, and power injection.

---

## 📐 Revisions: Physical Prototype vs. 4:3 Target

To preserve design lineage and avoid confusion, the project maintains two documented layout revisions:

| Specification | Physical Prototype (Rev 1.0) | Standard 4:3 Target (Rev 2.0) |
| :--- | :--- | :--- |
| **Grid Arrangement** | **5 modules wide × 3 modules high** | **4 modules wide × 3 modules high** |
| **Logical Resolution** | **50 × 30 pixels** | **40 × 30 pixels** |
| **Total LED Count** | **1,500 LEDs** | **1,200 LEDs** |
| **Display Aspect Ratio**| 5:3 (~1.67:1) | 4:3 (1.33:1 classic retro format) |
| **Nominal Active Area**| 828.0 mm × 496.8 mm | 662.4 mm × 496.8 mm |
| **Module Footprint** | 165.6 mm × 165.6 mm per module | 165.6 mm × 165.6 mm per module |
| **Status** | **Physical build tested & active** | Documented design configuration |

Both revisions use the same underlying modular components and firmware codebase. The active layout is configured with a single line change in [`firmware/arduino/led-display/config.h`](file:///e:/Embedded/Projects/led_metrix/firmware/arduino/led-display/config.h).

---

## ⚡ Hardware Architecture

```
[ 5V / 100A PSU ] ──(Main Switch)──> [ High-Current Busbar ]
                                            │
               ┌────────────────────────────┼───────────────────────────┐
               ▼                            ▼                           ▼
        [ Fuse Block 1 ]             [ Fuse Block 2 ]            [ Fuse Block N ]
         (3A Mini Blade)              (3A Mini Blade)             (3A Mini Blade)
               │                            │                           │
               ▼                            ▼                           ▼
         Module 1 (5V/GND)            Module 2 (5V/GND)           Module N (5V/GND)
         [+ 1000µF Cap]               [+ 1000µF Cap]              [+ 1000µF Cap]

[ ESP32-S3 ] 
  ├─ GPIO 4 (Data) ──>[ 74AHCT125 Level Shifter ]──[330Ω]──> Module 1 DIN ──> DOUT ──> Module 2 DIN ...
  ├─ GPIO 14 ─────────>[ Button 1: Next Mode ]
  └─ GPIO 15 ─────────>[ Button 2: Pause / Resume ]
```

### Core Hardware Specifications
- **LED Type**: WS2812B 5 V Addressable RGB LEDs (SMD 5050).
- **Module Pitch & Size**: 10×10 pixels on flexible FPCB, approx. 165.6 × 165.6 mm.
- **Microcontroller**: ESP32-S3 dual-core Xtensa LX7 @ 240 MHz.
- **Logic Buffering**: 74AHCT125 / 74HCT245 (3.3 V logic stepped up to 5.0 V).
- **Power Supply**: 5 V / 100 A (500 W max) regulated switching PSU.
- **Interactive Controls**: Dual tactile momentary pushbuttons (Next pattern / Pause).

---

## ⚠️ Electrical Safety & High Current Notice

> [!CAUTION]
> **A 5 V / 100 A power supply can deliver 500 Watts of power!**
> In the event of a short circuit, an unfused wire can instantly vaporize, catch fire, or destroy components.
> - **DO NOT daisy-chain power through the LED modules.**
> - Use a heavy-gauge main bus wire (AWG 10–12).
> - Each module's 5 V power branch **must be protected with an inline fuse (3 A to 5 A)**.
> - Detailed safety instructions and calculations are available in [`docs/power-distribution.md`](file:///e:/Embedded/Projects/led_metrix/docs/power-distribution.md).

---

## 📁 Repository Directory Structure

```text
tinkerspace-led-display/
├── README.md                     # Project overview and specifications
├── LICENSE                       # MIT, CERN-OHL-P, and CC-BY-SA licenses
├── CONTRIBUTING.md               # Contribution and PR guidelines
├── CODE_OF_CONDUCT.md            # Contributor Covenant Code of Conduct
├── .gitignore                    # Build and OS artifact exclusions
│
├── docs/                         # Comprehensive engineering documentation
│   ├── architecture.md           # System design & logical-to-physical coordinate mapping
│   ├── wiring.md                 # Signal wiring, level shifter, and button schematics
│   ├── power-distribution.md     # 5V/100A busbar, wire gauges, fusing, & injection rules
│   ├── assembly-guide.md         # Mechanical assembly, soldering, and frame integration
│   ├── firmware-setup.md         # Arduino IDE / PlatformIO installation and flashing guide
│   ├── troubleshooting.md        # Diagnostic flowcharts for common hardware/software bugs
│   └── bill-of-materials.md      # Complete parts list for 5×3 and 4×3 configurations
│
├── firmware/                     # ESP32-S3 source code
│   ├── arduino/
│   │   ├── led-display/          # Production firmware with modular architecture
│   │   │   ├── led-display.ino   # Main sketch & runtime loop
│   │   │   ├── config.h          # Grid size, pinouts, limits, and layout toggles
│   │   │   ├── display_mapping.h # Logical XY to serpentine 1D index conversion
│   │   │   ├── display_mapping.cpp
│   │   │   ├── animations.h      # Animation engine (plasma, fire, meteor, waves, etc.)
│   │   │   ├── animations.cpp
│   │   │   ├── buttons.h         # Debounced tactile switch handlers
│   │   │   └── buttons.cpp
│   │   └── examples/             # Minimal diagnostic sketches
│   │       ├── 01_single_module_test/
│   │       ├── 02_matrix_serpentine_test/
│   │       └── 03_button_input_test/
│   └── README.md
│
├── hardware/                     # CAD, 3D printing files, and schematics
│   ├── electronics/              # Wiring diagrams, level shifter, distribution boards
│   │   └── README.md
│   └── mechanical/               # 3D printable models for Bambu Lab A1
│       ├── README.md
│       ├── module-frame/         # 165.6 × 165.6 mm interlocking module frames
│       ├── diffuser/             # 0.16 mm and 0.24 mm PLA diffuser sheets & 4-tile sets
│       ├── backplate/            # Backboard drill templates and extrusion mounts
│       └── mounting/             # Wall brackets and desktop stands
│
├── assets/                       # Diagrams, renders, and reference imagery
│   ├── photos/                   # Hardware build photos
│   ├── diagrams/                 # Electrical and data routing schematics
│   └── renders/                  # 3D CAD renders
│
└── tests/                        # Offline validation tools and scripts
    ├── led-mapping/              # Python XY mapping test script & simulator
    │   └── validate_mapping.py
    └── power-validation/         # Voltage drop and current requirement calculator
        └── power_calculator.py
```

---

## 🚀 Quick Start Guide

### 1. Hardware Assembly
1. 3D-print fifteen 10×10 module frames using PLA/PETG on your Bambu Lab A1.
2. Adhere the flexible WS2812B 10×10 modules into each frame.
3. Wire the data chain in horizontal serpentine order according to [`docs/wiring.md`](file:///e:/Embedded/Projects/led_metrix/docs/wiring.md).
4. Run parallel 5 V power lines from the fused distribution busbar to each module injection terminal.

### 2. Firmware Flashing
1. Install the [Arduino IDE](https://www.arduino.cc/en/software) or PlatformIO.
2. Add ESP32 board support via the Boards Manager: `esp32 by Espressif Systems`.
3. Install the **FastLED** library (version 3.6.0 or higher).
4. Open [`firmware/arduino/led-display/led-display.ino`](file:///e:/Embedded/Projects/led_metrix/firmware/arduino/led-display/led-display.ino).
5. Select board: **ESP32S3 Dev Module** (USB CDC On Boot: Enabled).
6. Verify your layout in `config.h` (default is `CONFIG_LAYOUT_5X3` for the prototype).
7. Connect your ESP32-S3 via USB-C and click **Upload**.

### 3. Controls
- **Button 1 (GPIO 14)**: Cycle through animation modes (Fire, Plasma, Color Waves, Confetti, Rainbow, Meteor, etc.).
- **Button 2 (GPIO 15)**: Pause / Resume current animation.
- **Main Power Switch**: Safely isolate 5 V DC and AC mains.

---

## 📋 Pre-Publish Release Checklist

| # | Item | Status | Details / Documentation |
| :-: | :--- | :---: | :--- |
| 1 | **Confirm final physical revision** | ✅ Completed | 5×3 prototype (1,500 LEDs) & 4×3 target (1,200 LEDs) documented. |
| 2 | **Validate LED mapping** | ✅ Completed | Continuous serpentine XY converter with offline Python test suite. |
| 3 | **Complete electrical diagram** | ✅ Completed | Full level shifter, busbar, branch fuse, and button circuits. |
| 4 | **Validate power distribution** | ✅ Completed | Wire gauge, voltage drop, and fuse math documented in power guide. |
| 5 | **Clean up firmware** | ✅ Completed | Modular FastLED Arduino firmware with debounced buttons & animations. |
| 6 | **Organize mechanical files** | ✅ Completed | Module frame, 0.16/0.24mm diffusers, and backplate specs cataloged. |
| 7 | **Add project documentation** | ✅ Completed | Architecture, Wiring, Power, Assembly, Setup, BOM, and Troubleshooting. |
| 8 | **Choose open-source licenses** | ✅ Completed | Tri-license: MIT (code), CERN-OHL-P v2 (hardware), CC-BY-SA 4.0 (docs). |

---

## 👥 Community & Contributing

Contributions are warmly welcomed! Please see [`CONTRIBUTING.md`](file:///e:/Embedded/Projects/led_metrix/CONTRIBUTING.md) to get started.

Built with ❤️ by the **TinkerSpace** community.
