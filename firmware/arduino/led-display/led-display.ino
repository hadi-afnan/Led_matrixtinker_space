/**
 * ============================================================================
 * TinkerSpace Modular LED Display — Main Controller Firmware & Web Server
 * ============================================================================
 * Controller : ESP32-S3 (Xtensa Dual-Core LX7 @ 240 MHz)
 * Display    : 10x10 WS2812B Addressable RGB LED Modules (50x30 = 1,500 LEDs)
 * Web UI     : Embedded WLED-Style Responsive Web App (Wi-Fi AP & Station)
 * Control    : Web Browser, Physical Tactile Buttons, and USB Serial
 * Features   : Multi-Line Space Signs, Image Studio, 50x30 Pixel Paint, Animations
 * License    : MIT Open Source License
 * ============================================================================
 */

#include <Arduino.h>
#include <FastLED.h>

#include "config.h"
#include "display_mapping.h"
#include "buttons.h"
#include "animations.h"
#include "text_renderer.h"
#include "web_server_handler.h"

// -----------------------------------------------------------------------------
// Global Instances & Buffers
// -----------------------------------------------------------------------------
CRGB leds[TOTAL_LEDS];
DisplayMapping display(leds);
AnimationEngine engine(display, leds);
TextRenderer textRenderer(display);
WebServerHandler webHandler(display, engine, textRenderer, leds);

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
    Serial.println(F("Web App URL (Access Point) : http://192.168.4.1"));
    Serial.println(F("Web App URL (Local Network): http://tinkerspace.local"));
    Serial.println(F("--------------------------------------------------"));
    Serial.println(F("Serial Commands: 'n'=next, 'p'=pause, '+'=brighter, '-'=dimmer"));
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
                webHandler.setActiveSource(SOURCE_ANIMATION);
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
                    webHandler.setActiveSource(SOURCE_ANIMATION);
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
    delay(400); // Allow CDC connection on ESP32-S3

    // 2. Initialize Hardware Buttons
    btnNext.begin();
    btnPause.begin();

    // 3. Initialize FastLED Hardware Driver
    FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, TOTAL_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(currentBrightness);

    // Apply hardware current limiting for electrical safety
    FastLED.setMaxPowerInVoltsAndMilliamps(POWER_LIMIT_VOLTS, POWER_LIMIT_MILLIAMPS);

    // 4. Initialize Animation Engine & Text Renderer
    engine.begin();
    textRenderer.begin();

    // 5. Initialize Wi-Fi Access Point, mDNS, and Embedded Web Server
    webHandler.begin();

    // 6. Print startup summary
    printSystemBanner();
    FastLED.show();
}

// -----------------------------------------------------------------------------
// Arduino Main Loop
// -----------------------------------------------------------------------------
void loop() {
    // 1. Service Incoming Web Server HTTP Requests
    webHandler.handleClient();

    // 2. Service Physical Hardware Buttons
    btnNext.update();
    btnPause.update();

    if (btnNext.wasPressed()) {
        webHandler.setActiveSource(SOURCE_ANIMATION);
        engine.nextMode();
    }

    if (btnPause.wasPressed()) {
        engine.togglePause();
    }

    // 3. Check for Serial Console Interactions
    handleSerialCommands();

    // 4. Render Active Source to FastLED
    if (!webHandler.isPowerOn()) {
        // Display is toggled OFF via web app
        FastLED.clear();
        FastLED.show();
        delay(20);
        return;
    }

    EVERY_N_MILLISECONDS(1000 / TARGET_FRAME_RATE) {
        ActiveDisplaySource src = webHandler.getActiveSource();

        if (src == SOURCE_ANIMATION) {
            engine.update();
            FastLED.show();
        } else if (src == SOURCE_TEXT_SIGN) {
            textRenderer.update();
            FastLED.show();
        }
        // SOURCE_IMAGE_BUFFER is pushed directly when uploaded or painted
    }
}
