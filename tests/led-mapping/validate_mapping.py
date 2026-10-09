#!/usr/bin/env python3
"""
TinkerSpace Modular LED Display — Coordinate Mapping Validator
==============================================================
Validates that the mathematical two-stage serpentine coordinate translation
is a true 1-to-1 bijection (every (x, y) maps to a unique index with no overlaps
and no missing pixels).
"""

import sys

def get_pixel_index(x: int, y: int, width: int = 50, height: int = 30) -> int:
    # Boundary check
    if x < 0 or x >= width or y < 0 or y >= height:
        return -1

    # Left to right, bottom to top, row serpentine raster:
    # Origin (0,0) at bottom-left:
    # Row y=0 (bottom row) runs Left -> Right
    # Row y=1 (next row)   runs Right -> Left, alternating upwards
    if (y % 2) == 0:
        return (y * width) + x
    else:
        return (y * width) + (width - 1 - x)


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
            idx = get_pixel_index(x, y, width, height)
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
    assert get_pixel_index(-1, 0, width, height) == -1, "Left bound failed"
    assert get_pixel_index(width, 0, width, height) == -1, "Right bound failed"
    assert get_pixel_index(0, -1, width, height) == -1, "Top bound failed"
    assert get_pixel_index(0, height, width, height) == -1, "Bottom bound failed"
    print("  Boundary Checks       : PASS")

    # Print Corner Indices for physical verification
    print(f"  Corner Indices Check  :")
    print(f"    Bottom-Left  (0, 0)   -> Index {get_pixel_index(0, 0, width, height):4d}  [First LED / DIN]")
    print(f"    Bottom-Right ({width-1}, 0)  -> Index {get_pixel_index(width-1, 0, width, height):4d}  [End of bottom row]")
    print(f"    Top-Left     (0, {height-1})  -> Index {get_pixel_index(0, height-1, width, height):4d}")
    print(f"    Top-Right    ({width-1}, {height-1}) -> Index {get_pixel_index(width-1, height-1, width, height):4d}")

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
