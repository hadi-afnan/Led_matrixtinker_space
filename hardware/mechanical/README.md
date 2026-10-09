# Mechanical Design & 3D Printing

This directory contains specifications, CAD guidelines, and Bambu Lab A1 print profiles for the **TinkerSpace Modular LED Display** mechanical parts.

---

## 📁 Mechanical Subdirectories

- [`module-frame/`](file:///e:/Embedded/Projects/led_metrix/hardware/mechanical/module-frame/README.md): 165.6 × 165.6 mm interlocking module frames with light-baffle grids.
- [`diffuser/`](file:///e:/Embedded/Projects/led_metrix/hardware/mechanical/diffuser/README.md): 0.16 mm and 0.24 mm diffuser sheets, plus 4-column multi-tile sheets.
- [`backplate/`](file:///e:/Embedded/Projects/led_metrix/hardware/mechanical/backplate/README.md): Laser cutting and extrusion mounting templates.
- [`mounting/`](file:///e:/Embedded/Projects/led_metrix/hardware/mechanical/mounting/README.md): Wall mounting brackets and desktop kickstands.

---

## 🖨️ Bambu Lab A1 Recommended Slicer Profiles

| Parameter | Module Frame (Structural) | Diffuser Sheet (Optical) |
| :--- | :--- | :--- |
| **Nozzle Diameter** | 0.4 mm Stainless / Hardened | 0.4 mm or 0.2 mm |
| **Material** | PLA Basic Black / PETG | Bambu PLA Basic Jade White |
| **Layer Height** | 0.20 mm Standard | **0.08 mm High Quality** |
| **First Layer Height** | 0.20 mm | 0.08 mm or 0.10 mm |
| **Infill Density** | 15% – 20% Gyroid | **100% Solid** |
| **Walls / Perimeters**| 3 Loops | 2 Loops |
| **Build Plate** | Textured PEI Plate | Smooth PEI or High Temp Plate |
| **Print Speed** | Normal (100%) | 50% on first layer for perfect adhesion |

> [!TIP]
> Printing diffusers requires an impeccably clean build plate! Clean with dish soap and warm water, followed by 99% Isopropanol (IPA), to prevent fingerprint oil residues from showing up as dark smudges when illuminated.
