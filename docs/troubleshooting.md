# Troubleshooting & Diagnostic Guide

This guide provides systematic troubleshooting procedures for common electrical, firmware, mechanical, and optical issues encountered with the **TinkerSpace Modular LED Display**.

---

## 1. Quick Diagnostic Table

| Symptom | Probable Cause | Corrective Action |
| :--- | :--- | :--- |
| **Random flickering or white flash bursts** | Missing or faulty 3.3V to 5V level shifter; missing common ground. | Ensure 74AHCT125 is installed. Confirm common ground wire links ESP32-S3 GND and PSU negative rail. |
| **First module works; subsequent modules are off** | Broken solder joint on `DOUT` pad of the working module or `DIN` of the next. | Inspect solder joint between modules with a magnifying glass. Re-flow joint with fresh solder. |
| **Colors shift towards dull orange/red downstream** | Severe voltage drop across power rail (under 3.8 V). | Inspect branch fuses and wire connections. Verify parallel power injection is reaching all modules. |
| **ESP32-S3 reboots constantly (Brownout Detector)** | Inrush current dipping the 5 V rail below 4.5 V during boot. | Install a 1,000 µF capacitor across the ESP32 5V/GND pins; ensure PSU is set to 5.05 V. |
| **Patterns appear fragmented or zig-zagged** | Serpentine mapping mismatch in firmware. | Run `tests/led-mapping/validate_mapping.py` to verify mathematical mapping matches your physical wiring direction. |
| **Buttons trigger erratic or multiple mode changes** | Contact bounce or missing internal pull-up. | Verify pins are set to `INPUT_PULLUP`. Increase debounce threshold in `buttons.cpp` from 50 ms to 80 ms. |
| **PSU trips or makes ticking/clicking sounds** | Direct short circuit between +5 V and GND. | Immediately disconnect AC power. Check busbars and module injection points for dead shorts with a multimeter. |
| **LEDs display visible hot spots through diffuser** | Diffuser sheet too thin or air gap too small. | Switch from 0.16 mm to 0.24 mm diffuser sheet, or increase spacer height between LEDs and diffuser by 2 mm. |

---

## 2. Isolating a Failed LED in the Serial Chain

Because WS2812B LEDs operate in a serial chain ($DIN \rightarrow DOUT$), if pixel $N$ suffers internal silicon failure, all pixels downstream ($N+1$ to 1500) will receive no data and remain dark.

### The Bypass Diagnostic Method:
1. Locate the boundary between the last illuminated LED and the first dark LED.
2. If the boundary is between two physical modules:
   - Check the inter-module jumper wire with a multimeter.
   - Solder a temporary test wire directly from the ESP32 level shifter output to the `DIN` pad of the dark module.
   - If the second module now illuminates, the fault is located at the `DOUT` pad or final LED of the preceding module.
3. If the boundary is inside a single module:
   - You can replace individual SMD 5050 WS2812B chips with hot air, or simply swap that single 10×10 module without affecting the rest of the display.

---

## 3. Voltage Drop Field Measurements

To verify whether your power bus is healthy, perform live measurements with a DMM across the matrix while running the `All White` test pattern at 50% brightness:

```
[Module 0 Injection Pad]  ──> Should measure: 4.85 V – 5.05 V
[Module 7 Injection Pad]  ──> Should measure: 4.75 V – 5.00 V
[Module 14 Injection Pad] ──> Should measure: 4.75 V – 5.00 V
```

- If voltage at any module drops **below 4.2 V**, that module's power branch is inadequate.
- Check for high-resistance solder joints, loose screw terminals on the busbar, or undersized branch wires.

---

## 4. Software Diagnostic Checklist

1. **Serial Monitor Output**: Open the serial monitor at **115200 baud**. Confirm the boot banner displays the expected module count and layout mode.
2. **Step Through Diagnostic Sketches**:
   - Run `firmware/arduino/examples/01_single_module_test`: Tests one module in isolation.
   - Run `firmware/arduino/examples/02_matrix_serpentine_test`: Sends a single white dot crawling through all 1,500 pixels in numerical order, visually confirming your serpentine routing.
   - Run `firmware/arduino/examples/03_button_input_test`: Prints raw button press events to ensure tactile switches are functioning cleanly.
