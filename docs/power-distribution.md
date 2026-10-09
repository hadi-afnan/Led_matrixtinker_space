# Power Distribution & Electrical Safety Guide

> [!CAUTION]
> **HIGH CURRENT ELECTRICAL HAZARD**  
> A 5 V / 100 A power supply can supply 500 Watts of continuous power. In the event of an accidental short circuit, an unprotected 100 A supply behaves like an arc welder: small gauge wires can instantly vaporize, catch fire, or fuse together.  
> **Always fuse individual branch circuits, never bypass current limits, and use appropriately sized conductors.**

---

## 1. Power Consumption Calculations

Each WS2812B pixel contains red, green, and blue LED dies with an internal constant-current driver IC:
- **Red channel**: ~20 mA at 100% brightness.
- **Green channel**: ~20 mA at 100% brightness.
- **Blue channel**: ~20 mA at 100% brightness.
- **Quiescent current (idle/black)**: ~1 mA per pixel.

### Current Draw Scenarios

| Setup | Pixel Count | Theoretical Max (100% White) | Typical Dynamic Animation | Idle / Standby |
| :--- | :---: | :---: | :---: | :---: |
| **Single 10×10 Module** | 100 | **6.0 A** (30 W) | **1.8 A – 2.8 A** (10 W) | ~0.1 A (0.5 W) |
| **4:3 Target (12 Modules)** | 1,200 | **72.0 A** (360 W) | **22.0 A – 35.0 A** (140 W) | ~1.2 A (6.0 W) |
| **5:3 Prototype (15 Modules)**| 1,500 | **90.0 A** (450 W) | **28.0 A – 44.0 A** (175 W) | ~1.5 A (7.5 W) |

### Software Safety Limits
In our production firmware (`config.h`), we enforce dynamic power limits via FastLED's internal power management:
```cpp
FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT_MA); // Default: 25,000 mA (25 A)
```
This automatically scales global brightness if an animation requests more current than the target power envelope.

---

## 2. Why Power Daisy-Chaining is Prohibited

Flexible FPCB matrix panels use copper traces between 0.5 oz (18 µm) and 1.0 oz (35 µm) thickness. These micro-thin traces have significant electrical resistance.

### The Consequences of Daisy-Chaining:
1. **Fire & Delamination**: Forcing 15–30 A through the edge traces of Module 0 will overheat the flexible substrate, melting the adhesive and igniting surrounding 3D-printed PLA.
2. **Severe Voltage Drop**: By Module 3, a 5.0 V rail will drop to 3.2 V or lower. Below 3.5 V, blue and green LEDs fail to illuminate, turning animations a degraded pink/brown color, and the internal ICs will freeze or lock up.

```
WRONG (Daisy-Chain):
[PSU 5V] ──> [Mod 0] ──> [Mod 1] ──> [Mod 2] ──> [Mod 3]  (DANGEROUS RESISTIVE HEATING!)

CORRECT (Parallel Star / Busbar):
                ┌──[Fuse 3A]──> [Mod 0]
                ├──[Fuse 3A]──> [Mod 1]
[PSU 5V Bus] ───┼──[Fuse 3A]──> [Mod 2]
                └──[Fuse 3A]──> [Mod 3]
```

---

## 3. Recommended Power Distribution Architecture

```mermaid
graph TD
    PSU[G-Power 5V 100A PSU] -->|AWG 8/10 Main Feed| MainSwitch[High-Current Switch / Busbar]
    
    subgraph "Positive Distribution (+5V)"
        MainSwitch --> BusbarPos[Heavy Copper +5V Busbar]
        BusbarPos --> Fuse1[Blade Fuse 3A] --> Mod0[Module 0 (+5V)]
        BusbarPos --> Fuse2[Blade Fuse 3A] --> Mod1[Module 1 (+5V)]
        BusbarPos --> FuseN[Blade Fuse 3A] --> ModN[Module N (+5V)]
    end

    subgraph "Negative Distribution (GND)"
        PSU --> BusbarGND[Heavy Copper GND Busbar]
        BusbarGND --> Mod0GND[Module 0 (GND)]
        BusbarGND --> Mod1GND[Module 1 (GND)]
        BusbarGND --> ModNGND[Module N (GND)]
        BusbarGND --> ESPGND[ESP32-S3 & Shifter GND]
    end
```

---

## 4. Conductor Sizing (Wire Gauge Selection)

To keep voltage drop under **3% (0.15 V)** across distribution runs:

| Conductor Segment | Expected Peak Current | Recommended Wire Gauge | Resistance (mΩ/m) |
| :--- | :---: | :---: | :---: |
| **Main PSU to Busbars** | 60 A – 90 A | **AWG 6 to AWG 8** (Multi-strand) | ~1.3 mΩ/m |
| **Busbar to 6-Way Fuse Box** | 20 A – 30 A | **AWG 10 to AWG 12** | ~3.3 mΩ/m |
| **Branch from Fuse to Module**| 3.0 A – 5.0 A | **AWG 18 to AWG 20** | ~21.0 mΩ/m |
| **Data Jumpers between Modules**| < 10 mA | **AWG 24 to AWG 26** | N/A (signal only) |

---

## 5. Branch Fusing Specifications

Every single 10×10 module (or paired module cluster) **must** have dedicated overcurrent protection:
- **Fuse Type**: Automotive mini blade fuses (ATM / ATC) or glass cartridge fuses.
- **Rating**: **3 A (Violet) or 5 A (Tan/Orange)** fast-blow.
- **Placement**: Fuse must be placed on the positive line within 10 cm of the busbar.

If a short circuit occurs anywhere in module $N$, its 3 A fuse blows in under 50 milliseconds, isolating the fault without tripping the 100 A power supply or causing heat damage.

---

## 6. Bulk Capacitance (De-coupling)

Addressable LEDs switch high currents at 800 kHz using pulse-width modulation (PWM). This creates rapid di/dt transient spikes that cause data jitter and noise.
- **Requirement**: Solder a **1,000 µF, 6.3 V or 10 V low-ESR electrolytic capacitor** across the `+5V` and `GND` pads directly at each module's power input.
- **Polarity**: Ensure the negative stripe on the capacitor body matches the `GND` pad!
