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

Open [`firmware/arduino/led_display/config.h`](file:///e:/Embedded/Projects/led_metrix/firmware/arduino/led_display/config.h). The firmware is built with compile-time configuration flags:

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

### Wi-Fi Configuration
```cpp
#define WIFI_AP_SSID        "TinkerSpace-LED" // Access point name for direct phone connection
#define WIFI_AP_PASSWORD    "makeanything"    // WPA2 passphrase (min 8 chars); "" for open
#define MDNS_HOSTNAME       "tinkerspace"     // Accessible at http://tinkerspace.local
```

---

## 5. Compiling and Flashing

1. Open [`firmware/arduino/led_display/led_display.ino`](file:///e:/Embedded/Projects/led_metrix/firmware/arduino/led_display/led_display.ino).
2. Click the **Verify** checkmark button to ensure compilation succeeds.
3. Click the **Upload** arrow button.
4. *If upload fails to initiate automatically*: Hold down the **BOOT** button on the ESP32-S3, tap **RST/EN**, then release **BOOT** to force ROM download mode, and click Upload again.

---

## 6. Accessing the Web Application

Once flashed, the ESP32-S3 automatically spins up an Access Point, a Captive Portal DNS server, and an embedded web server:

### Connecting from Phone, Tablet, or PC
1. On your device, look for the Wi-Fi network named **`TinkerSpace-LED`**.
2. Enter password: **`makeanything`**.
3. **Handle Device Network Alerts**:
   - **Android**: A notification will state *"Wi-Fi has no internet access"*. Tap it and select **"Keep Wi-Fi Connection"** / **"Stay Connected"** (or turn off Mobile Data).
   - **iOS / iPhone**: The captive portal login assistant will open automatically. If dismissed, tap **"Use Without Internet"**.
4. Open your web browser and navigate to:
   ```text
   http://192.168.4.1
   ```
   Or if connected to the same local network:
   ```text
   http://tinkerspace.local
   ```
5. The responsive **TinkerSpace LED Web Controller** loads immediately!

### Key Web App Features
- **Space Signs & Multi-Line Text**: Display 3 distinct lines of text, big headers, scrolling marquees, or status badges with custom colors, alignments, and scrolling/blinking effects.
- **Quick Space Presets**: 1-click makerspace actions ("SPACE IS OPEN", "WORKSHOP IN PROGRESS", "3D PRINTERS ACTIVE", "LASER CUTTER BUSY", "COFFEE BREAK", "PIZZA TIME").
- **Image Studio**: Upload any image or logo; auto-downsampled and Floyd-Steinberg dithered for 50×30 pixels.
- **Pixel Art Studio**: Draw pixel-by-pixel with pencil, eraser, and paint bucket with live sync.
- **Animations**: Generative WLED-style patterns (Fire, Plasma, Meteor, Rainbow, Matrix Rain, Module Sweep).
- **Hardware Settings**: Toggle between row-serpentine and modular tile-serpentine topology, run diagnostics, and connect to local makerspace Wi-Fi.

---

## 7. Serial Monitor Diagnostics

Open the Arduino Serial Monitor (**Tools** > **Serial Monitor**) set to **115200 baud**. Upon boot, the firmware outputs:

```text
==================================================
   TINKERSPACE MODULAR LED DISPLAY CONTROLLER     
==================================================
Layout Configured : 5x3 Prototype (50x30)
Grid Resolution   : 50 x 30
Total LED Count   : 1500
Data Output Pin   : GPIO 4
Button 1 (Next)   : GPIO 14
Button 2 (Pause)  : GPIO 15
Power Safety Cap  : 25000 mA @ 5 V
--------------------------------------------------
Web App URL (Access Point) : http://192.168.4.1
Web App URL (Local Network): http://tinkerspace.local
--------------------------------------------------
Serial Commands: 'n'=next, 'p'=pause, '+'=brighter, '-'=dimmer
==================================================
```
