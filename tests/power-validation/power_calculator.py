#!/usr/bin/env python3
"""
TinkerSpace Modular LED Display — Electrical Power & Voltage Drop Calculator
=============================================================================
Calculates current requirements, recommended wire gauges, branch fuse ratings,
and estimated voltage drops for modular LED matrices.
"""

# Copper wire resistance per meter at 20°C (ohms/meter)
AWG_RESISTANCE_OHMS_PER_M = {
    6:  0.001296,
    8:  0.002061,
    10: 0.003277,
    12: 0.005211,
    14: 0.008286,
    16: 0.01317,
    18: 0.02095,
    20: 0.03331,
    22: 0.05296,
    24: 0.08422,
}

def analyze_power(modules_x: int, modules_y: int, name: str):
    num_modules = modules_x * modules_y
    total_leds = num_modules * 100

    # WS2812B Electrical Characteristics
    max_current_per_led_ma = 60.0    # 100% White (R=20mA, G=20mA, B=20mA)
    typical_current_per_led_ma = 22.0 # Dynamic mixed animation (plasma/fire/rainbow)
    idle_current_per_led_ma = 1.0    # Quiescent controller IC current (Black/Off)
    voltage = 5.0

    peak_current_a = (total_leds * max_current_per_led_ma) / 1000.0
    typical_current_a = (total_leds * typical_current_per_led_ma) / 1000.0
    idle_current_a = (total_leds * idle_current_per_led_ma) / 1000.0

    peak_power_w = peak_current_a * voltage
    typical_power_w = typical_current_a * voltage

    current_per_module_peak_a = (100 * max_current_per_led_ma) / 1000.0
    current_per_module_typ_a = (100 * typical_current_per_led_ma) / 1000.0

    print(f"\n=======================================================")
    print(f"POWER ANALYSIS: {name}")
    print(f"=======================================================")
    print(f"  Configuration       : {modules_x}x{modules_y} modules ({num_modules} total)")
    print(f"  Total WS2812B LEDs  : {total_leds}")
    print(f"  Theoretical Peak    : {peak_current_a:.1f} A ({peak_power_w:.1f} W)")
    print(f"  Typical Dynamic Load: {typical_current_a:.1f} A ({typical_power_w:.1f} W)")
    print(f"  Idle / Standby Draw : {idle_current_a:.2f} A ({idle_current_a * voltage:.1f} W)")
    print(f"-------------------------------------------------------")
    print(f"Per-Module Branch Load (100 LEDs):")
    print(f"  Module Peak Current : {current_per_module_peak_a:.2f} A")
    print(f"  Module Typical Load : {current_per_module_typ_a:.2f} A")
    print(f"  Recommended Fuse    : 3 A Fast-Blow Blade Fuse (ATO/ATC)")
    print(f"  Decoupling Cap      : 1,000 µF / 10 V Electrolytic")
    print(f"-------------------------------------------------------")

    # PSU Suitability Check (G-Power 5V 100A / 500W)
    psu_rated_current_a = 100.0
    psu_rated_power_w = 500.0
    margin_peak = ((psu_rated_current_a - peak_current_a) / psu_rated_current_a) * 100.0
    margin_typ = ((psu_rated_current_a - typical_current_a) / psu_rated_current_a) * 100.0

    print(f"PSU Suitability (G-Power 5V / 100A / 500W):")
    print(f"  Load Margin at Theoretical 100% White : {margin_peak:+.1f}% headroom")
    print(f"  Load Margin at Typical Dynamic Use    : {margin_typ:+.1f}% headroom")
    if peak_current_a <= psu_rated_current_a:
        print(f"  PSU Verdict         : SAFE & ADEQUATE (Within 100A rating)")
    else:
        print(f"  PSU Verdict         : WARNING (Exceeds 100A rating without current limit)")


def calculate_voltage_drop(wire_awg: int, length_meters: float, current_amps: float):
    if wire_awg not in AWG_RESISTANCE_OHMS_PER_M:
        return None
    r_per_m = AWG_RESISTANCE_OHMS_PER_M[wire_awg]
    # Round-trip loop (both positive and ground conductors)
    total_resistance = 2.0 * length_meters * r_per_m
    v_drop = current_amps * total_resistance
    v_delivered = 5.0 - v_drop
    pct_drop = (v_drop / 5.0) * 100.0
    return v_drop, v_delivered, pct_drop


def print_voltage_drop_table():
    print(f"\n=======================================================")
    print(f"VOLTAGE DROP SAMPLES (Round-Trip 5.0 V Supply)")
    print(f"=======================================================")
    print(f"{'Segment':<32} {'AWG':<5} {'Len (m)':<8} {'Current':<8} {'Drop (V)':<9} {'Delivered':<10} {'Status'}")
    print("-" * 80)

    scenarios = [
        ("Main Busbar Feed from PSU", 8, 1.0, 50.0),
        ("Main Busbar Feed from PSU", 10, 1.0, 50.0),
        ("Module Branch Wire (AWG 18)", 18, 0.5, 3.0),
        ("Module Branch Wire (AWG 20)", 20, 0.5, 3.0),
        ("Daisy Chain through 3 modules", 24, 0.5, 9.0),
    ]

    for name, awg, length, current in scenarios:
        v_drop, v_del, pct = calculate_voltage_drop(awg, length, current)
        status = "ACCEPTABLE (<5%)" if pct < 5.0 else "DANGEROUS / HIGH"
        print(f"{name:<32} {awg:<5} {length:<8.1f} {current:<8.1f} {v_drop:<9.3f} {v_del:<10.2f} {status}")


if __name__ == "__main__":
    analyze_power(5, 3, "Prototype 5x3 Layout (1,500 LEDs)")
    analyze_power(4, 3, "Target 4:3 Layout (1,200 LEDs)")
    print_voltage_drop_table()
