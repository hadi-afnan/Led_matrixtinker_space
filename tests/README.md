# Offline Validation & Simulation Tools

This directory contains standalone Python test suites and engineering calculators to validate matrix coordinate mapping and electrical power distribution before committing to physical assembly or firmware uploads.

---

## 🛠️ Included Tools

### 1. LED Mapping Validator ([`led-mapping/validate_mapping.py`](file:///e:/Embedded/Projects/led_metrix/tests/led-mapping/validate_mapping.py))
- Validates the two-stage serpentine coordinate translation logic for both 5×3 and 4×3 configurations.
- Checks mathematical bijection (zero duplicate LED indices, zero missed pixels).
- Generates ASCII topology maps to visually inspect data flow.

```bash
python tests/led-mapping/validate_mapping.py
```

### 2. Electrical Power Calculator ([`power-validation/power_calculator.py`](file:///e:/Embedded/Projects/led_metrix/tests/power-validation/power_calculator.py))
- Calculates theoretical peak current, realistic animation current, and wire voltage drop for given wire gauges (AWG) and lengths.
- Sizing tool for power supply headroom and branch fuses.

```bash
python tests/power-validation/power_calculator.py
```
