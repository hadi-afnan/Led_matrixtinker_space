/**
 * Example 02: Full Matrix Serpentine Continuity Test
 * 
 * Purpose:
 * Validates module chaining, jumper soldering, and serpentine data propagation
 * across the entire assembled display.
 */

#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN          4
#define MODULES_COUNT    15    // Set to 15 for 5x3 or 12 for 4x3
#define LEDS_PER_MODULE  100
#define TOTAL_LEDS       (MODULES_COUNT * LEDS_PER_MODULE)
#define LED_TYPE         WS2812B
#define COLOR_ORDER      GRB
#define BRIGHTNESS       60

CRGB leds[TOTAL_LEDS];

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println(F("--- TinkerSpace Full Matrix Serpentine Test ---"));
    Serial.print(F("Total Modules: ")); Serial.println(MODULES_COUNT);
    Serial.print(F("Total LEDs: ")); Serial.println(TOTAL_LEDS);

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, TOTAL_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 10000); // 10A current limit for safe testing
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
}

void loop() {
    // Test 1: Light up each module in serial sequence one by one
    Serial.println(F("Phase 1: Module-by-Module Sequence Check"));
    for (int m = 0; m < MODULES_COUNT; m++) {
        Serial.print(F("Illuminating Module #")); Serial.println(m);
        FastLED.clear();
        int startIdx = m * LEDS_PER_MODULE;
        for (int i = 0; i < LEDS_PER_MODULE; i++) {
            leds[startIdx + i] = CHSV(m * (255 / MODULES_COUNT), 240, 255);
        }
        FastLED.show();
        delay(600);
    }

    // Test 2: Sequential pixel chase across entire chain
    Serial.println(F("Phase 2: Continuous Pixel Walk across entire chain"));
    FastLED.clear();
    for (int i = 0; i < TOTAL_LEDS; i += 2) {
        leds[i] = CRGB::Cyan;
        if (i % 25 == 0) FastLED.show();
        delay(2);
    }
    FastLED.show();
    delay(1000);

    // Test 3: Gentle whole-screen rainbow breathe
    Serial.println(F("Phase 3: Whole-Screen Rainbow Breathe"));
    for (int h = 0; h < 256; h += 4) {
        fill_solid(leds, TOTAL_LEDS, CHSV(h, 220, 180));
        FastLED.show();
        delay(20);
    }
    FastLED.clear();
    FastLED.show();
    delay(500);
}
