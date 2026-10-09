# Contributing to TinkerSpace Modular LED Display

Thank you for your interest in contributing to the **TinkerSpace Modular LED Display** project! This project is built by and for makers, educators, students, and open-source hardware enthusiasts.

---

## Areas You Can Contribute

1. **Firmware & Animations**:
   - New patterns, visualizers, particle systems, games, and sprite rendering.
   - Memory and frame rate optimization on the ESP32-S3.
   - Text scrolling and font library additions.
2. **Mechanical & 3D Printing**:
   - Snap-fit improvements for the 10×10 module frames.
   - Optical testing for diffuser thicknesses (e.g., testing PETG, translucent PLA, infill patterns).
   - Mounting solutions (VESA brackets, French cleat wall mounts, desktop stands).
3. **Electronics & Safety**:
   - Power distribution busbar designs and custom distribution PCBs.
   - Logic level shifter breakout boards.
   - Thermal testing and fuse selection validation.
4. **Documentation**:
   - Translations, beginner-friendly wiring diagrams, troubleshooting case studies.

---

## Development Guidelines

### 1. Firmware Standards
- **Non-Blocking Code**: Never use `delay()` in animation loops. Use `millis()`-based task scheduling so button presses and UI interactions remain instantly responsive.
- **Matrix Mapping**: Always draw pixels through the continuous `DisplayMapping::setPixel(x, y, color)` abstraction rather than calculating physical LED strip indices inside animation logic.
- **Memory Safety**: The ESP32-S3 has ample SRAM, but keep dynamic memory allocations minimal. Prefer pre-allocated buffers.
- **FastLED / NeoPixel**: Keep frame rate throttled reasonably (e.g. 60-120 FPS) to prevent unnecessary thermal buildup.

### 2. Mechanical Design (3D Printing)
- Designed primarily for **Bambu Lab A1** (256 × 256 mm bed) or standard 220 × 220 mm beds.
- Maintain tolerance clearances of 0.2 mm – 0.3 mm for snap joints.
- Export both native CAD files (STEP / Fusion 360 / FreeCAD) and printable meshes (`.stl`, `.3mf`).

### 3. Safety Standards
- Because this project uses a high-current 5 V power supply (up to 100 A / 500 W), all electrical design contributions **must prioritize safety**:
  - Direct branch fusing for each module.
  - Proper wire gauge sizing (AWG).
  - Bulk capacitance at power injection points.

---

## Submitting Contributions

1. Fork the repository on GitHub.
2. Create a topic branch:
   ```bash
   git checkout -b feature/my-cool-animation
   # or
   git checkout -b fix/diffuser-clip-tolerance
   ```
3. Commit your changes with clear, descriptive messages:
   ```bash
   git commit -m "Add Perlin noise rainbow animation"
   ```
4. Push to your fork:
   ```bash
   git push origin feature/my-cool-animation
   ```
5. Open a Pull Request against the `main` branch. Provide screenshots, photos of physical prints, or video recordings of animations running on actual hardware when possible.

---

## Code of Conduct

Please review and adhere to our [Code of Conduct](file:///e:/Embedded/Projects/led_metrix/CODE_OF_CONDUCT.md) in all community interactions.
