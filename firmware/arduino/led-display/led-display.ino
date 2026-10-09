/**
 * ============================================================================
 * TinkerSpace Modular LED Display — Main Controller Firmware
 * ============================================================================
 * Controller : ESP32-S3 (Xtensa LX7 @ 240 MHz)
 * Display    : 10x10 WS2812B Addressable RGB LED Modules
 * Grid       : Configurable 5x3 Prototype (1500 LEDs) or 4x3 Target (1200 LEDs)
 * Wiring     : Horizontal Serpentine Layout
 * Control    : Dual Tactile Switches + USB Serial Terminal
 * License    : MIT Open Source License
 * ============================================================================
 */

#include <Arduino.h>
#include <FastLED.h>

#include "config.h"
#include "display_mapping.h"
#include "buttons.h"
#include "animations.h"

// -----------------------------------------------------------------------------
// Global Instances & Buffers
// -----------------------------------------------------------------------------
CRGB leds[TOTAL_LEDS];
DisplayMapping display(leds);
AnimationEngine engine(display, leds);

Button btnNext(PIN_BTN_NEXT);
Button btnPause(PIN_BTN_PAUSE);

uint8_t currentBrightness = DEFAULT_BRIGHTNESS;

// -----------------------------------------------------------------------------
// Helper: Print System Banner to Serial
// -----------------------------------------------------------------------------
void printSystemBanner() {
    Serial.println();
    Serial.println(F("=================================================="));
    Serial.println(F("   TINKERSPACE MODULAR LED DISPLAY CONTROLLER     "));
    Serial.println(F("=================================================="));
    Serial.print(F("Layout Configured : ")); Serial.println(LAYOUT_NAME);
    Serial.print(F("Grid Resolution   : ")); Serial.print(DISPLAY_WIDTH); 
    Serial.print(F(" x ")); Serial.println(DISPLAY_HEIGHT);
    Serial.print(F("Total LED Count   : ")); Serial.println(TOTAL_LEDS);
    Serial.print(F("Data Output Pin   : GPIO ")); Serial.println(PIN_LED_DATA);
    Serial.print(F("Button 1 (Next)   : GPIO ")); Serial.println(PIN_BTN_NEXT);
    Serial.print(F("Button 2 (Pause)  : GPIO ")); Serial.println(PIN_BTN_PAUSE);
    Serial.print(F("Power Safety Cap  : ")); Serial.print(POWER_LIMIT_MILLIAMPS);
    Serial.print(F(" mA @ ")); Serial.print(POWER_LIMIT_VOLTS); Serial.println(F(" V"));
    Serial.println(F("--------------------------------------------------"));
    Serial.println(F("Serial Commands:"));
    Serial.println(F("  'n' - Next animation"));
    Serial.println(F("  'p' - Pause / Resume"));
    Serial.println(F("  '+' - Increase brightness"));
    Serial.println(F("  '-' - Decrease brightness"));
    Serial.println(F("  '0'..'9' - Jump directly to mode index"));
    Serial.println(F("=================================================="));
    Serial.println();
}

// -----------------------------------------------------------------------------
// Helper: Process Incoming Serial Commands
// -----------------------------------------------------------------------------
void handleSerialCommands() {
    while (Serial.available() > 0) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'n':
            case 'N':
                engine.nextMode();
                break;
            case 'p':
            case 'P':
                engine.togglePause();
                break;
            case '+':
                currentBrightness = qadd8(currentBrightness, 16);
                if (currentBrightness > MAX_BRIGHTNESS) currentBrightness = MAX_BRIGHTNESS;
                FastLED.setBrightness(currentBrightness);
                Serial.print(F("Brightness: ")); Serial.println(currentBrightness);
                break;
            case '-':
                currentBrightness = qsub8(currentBrightness, 16);
                if (currentBrightness < 16) currentBrightness = 16;
                FastLED.setBrightness(currentBrightness);
                Serial.print(F("Brightness: ")); Serial.println(currentBrightness);
                break;
            default:
                if (cmd >= '0' && cmd <= '9') {
                    int targetMode = cmd - '0';
                    engine.setMode(targetMode);
                    Serial.print(F("Jumped to mode: ")); Serial.println(engine.getModeName());
                }
                break;
        }
    }
}

// -----------------------------------------------------------------------------
// Arduino Setup
// -----------------------------------------------------------------------------
void setup() {
    // 1. Initialize USB Serial
    Serial.begin(115200);
    delay(500); // Allow CDC connection on ESP32-S3

    // 2. Output boot information
    printSystemBanner();

    // 3. Initialize Buttons
    btnNext.begin();
    btnPause.begin();

    // 4. Initialize FastLED hardware driver
    FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, TOTAL_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(currentBrightness);

    // Apply hardware current limiting for electrical safety
    FastLED.setMaxPowerInVoltsAndMilliamps(POWER_LIMIT_VOLTS, POWER_LIMIT_MILLIAMPS);

    // 5. Initialize Animation Engine
    engine.begin();
    FastLED.show();

    Serial.println(F("System initialized successfully. Starting animation engine."));
}

// -----------------------------------------------------------------------------
// Arduino Main Loop
// -----------------------------------------------------------------------------
void loop() {
    // 1. Update Hardware Buttons
    btnNext.update();
    btnPause.update();

    if (btnNext.wasPressed()) {
        engine.nextMode();
    }

    if (btnPause.wasPressed()) {
        engine.togglePause();
    }

    // 2. Check for Serial console interactions
    handleSerialCommands();

    // 3. Throttle Animation Engine to Target Frame Rate (60 FPS)
    EVERY_N_MILLISECONDS(1000 / TARGET_FRAME_RATE) {
        engine.update();
        FastLED.show();
    }
}
