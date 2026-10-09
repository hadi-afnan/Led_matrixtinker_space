# Optical Diffuser Specifications & Test Profiles

The optical diffuser converts harsh, point-source WS2812B SMD LEDs into smooth, uniform pixel squares. Diffuser thickness directly impacts the trade-off between light transmission (brightness) and optical diffusion (spot-blending).

---

## 🔬 Experimental Diffuser Variants

All diffuser tests were sliced specifically for the **Bambu Lab A1** using Bambu PLA Basic Jade White.

### Variant A: 0.16 mm Ultrathin Diffuser
- **Layer Construction**: 2 printed layers @ 0.08 mm layer height.
- **Light Transmission**: Very high (~85% transmission).
- **Diffusion Quality**: Moderate. LED emitter dies ("hotspots") remain faintly visible at close range.
- **Mechanical Properties**: Highly flexible; fragile during removal from the print bed. Requires slow, gentle peel.

### Variant B: 0.24 mm Balanced Diffuser (Recommended)
- **Layer Construction**: 3 printed layers @ 0.08 mm layer height.
- **Light Transmission**: Balanced (~70% transmission).
- **Diffusion Quality**: High. Hotspots are effectively softened into square pixel glows.
- **Mechanical Properties**: Improved dimensional stiffness; resists curling and warping.

---

## 🧩 Diffuser Geometry & Multi-Tile Layout

Two panel formats are provided for the Bambu Lab A1 (256 × 256 mm build plate):

### 1. Single Module Sheets
- **Dimensions**: 165.6 mm × 165.6 mm
- **Usage**: Snaps directly into an individual 10×10 frame.
- **Advantage**: Modular replacement; individual modules can be serviced without disturbing neighbors.

### 2. Multi-Module Extended Tiles
- **Dimensions**: **207.0 mm × 165.6 mm**
- **Grid Layout**: Arranged in **4 columns × 3 rows** to span the nominal 828 × 496.8 mm prototype panel.
- **Advantage**: Bridges module seams, creating a seamless unified glass-like front face.

---

## ⚠️ Known Gotchas & Testing Notes

1. **Bed Adhesion & Warping**: Thin, large-area sheets are susceptible to edge lifting. Set the bed temperature to 60°C – 65°C and ensure no draft reaches the printer.
2. **First Layer Uniformity**: Even a 0.02 mm deviation in bed mesh level will cause visible thickness gradients. Run full bed leveling before printing diffuser sheets.
3. **Infill Shadows**: Never use infill on diffusers! Slicing must enforce **100% solid concentric or rectilinear infill**.
