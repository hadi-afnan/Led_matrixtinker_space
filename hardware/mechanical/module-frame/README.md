# 10×10 Module Frame Specifications

The module frame is the fundamental building block of the display, designed to hold a single 10×10 flexible WS2812B LED panel.

---

## 📐 Dimensions & Tolerances

- **Outer Footprint**: 165.6 mm × 165.6 mm
- **Internal Pocket**: Sized for standard 100-pixel flexible FPCB (approx. 160 mm active matrix area).
- **Height / Depth**: 18.0 mm total depth:
  - 12.0 mm rear chamber for cable routing and capacitor clearance.
  - 4.0 mm optical baffle depth to prevent light cross-talk between adjacent LEDs.
  - 2.0 mm top lip for snapping in the optical diffuser.
- **Interlocking System**: Male/female dovetail tabs and M3 clearance holes on all four perimeter edges.

---

## 🖨️ Printing Guidelines

- **Recommended Filament**: Black PLA or PETG. Black prevents internal reflection and maintains high contrast between adjacent pixels.
- **Quantity**:
  - **15 frames** for 5×3 Prototype.
  - **12 frames** for 4×3 Target.
- **Estimated Print Time**: ~2h 15m per frame on Bambu Lab A1.
- **Estimated Weight**: ~75 g per frame.

---

## 💡 Assembly Tips

1. Press-fit brass M3 heat-set threaded inserts into the perimeter holes using a soldering iron set to 230°C.
2. Route the 5V and GND wires through the bottom cutouts before screwing neighboring modules together.
