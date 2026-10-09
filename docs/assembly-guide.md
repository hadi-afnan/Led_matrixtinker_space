# Physical Assembly & Construction Guide

This step-by-step guide walks you through fabricating the mechanical components, assembling the structural frame, soldering the wiring harness, and performing pre-flight electrical testing for the **TinkerSpace Modular LED Display**.

---

## 🛠️ Required Tools & Materials

### Tools
- **3D Printer**: Bambu Lab A1 (or any printer with at least 220 × 220 mm build volume).
- **Soldering Station**: Temperature-controlled iron (350°C / 660°F recommended for heavy pads), lead-free or 63/37 solder, flux pen.
- **Hand Tools**: Precision wire strippers (AWG 18–26), flush cutters, hex drivers (M3), tweezers.
- **Test Equipment**: Digital Multimeter (DMM) with continuity beeper.
- **Safety Equipment**: Safety glasses, heat-resistant soldering mat, ESD protection.

### Consumables
- White PLA filament for diffusers (e.g., Bambu PLA Basic Jade White).
- Black or dark grey PLA/PETG filament for module frames (prevents optical bleed between pixels).
- 24 AWG silicone stranded wire (signals).
- 18–20 AWG silicone stranded wire (module power branches).
- 8–10 AWG silicone stranded wire (main power feeds).
- Heat-shrink tubing & Kapton tape.
- 15× 1,000 µF / 6.3 V or 10 V electrolytic capacitors.

---

## Phase 1: 3D Printing Components

### 1. Module Frames (15 units for 5×3 prototype; 12 units for 4×3 target)
- **Material**: Black PLA or PETG (Black plastic absorbs stray light and creates crisp pixel contrast).
- **Infill**: 15% – 20% Gyroid.
- **Layer Height**: 0.20 mm Standard.
- **Wall Loops**: 3 walls.

### 2. Diffuser Sheets
- **Material**: Translucent / Opaque White PLA (Bambu Jade White).
- **Layer Height**: 0.08 mm High Quality.
- **Infill**: 100% Solid (essential for uniform light scatter without internal infill shadows).
- **Thickness Variants**:
  - **0.16 mm (2 layers @ 0.08 mm)**: Maximum light output and brightness, slightly more hotspot visibility.
  - **0.24 mm (3 layers @ 0.08 mm)**: Superior diffusion and smoother blend; slightly stiffer sheet.

---

## Phase 2: Frame Grid Assembly

```
┌──────────────┬──────────────┬──────────────┬──────────────┬──────────────┐
│  Module 0    │  Module 1    │  Module 2    │  Module 3    │  Module 4    │
├──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│  Module 9    │  Module 8    │  Module 7    │  Module 6    │  Module 5    │
├──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
│  Module 10   │  Module 11   │  Module 12   │  Module 13   │  Module 14   │
└──────────────┴──────────────┴──────────────┴──────────────┴──────────────┘
```

1. Lay all printed module frames face-down on a flat, clean surface.
2. Join adjacent frames using the alignment tabs and M3 button-head screws.
3. Affix the grid to a rigid backplate (e.g., 6 mm laser-cut acrylic sheet or 2020 aluminum extrusion perimeter frame).
4. Verify overall frame dimensions (nominally 828 × 496.8 mm for 5×3).

---

## Phase 3: Module Placement & Orientation

1. **Clean FPCB Pads**: Inspect the solder pads on each 10×10 panel. Pre-tin `DIN`, `DOUT`, `+5V`, and `GND` pads.
2. **Adhere Panels**: Carefully remove the protective backing and stick each 10×10 FPCB module into its recessed frame socket.
3. **Verify Orientation**: Ensure each module's internal coordinate orientation matches your wiring plan (Row 0 modules have `DIN` on the left, Row 1 has `DIN` on the right if reverse-oriented, or keep all modules oriented identically and route serpentine interconnect jumpers).

---

## Phase 4: Data Harness Soldering

```
   Row 0:  [DIN -> DOUT] ──> [DIN -> DOUT] ──> [DIN -> DOUT] ──> [DIN -> DOUT] ──> [DIN -> DOUT]
                                                                                            │
   Row 1:  [DOUT <- DIN] <── [DOUT <- DIN] <── [DOUT <- DIN] <── [DOUT <- DIN] <── [DOUT <- DIN]
              │
   Row 2:  [DIN -> DOUT] ──> [DIN -> DOUT] ──> [DIN -> DOUT] ──> [DIN -> DOUT] ──> [DIN -> DOUT]
```

1. Solder a 3-wire harness (Data, GND, and optional 5V) from the ESP32 level shifter output to **Module 0 DIN**.
2. Solder short 24 AWG flexible jumper wires from `DOUT` of Module $k$ to `DIN` of Module $k+1$.
3. At the end of each row, run a neat jumper wire down to the next row following the serpentine path.
4. Secure each wire junction with Kapton tape to prevent tension on the solder joints.

---

## Phase 5: Power Bus & Injection Soldering

1. Mount the automotive blade fuse block and copper busbar adjacent to the rear frame.
2. Run dedicated **AWG 18 or 20** wire pairs (+5 V and GND) from each fuse output directly to each 10×10 module power injection pads.
3. Solder a **1,000 µF / 10 V electrolytic capacitor** across each module's `+5V` and `GND` pads. Observe correct polarity!
4. Tie all ground wires back to the primary negative busbar.

---

## Phase 6: Controller & Front-Panel Enclosure

1. Install the ESP32-S3 Dev Board, 74AHCT125 level shifter, and input switches into the 3D-printed electronics housing.
2. Wire Button 1 (Next Mode) between GPIO 14 and GND.
3. Wire Button 2 (Pause/Resume) between GPIO 15 and GND.
4. Mount the enclosure to the frame side or rear bezel.

---

## Phase 7: Pre-Flight Electrical Checklist

> [!IMPORTANT]
> **Complete all 5 checklist steps BEFORE flipping the main power switch!**

- [ ] **Step 1: Visual Inspection**: Check all solder joints for stray solder bridges, whisker shorts, or cold joints.
- [ ] **Step 2: Dead-Short Multimeter Test**: Set your DMM to resistance/continuity mode. Measure resistance between `+5V` bus and `GND` bus. Resistance should start low (capacitors charging) and climb to several kilo-ohms. **If reading remains 0.0 Ω (beep), DO NOT APPLY POWER.**
- [ ] **Step 3: Unloaded PSU Voltage Check**: Disconnect the LED grid. Power on the G-Power PSU. Measure DC output with your DMM. It should measure between **4.95 V and 5.10 V**. Adjust the trimpot if necessary.
- [ ] **Step 4: Single Module Smoke Test**: Connect only Module 0. Flash the test sketch [`01_single_module_test`](file:///e:/Embedded/Projects/led_metrix/firmware/arduino/examples/01_single_module_test). Confirm illumination.
- [ ] **Step 5: Full Grid Bring-Up**: Connect all modules. Flash the full firmware. Verify all 1,500 (or 1,200) pixels light up uniformly without flickering or voltage drop.
