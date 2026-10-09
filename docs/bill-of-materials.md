# Bill of Materials (BOM)

This Bill of Materials details all components, fasteners, electronic modules, and 3D printing consumables required to construct the **TinkerSpace Modular LED Display**.

---

## 1. Electronics & LED Components

| Component | Description / Specification | Qty (5×3 Prototype) | Qty (4×3 Target) | Sourcing Notes / Alternatives |
| :--- | :--- | :---: | :---: | :--- |
| **LED Modules** | WS2812B 10×10 flexible FPCB matrix, 5 V | **15 pcs** | **12 pcs** | Standard 165.6 × 165.6 mm FPCB panels |
| **Microcontroller** | ESP32-S3 Dev Board (Dual Core, 8MB Flash) | **1 pc** | **1 pc** | ESP32-S3-DevKitC-1 or clone with native USB |
| **Logic Level Shifter**| 74AHCT125 (Texas Instruments DIP-14) | **1 pc** | **1 pc** | 74HCT125, 74AHCT245, or SN74LVC245A |
| **Damping Resistor** | 330 Ω to 470 Ω, 0.25 W metal film | **1 pc** | **1 pc** | Placed in series with GPIO 4 data line |
| **Tactile Buttons** | 12×12 mm momentary pushbuttons with caps | **2 pcs** | **2 pcs** | Button 1 (Next), Button 2 (Pause) |
| **Main Power Switch** | DPST Rocker Switch (10 A / 250 V AC rated) | **1 pc** | **1 pc** | AC mains isolation with status indicator light |
| **Bulk Capacitors** | 1,000 µF, 6.3 V or 10 V low-ESR electrolytic | **15 pcs** | **12 pcs** | 1 capacitor per 10×10 module power rail |

---

## 2. Power Distribution Hardware

| Component | Description / Specification | Qty (5×3 Prototype) | Qty (4×3 Target) | Sourcing Notes / Alternatives |
| :--- | :--- | :---: | :---: | :--- |
| **Power Supply** | G-Power 5 V / 100 A (500 W max) switching PSU | **1 pc** | **1 pc** | Mean Well LRS-350-5 or RSP-500-5 equivalent |
| **Fuse Blocks** | 6-way or 12-way automotive blade fuse box | **2 blocks** (15 slots) | **2 blocks** (12 slots) | Marine-grade ATO/ATC fuse distribution block |
| **Branch Fuses** | 3 A (Violet) or 5 A (Tan) mini blade fuses | **20 pcs** (incl. spares) | **16 pcs** (incl. spares) | Fast-acting automotive blade fuses |
| **Copper Busbars** | Dual solid copper busbar pair (+5 V & GND) | **1 pair** | **1 pair** | Rated for at least 80 A continuous |
| **Main Power Wire** | AWG 8 or AWG 10 silicone high-strand wire | **2 meters** (Red/Black) | **2 meters** (Red/Black) | Links PSU terminals directly to busbars |
| **Branch Power Wire**| AWG 18 or AWG 20 silicone wire | **15 meters** (Red/Black)| **12 meters** (Red/Black)| Connects fuse outputs to module pads |
| **Signal Wire** | AWG 24 or AWG 26 flexible silicone wire | **5 meters** (Assorted) | **5 meters** (Assorted) | Inter-module data jumpers and button links |

---

## 3. 3D Printing & Mechanical Hardware

| Component | Description / Specification | Qty (5×3 Prototype) | Qty (4×3 Target) | Sourcing Notes / Alternatives |
| :--- | :--- | :---: | :---: | :--- |
| **Module Frames** | 3D-printed interlocking frame (165.6 mm) | **15 prints** | **12 prints** | Bambu PLA Basic Black (~1.2 kg total) |
| **Diffuser Sheets** | 0.16 mm or 0.24 mm flat diffuser tiles | **15 sheets** (or 4 panels)| **12 sheets** | Bambu PLA Basic Jade White (~350 g total) |
| **Controller Case** | 3D-printed electronics housing | **1 set** | **1 set** | Encloses ESP32-S3, buttons, and level shifter |
| **M3 Screws** | M3 × 8 mm & M3 × 12 mm button head hex screws | **60 pcs** | **48 pcs** | Joining adjacent module frames |
| **M3 Threaded Inserts**| M3 × 4 mm brass heat-set threaded inserts | **60 pcs** | **48 pcs** | Melted into printed frame joining holes |
| **Structural Backing** | 6 mm laser-cut acrylic or 2020 aluminum rail | **828 × 496.8 mm** | **662.4 × 496.8 mm** | Provides rigid structural backing for wall mount |

---

## 4. Estimated Cost Summary

| Subsystem | Estimated Cost (USD) |
| :--- | :---: |
| LED Modules (15× WS2812B 10×10 panels) | $75 – $110 |
| Power Supply (5 V / 100 A industrial PSU) | $45 – $65 |
| Microcontroller & Electronics (ESP32-S3, 74AHCT125, buttons) | $12 – $18 |
| Power Distribution (Fuse box, fuses, busbars, capacitors) | $20 – $30 |
| Wire, Hardware & Fasteners (Silicone wire, M3 hardware) | $15 – $25 |
| 3D Printing Filament (1.5 kg PLA) | $25 – $35 |
| **Total Estimated Build Cost** | **~$190 – $280 USD** |
