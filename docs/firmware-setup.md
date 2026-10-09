# Firmware Setup & Installation Guide

This guide details how to configure your development environment, install required libraries, configure the hardware options, and flash the ESP32-S3 microcontroller.

---

## 1. Development Environment Options

You can build and flash the firmware using either the **Arduino IDE (2.x)** or **VS Code + PlatformIO**.

---

## 2. Arduino IDE Setup (Recommended for Beginners)

### Step 1: Install Arduino IDE
Download and install [Arduino IDE 2.3+](https://www.arduino.cc/en/software).

### Step 2: Install ESP32 Board Package
1. Open **File** > **Preferences**.
2. In the **Additional boards manager URLs** field, paste:
   ```text
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Click **OK**.
4. Navigate to **Tools** > **Board** > **Boards Manager**.
5. Search for `esp32` and install the package by **Espressif Systems** (version 2.0.14 or newer / 3.0+).

### Step 3: Install FastLED Library
1. Navigate to **Tools** > **Manage Libraries...** (or press `Ctrl+Shift+I`).
2. Search for `FastLED`.
3. Install **FastLED** by Daniel Garcia (Version **3.6.0** or newer).

---

## 3. ESP32-S3 Board Settings in Arduino IDE

Connect your ESP32-S3 to your computer using a data-capable USB-C cable. Under the **Tools** menu, configure the following options:

| Setting | Value | Notes |
| :--- | :--- | :--- |
| **Board** | `ESP32S3 Dev Module` | Standard generic ESP32-S3 profile |
| **Port** | Select active COM port | e.g., `COM5` (Windows) or `/dev/ttyACM0` (Linux) |
| **USB CDC On Boot** | **Enabled** | **Critical**: Enables `Serial.print()` on native USB |
| **Flash Size** | `8MB (64Mb)` or `16MB` | Match your ESP32-S3 module specs |
| **Partition Scheme** | `Huge APP (3MB No OTA)` | Provides generous code space for animations |
| **Upload Speed** | `921600` | High-speed flashing |
| **Core Debug Level** | `None` (or `Info` for debug)| Keep at None for production speed |

---

## 4. Hardware Configuration (`config.h`)

Open [`firmware/arduino/led-display/config.h`](file:///e:/Embedded/Projects/led_metrix/firmware/arduino/led-display/config.h). The firmware is built with compile-time configuration flags:

### Selecting the Display Layout
Uncomment the target layout macro that matches your physical hardware:

```cpp
// Choose one active layout:
#define CONFIG_LAYOUT_5X3    // Physical Prototype: 50x30 pixels (1,500 LEDs)
// #define CONFIG_LAYOUT_4X3 // Target 4:3 Design: 40x30 pixels (1,200 LEDs)
```

### Pin Assignments
Verify your GPIO connections:
```cpp
#define PIN_LED_DATA     4   // ESP32-S3 GPIO connected to Level Shifter 1A
#define PIN_BTN_NEXT    14   // Tactile Button 1 (Next pattern, to GND)
#define PIN_BTN_PAUSE   15   // Tactile Button 2 (Pause/resume, to GND)
```

### Safety & Power Envelope
Adjust current limits to match your power wiring safety profile:
```cpp
#define MAX_BRIGHTNESS      160     // Global brightness scale (0 - 255)
#define CURRENT_LIMIT_MA    25000   // FastLED dynamic limiter: 25 A (125 W)
```

---

## 5. Compiling and Flashing

1. Open [`firmware/arduino/led-display/led-display.ino`](file:///e:/Embedded/Projects/led_metrix/firmware/arduino/led-display/led-display.ino).
2. Click the **Verify** checkmark button to ensure compilation succeeds.
3. Click the **Upload** arrow button.
4. *If upload fails to initiate automatically*: Hold down the **BOOT** button on the ESP32-S3, tap **RST/EN**, then release **BOOT** to force ROM download mode, and click Upload again.

---

## 6. Serial Monitor Diagnostics

Open the Arduino Serial Monitor (**Tools** > **Serial Monitor**) set to **115200 baud**. Upon boot, the firmware outputs:

```text
========================================
 TinkerSpace Modular LED Display v1.0
========================================
Layout: 5x3 Grid (50x30 = 1500 LEDs)
Data Pin: GPIO 4
Button 1 (Next): GPIO 14
Button 2 (Pause): GPIO 15
FastLED Power Limit: 25000 mA @ 5V
System Initialized. Starting Animation 0: Rainbow Wave
```
