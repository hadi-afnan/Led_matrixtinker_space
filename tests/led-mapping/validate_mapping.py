#!/usr/bin/env python3
"""
TinkerSpace Modular LED Display — Coordinate Mapping Validator
==============================================================
Validates that the mathematical two-stage serpentine coordinate translation
is a true 1-to-1 bijection (every (x, y) maps to a unique index with no overlaps
and no missing pixels).
"""

import sys

def get_pixel_index(x: int, y: int, modules_x: int, modules_y: int) -> int:
    module_size = 10
    total_width = modules_x * module_size
    total_height = modules_y * module_size

    # Boundary check
    if x < 0 or x >= total_width or y < 0 or y >= total_height:
        return -1

    module_x = x // module_size
    module_y = y // module_size

    local_x = x % module_size
    local_y = y % module_size

    # Inter-Module Serpentine:
    # Even rows (0, 2): Left to Right
    # Odd rows (1): Right to Left
    if (module_y % 2) == 0:
        module_index = (module_y * modules_x) + module_x
    else:
        module_index = (module_y * modules_x) + (modules_x - 1 - module_x)

    # Intra-Module Serpentine (inside standard 10x10 panel):
    # Even rows: Left to Right
    # Odd rows: Right to Left
    if (local_y % 2) == 0:
        pixel_in_module = (local_y * module_size) + local_x
    else:
        pixel_in_module = (local_y * module_size) + (module_size - 1 - local_x)

    leds_per_module = module_size * module_size
    return (module_index * leds_per_module) + pixel_in_module


def test_layout(name: str, modules_x: int, modules_y: int):
    width = modules_x * 10
    height = modules_y * 10
    total_leds = width * height

    print(f"\n=======================================================")
    print(f"Testing Layout: {name} ({modules_x}x{modules_y} modules, {width}x{height} = {total_leds} LEDs)")
    print(f"=======================================================")

    mapped_indices = set()
    duplicates = []
    out_of_bounds = []

    for y in range(height):
        for x in range(width):
            idx = get_pixel_index(x, y, modules_x, modules_y)
            if idx < 0 or idx >= total_leds:
                out_of_bounds.append((x, y, idx))
            elif idx in mapped_indices:
                duplicates.append((x, y, idx))
            else:
                mapped_indices.add(idx)

    # Verification results
    print(f"  Total Pixels Checked  : {width * height}")
    print(f"  Unique Indices Mapped : {len(mapped_indices)}")
    print(f"  Duplicate Collisions  : {len(duplicates)}")
    print(f"  Out of Bounds Errors  : {len(out_of_bounds)}")

    # Check boundaries outside display
    assert get_pixel_index(-1, 0, modules_x, modules_y) == -1, "Left bound failed"
    assert get_pixel_index(width, 0, modules_x, modules_y) == -1, "Right bound failed"
    assert get_pixel_index(0, -1, modules_x, modules_y) == -1, "Top bound failed"
    assert get_pixel_index(0, height, modules_x, modules_y) == -1, "Bottom bound failed"
    print("  Boundary Checks       : PASS")

    if len(mapped_indices) == total_leds and len(duplicates) == 0:
        print(f"  Result                : SUCCESS! Perfect 1:1 Bijection.")
        return True
    else:
        print(f"  Result                : FAILED! Mapping contains errors.")
        return False


def print_ascii_topology(modules_x: int, modules_y: int):
    print("\nModule Routing Topology:")
    for my in range(modules_y):
        row_str = f"Row {my}: "
        if my % 2 == 0:
            mods = [f"[Mod {(my * modules_x) + mx:02d}]" for mx in range(modules_x)]
            row_str += " ---> ".join(mods)
        else:
            mods = [f"[Mod {(my * modules_x) + (modules_x - 1 - mx):02d}]" for mx in range(modules_x)]
            row_str += " <--- ".join(mods)
        print("  " + row_str)


if __name__ == "__main__":
    t1 = test_layout("Prototype 5x3", 5, 3)
    print_ascii_topology(5, 3)

    t2 = test_layout("Target 4x3", 4, 3)
    print_ascii_topology(4, 3)

    if t1 and t2:
        print("\nAll LED mapping unit tests passed successfully!\n")
        sys.exit(0)
    else:
        sys.exit(1)
