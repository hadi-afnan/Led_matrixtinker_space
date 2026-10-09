# System Architecture & Mapping Design

This document details the software architecture, coordinate mapping mathematics, and data flow of the **TinkerSpace Modular LED Display**.

---

## 1. High-Level System Architecture

The system decouples high-level animation rendering from physical LED hardware through an abstracted logical coordinate canvas:

```mermaid
graph TD
    subgraph "Application & Animation Layer"
        UserControls[Tactile Buttons / Serial] --> StateMachine[State Machine / Mode Selector]
        StateMachine --> AnimEngine[Animation Engine]
        AnimEngine --> DrawCanvas["Logical 2D Canvas<br/>setPixel(x, y, CRGB)"]
    end

    subgraph "Hardware Abstraction Layer (HAL)"
        DrawCanvas --> DisplayMapper["DisplayMapping Engine<br/>(Inter-Module & Intra-Module Serpentine Mapping)"]
        DisplayMapper --> PixelBuffer["1D FastLED Pixel Array<br/>CRGB leds[TOTAL_LEDS]"]
    end

    subgraph "Hardware Layer"
        PixelBuffer --> RMTDriver["ESP32-S3 RMT Hardware Peripheral"]
        RMTDriver --> LevelShifter["74AHCT125 (3.3V to 5V Buffer)"]
        LevelShifter --> WS2812Chain["WS2812B Chain (1,500 or 1,200 LEDs)"]
    end
```

---

## 2. Coordinate Mapping: From 2D (X, Y) to 1D Serial Index

A core design requirement is that software developers, students, and game designers should not need to know where individual wires are soldered. They draw to an intuitive Cartesian grid:
- **Origin `(0, 0)`**: Top-Left corner of the entire display.
- **X-axis**: Points right (`0 <= X < TOTAL_WIDTH`).
- **Y-axis**: Points down (`0 <= Y < TOTAL_HEIGHT`).

### Physical Topology Variations

```
========================================================================
LAYOUT 1: Physical Prototype (5 modules wide × 3 modules high = 50 × 30)
========================================================================
Row 0:  [Mod 0 ] ---> [Mod 1 ] ---> [Mod 2 ] ---> [Mod 3 ] ---> [Mod 4 ] ──┐
                                                                          │
Row 1:  [Mod 9 ] <─── [Mod 8 ] <─── [Mod 7 ] <─── [Mod 6 ] <─── [Mod 5 ] <─┘
        │
Row 2:  └──> [Mod 10] ---> [Mod 11] ---> [Mod 12] ---> [Mod 13] ---> [Mod 14]

========================================================================
LAYOUT 2: Standard 4:3 Target (4 modules wide × 3 modules high = 40 × 30)
========================================================================
Row 0:  [Mod 0 ] ---> [Mod 1 ] ---> [Mod 2 ] ---> [Mod 3 ] ──┐
                                                             │
Row 1:  [Mod 7 ] <─── [Mod 6 ] <─── [Mod 5 ] <─── [Mod 4 ] <─┘
        │
Row 2:  └──> [Mod 8 ] ---> [Mod 9 ] ---> [Mod 10] ---> [Mod 11]
```

---

## 3. The Two-Stage Serpentine Transform

Each physical pixel location requires a two-stage transformation:

### Stage 1: Inter-Module Serpentine (Module Grid Location)
Given global coordinates `(x, y)`:
1. Determine the module grid position:
   $$\text{module\_x} = \lfloor x / 10 \rfloor$$
   $$\text{module\_y} = \lfloor y / 10 \rfloor$$
2. Calculate the local coordinate within that module:
   $$\text{local\_x} = x \pmod{10}$$
   $$\text{local\_y} = y \pmod{10}$$
3. Because rows alternate direction (serpentine cabling between modules):
   - If `module_y` is **even** (Rows 0, 2, ...): modules run left-to-right:
     $$\text{module\_index} = (\text{module\_y} \times \text{MODULES\_X}) + \text{module\_x}$$
   - If `module_y` is **odd** (Row 1, ...): modules run right-to-left:
     $$\text{module\_index} = (\text{module\_y} \times \text{MODULES\_X}) + (\text{MODULES\_X} - 1 - \text{module\_x})$$

### Stage 2: Intra-Module Serpentine (10×10 Internal Traces)
Standard 10×10 flexible FPCB WS2812B matrices are internally routed in serpentine rows:
- In odd rows within the module, the data traces travel backwards:
  - If `local_y` is **even**: $\text{pixel\_in\_module} = (\text{local\_y} \times 10) + \text{local\_x}$
  - If `local_y` is **odd**: $\text{pixel\_in\_module} = (\text{local\_y} \times 10) + (9 - \text{local\_x})$

### Final 1D Index Calculation
$$\text{Index} = (\text{module\_index} \times 100) + \text{pixel\_in\_module}$$

This transformation runs in sub-microsecond time on the ESP32-S3 and is implemented in `DisplayMapping::getPixelIndex(x, y)`.

---

## 4. Firmware Subsystems & State Machine

```mermaid
stateDiagram-v2
    [*] --> Initialization
    Initialization --> RunningAnimation
    
    RunningAnimation --> NextAnimation : Button 1 Click
    NextAnimation --> RunningAnimation : Mode Index Incremented
    
    RunningAnimation --> Paused : Button 2 Click
    Paused --> RunningAnimation : Button 2 Click (Resume)
    
    RunningAnimation --> BrightnessAdjust : Long Press (Optional)
```

The firmware is structured into modular components:
1. **`config.h`**: Hardware pin assignments, grid dimensions, brightness caps, and layout presets.
2. **`display_mapping`**: Fast matrix translation routines with boundary clamping.
3. **`animations`**: Clean, non-blocking rendering classes for plasma, fire, waves, text scrolling, and color fills.
4. **`buttons`**: Non-blocking software debounce routine using state-edge detection.
5. **`led-display.ino`**: Top-level coordinator and timing scheduler.

---

## 5. Electrical Domain Isolation & Safety

The system features two interconnected electrical domains:
1. **Low-Voltage Signal Domain (3.3 V)**: The ESP32-S3 microcontroller operates at 3.3 V CMOS levels. The RMT output is passed to a high-speed CMOS buffer (74AHCT125) powered by 5.0 V.
2. **High-Current Power Domain (5.0 V)**: A 500 W industrial switching power supply drives the LED matrix through heavy-gauge copper busbars and individually fused branch lines.

A single, unified ground plane connects the power supply, LED strips, logic level shifter, and ESP32-S3 ground pins to prevent floating signal references and noise glitches.
