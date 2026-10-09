# Electronics & Circuit Specifications

This directory contains the electrical architecture, schematic details, and component pinouts for the **TinkerSpace Modular LED Display**.

---

## 1. Controller & Logic Conditioning Schematic

```text
                  +5V (Clean Rail from PSU)
                     │
         ┌───────────┴───────────┐
         │                       │
      [pin 14]                [5V Pin]
    ┌──────────┐            ┌──────────┐
    │ 74AHCT125│            │ ESP32-S3 │
    │          │            │Dev Board │
    │ 1A (pin2)│<──GPIO 4───┤          │
    │          │            │  GPIO 14 ├──o───\ o─── GND (Btn 1: Next)
    │ 1Y (pin3)├──[330Ω]──┐ │  GPIO 15 ├──o───\ o─── GND (Btn 2: Pause)
    │          │          │ │          │
    │1/OE(pin1)├──GND     │ │   GND    ├──┐
    │ GND(pin7)├──GND     │ └──────────┘  │
    └──────────┘          │               │
                          ▼               │
                    Module 0 DIN          │
                          │               │
                          └───────┬───────┘
                                  │
                             Common GND
```

### Components
- **Logic Level Shifter**: Texas Instruments `74AHCT125` (Quad Bus Buffer with 3-State Outputs).
- **Damping Resistor**: `330 Ω`, 1/4 W (reduces transmission line reflections on high-speed 800 kHz signal).
- **Buttons**: Normally-open SPST momentary tactile pushbuttons.

---

## 2. High-Current Power Distribution Block

```text
 [G-Power 5V 100A PSU]
       │       │
      +5V     GND
   (AWG 8) (AWG 8)
       │       │
       ▼       ▼
 ┌──────────┐ ┌──────────┐
 │ +5V BUS  │ │ GND BUS  │
 └────┬─────┘ └────┬─────┘
      │            │
      ├─[Fuse 3A]──┼──────> Module 0 (+5V / GND) [+ 1000µF Cap]
      ├─[Fuse 3A]──┼──────> Module 1 (+5V / GND) [+ 1000µF Cap]
      ├─[Fuse 3A]──┼──────> Module 2 (+5V / GND) [+ 1000µF Cap]
      │    ...     │              ...
      └─[Fuse 3A]──┴──────> Module 14 (+5V / GND) [+ 1000µF Cap]
```

### Design Rules
1. **Parallel Distribution**: Never run supply current in series through multiple panels.
2. **Branch Fuses**: Protect every single module branch line with a 3 A fast-acting mini blade fuse.
3. **Decoupling Capacitors**: Place a 1,000 µF 10 V electrolytic capacitor across the power leads directly at each module.
