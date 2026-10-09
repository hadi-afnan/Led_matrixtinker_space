# Wiring & Electrical Connections Guide

This document describes the complete wiring configuration for the **TinkerSpace Modular LED Display**, including the ESP32-S3 pinouts, logic-level shifting, tactile control buttons, and inter-module serial data routing.

---

## 1. Pin Assignment Summary

| ESP32-S3 Pin | Function | External Component | Connection Details |
| :--- | :--- | :--- | :--- |
| **GPIO 4** | Data Out | 74AHCT125 Level Shifter | Pin 2 (`1A` Input) |
| **GPIO 14** | Button 1 (Next Animation) | Tactile Switch 1 | Connects to Switch; other terminal to GND (`INPUT_PULLUP`) |
| **GPIO 15** | Button 2 (Pause/Resume) | Tactile Switch 2 | Connects to Switch; other terminal to GND (`INPUT_PULLUP`) |
| **5V / VIN** | Microcontroller Power | 5 V DC Bus / USB-C | Regulated 5.0 V from power distribution block |
| **GND** | System Ground | Common Ground | Tied to PSU Ground, Level Shifter GND, and LED GND |

---

## 2. Logic Level Shifting (3.3 V to 5.0 V)

### Why Level Shifting is Mandatory
The WS2812B protocol requires an input high voltage ($V_{IH}$) of at least **$0.7 \times V_{DD}$**. When powered by 5.0 V:
$$V_{IH(\min)} = 0.7 \times 5.0\text{ V} = 3.5\text{ V}$$

The ESP32-S3 outputs a maximum logic HIGH of **3.3 V**. Driving the LED data line directly with 3.3 V leads to:
- Random flickering or strobe flashes.
- Downstream color corruption and skipped frames.
- Sensitivity to cable length and RF interference.

### Recommended Circuit: 74AHCT125
Use a high-speed CMOS non-inverting quad buffer (such as the **Texas Instruments 74AHCT125** or **74HCT125**):

```
             74AHCT125 IC
            ┌────────────┐
    GND ───1┤ 1/OE   VCC ├14─── +5V (from clean 5V rail)
ESP32 GPIO4─2┤ 1A     2/OE├13─── +5V (disabled)
330Ω Resistor3┤ 1Y       2A ├12
    │       4┤ 2Y       2Y ├11
    │       5┤ 3A     3/OE├10
    │       6┤ 3Y       3A ├9
    │       7┤ GND      3Y ├8
    │       └────────────┘
    ▼
Module 0 DIN ─── (First LED in chain)
```

> [!TIP]
> **Damping Resistor**: Place a **330 Ω to 470 Ω** metal film resistor directly in series between the `1Y` output pin and the first LED's `DIN` pad. This dampens high-frequency reflections and prevents ringing on the 800 kHz data line.

---

## 3. Pushbutton Input Wiring

Both tactile buttons utilize the ESP32-S3's internal pull-up resistors (`pinMode(PIN, INPUT_PULLUP)`):

```
  ESP32-S3 GPIO 14 ────o──────\ o───── System GND
                      (Tactile Button 1: Next Mode)

  ESP32-S3 GPIO 15 ────o──────\ o───── System GND
                      (Tactile Button 2: Pause / Resume)
```

- When the button is **open**: Internal pull-up holds the pin at logic HIGH (3.3 V).
- When the button is **pressed**: Pin is pulled directly to GND (logic LOW, 0 V).
- The firmware incorporates an active software debounce routine (`buttons.cpp`) with a 50 ms filter window.

---

## 4. Module-to-Module Serpentine Data Wiring

The data signal travels through the matrix in a continuous horizontal serpentine path. Each module has:
- `DIN` (Data In)
- `DOUT` (Data Out)
- `+5V` (Power)
- `GND` (Ground)

```
[Row 0 - Left to Right]
Module 0 (DOUT) ──> Module 1 (DIN) ──> Module 2 (DIN) ──> Module 3 (DIN) ──> Module 4 (DIN)
                                                                                  │
                                                                       (Jumper cable down)
                                                                                  │
[Row 1 - Right to Left]                                                           ▼
Module 9 (DIN) <── Module 8 (DIN) <── Module 7 (DIN) <── Module 6 (DIN) <── Module 5 (DIN)
      │
(Jumper down)
      │
      ▼
[Row 2 - Left to Right]
Module 10 (DIN) ──> Module 11 (DIN) ──> Module 12 (DIN) ──> Module 13 (DIN) ──> Module 14 (DIN)
```

### Best Practices for Inter-Module Data Jumpers
1. **Keep jumpers short**: Maintain data lines between modules under 10 cm (4 inches) to preserve signal integrity.
2. **Twist with Ground**: When running long data runs between rows, run a ground wire twisted alongside the data wire to minimize inductive pickup.
3. **Flexible wire**: Use **24 AWG or 26 AWG silicone-insulated stranded wire** to prevent fatigue fractures during module installation.

---

## 5. Main Power Switch & AC Mains Wiring

> [!CAUTION]
> AC mains wiring (110 V / 230 V) carries lethal voltages. Always insulate all AC terminal screws with heat shrink or terminal shrouds.

1. **AC Mains Input**:
   - `L` (Live / Line) passes through the illuminated DPST rocker switch before reaching the power supply terminal.
   - `N` (Neutral) connects to the PSU Neutral terminal.
   - `PE / Earth Ground` connects directly to the chassis ground screw on the G-Power power supply.
2. **DC Output (5 V)**:
   - Positive (+5 V) connects directly to the fused distribution busbar.
   - Negative (GND) connects directly to the negative ground busbar.
