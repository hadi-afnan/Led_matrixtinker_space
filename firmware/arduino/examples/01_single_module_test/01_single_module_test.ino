/**
 * Example 01: Single 10x10 Module Smoke Test
 * 
 * Purpose:
 * Connect a single 10x10 WS2812B module directly to GPIO 4 to verify:
 * 1. Solder pads (DIN, 5V, GND) are healthy.
 * 2. Color channel ordering (GRB).
 * 3. No dead pixels before assembling into the large frame.
 */

#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN       4
#define NUM_LEDS      100
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
#define BRIGHTNESS    64

CRGB leds[NUM_LEDS];

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println(F("--- TinkerSpace Single Module Test (100 LEDs) ---"));

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
}

void loop() {
    // 1. Red fill
    Serial.println(F("Testing: RED"));
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(1000);

    // 2. Green fill
    Serial.println(F("Testing: GREEN"));
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.show();
    delay(1000);

    // 3. Blue fill
    Serial.println(F("Testing: BLUE"));
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    FastLED.show();
    delay(1000);

    // 4. White fill (checks power draw & uniform color)
    Serial.println(F("Testing: WHITE"));
    fill_solid(leds, NUM_LEDS, CRGB::White);
    FastLED.show();
    delay(1000);

    // 5. Pixel chase (crawl from 0 to 99)
    Serial.println(F("Testing: Single Pixel Walk (0 -> 99)"));
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Yellow;
        FastLED.show();
        delay(20);
        leds[i] = CRGB::Black;
    }
}
