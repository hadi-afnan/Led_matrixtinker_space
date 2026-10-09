/**
 * ============================================================================
 * TinkerSpace Modular LED Display — Corner & Serpentine Raster Test
 * ============================================================================
 * Purpose:
 * Verifies physical LED raster orientation and coordinate mapping before
 * running the full controller.
 * 
 * Logical Coordinate Definition:
 *   Origin (0,0) : Bottom-Left corner
 *   X-axis       : 0 to 49 (Left to Right)
 *   Y-axis       : 0 to 29 (Bottom to Top)
 * 
 * Physical Serpentine Raster Assumption:
 *   - LED 0 is at Bottom-Left (0, 0)
 *   - Row 0 (bottom) runs Left-to-Right  (0 -> 49)
 *   - Row 1 (next)   runs Right-to-Left (50 -> 99)
 *   - Row 2 (next)   runs Left-to-Right (100 -> 149)
 *   - Alternating upwards to Row 29
 * 
 * Visual Verification:
 *   - Bottom-Left  (0, 0)   = RED   (LED 0)
 *   - Bottom-Right (49, 0)  = GREEN (LED 49)
 *   - Top-Left     (0, 29)  = BLUE  (LED 1499)
 *   - Top-Right    (49, 29) = WHITE (LED 1450)
 *   - Animated Crawl:
 *     Row 0 crawls Left -> Right in YELLOW
 *     Row 1 crawls Right -> Left in CYAN
 * ============================================================================
 */

#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN          4
#define DISPLAY_WIDTH    50
#define DISPLAY_HEIGHT   30
#define TOTAL_LEDS       (DISPLAY_WIDTH * DISPLAY_HEIGHT) // 1,500 LEDs

#define LED_TYPE         WS2812B
#define COLOR_ORDER      GRB
#define SAFE_BRIGHTNESS  50      // Low brightness for initial bench test

CRGB leds[TOTAL_LEDS];

// Mapping Modes:
// 1 = Global Row-Serpentine (bottom row L->R, row 1 R->L, alternating upwards)
// 2 = Modular 10x10 Serpentine (within 5x3 module tiles)
int mappingMode = 1;

/**
 * @brief Translates logical (x, y) with Bottom-Left origin (0, 0) to 1D index
 */
int getPixelIndex(int x, int y) {
    if (x < 0 || x >= DISPLAY_WIDTH || y < 0 || y >= DISPLAY_HEIGHT) {
        return -1;
    }

    if (mappingMode == 1) {
        // Mode 1: Global Row Serpentine (matches user diagram)
        if ((y & 1) == 0) {
            // Even rows (0, 2, 4...) run Left -> Right
            return (y * DISPLAY_WIDTH) + x;
        } else {
            // Odd rows (1, 3, 5...) run Right -> Left
            return (y * DISPLAY_WIDTH) + (DISPLAY_WIDTH - 1 - x);
        }
    } else {
        // Mode 2: Modular 10x10 Tiles
        int mx = x / 10;
        int my = y / 10;
        int lx = x % 10;
        int ly = y % 10;

        int modIdx;
        if ((my & 1) == 0) {
            modIdx = (my * 5) + mx;
        } else {
            modIdx = (my * 5) + (4 - mx);
        }

        int pixInMod;
        if ((ly & 1) == 0) {
            pixInMod = (ly * 10) + lx;
        } else {
            pixInMod = (ly * 10) + (9 - lx);
        }

        return (modIdx * 100) + pixInMod;
    }
}

void setPixel(int x, int y, CRGB color) {
    int idx = getPixelIndex(x, y);
    if (idx >= 0 && idx < TOTAL_LEDS) {
        leds[idx] = color;
    }
}

// Draw a 2x2 cluster at a corner so it is prominent and unmistakable
void drawCornerBlock(int x, int y, CRGB color) {
    int dx = (x == 0) ? 1 : -1;
    int dy = (y == 0) ? 1 : -1;
    setPixel(x, y, color);
    setPixel(x + dx, y, color);
    setPixel(x, y + dy, color);
    setPixel(x + dx, y + dy, color);
}

void printStatus() {
    Serial.println();
    Serial.println(F("=================================================="));
    Serial.println(F("   TINKERSPACE MATRIX CORNER & RASTER VERIFIER    "));
    Serial.println(F("=================================================="));
    Serial.print(F("Active Mapping: Mode ")); Serial.print(mappingMode);
    Serial.println(mappingMode == 1 ? F(" (Global Row-Serpentine)") : F(" (Modular 10x10)"));
    Serial.println(F("Corner Markers:"));
    Serial.print(F("  [BOTTOM-LEFT]  (0, 0)   -> RED   (LED Index ")); 
    Serial.print(getPixelIndex(0, 0)); Serial.println(F(")"));
    Serial.print(F("  [BOTTOM-RIGHT] (49, 0)  -> GREEN (LED Index ")); 
    Serial.print(getPixelIndex(49, 0)); Serial.println(F(")"));
    Serial.print(F("  [TOP-LEFT]     (0, 29)  -> BLUE  (LED Index ")); 
    Serial.print(getPixelIndex(0, 29)); Serial.println(F(")"));
    Serial.print(F("  [TOP-RIGHT]    (49, 29) -> WHITE (LED Index ")); 
    Serial.print(getPixelIndex(49, 29)); Serial.println(F(")"));
    Serial.println(F("--------------------------------------------------"));
    Serial.println(F("Commands: '1'=Global Serpentine, '2'=Modular 10x10, '+'/'-'=Brightness"));
    Serial.println(F("=================================================="));
}

void setup() {
    Serial.begin(115200);
    delay(400);

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, TOTAL_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(SAFE_BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 5000); // 5A safety cap for bench testing

    FastLED.clear();
    FastLED.show();

    printStatus();
}

int crawlX = 0;
int crawlRow = 0;
unsigned long lastCrawl = 0;
unsigned long lastBannerPrint = 0;

void loop() {
    // 1. Process Serial commands
    if (Serial.available()) {
        char c = Serial.read();
        if (c == '1') {
            mappingMode = 1;
            Serial.println(F("Switched to Mode 1: Global Row-Serpentine"));
            printStatus();
        } else if (c == '2') {
            mappingMode = 2;
            Serial.println(F("Switched to Mode 2: Modular 10x10 Serpentine"));
            printStatus();
        } else if (c == '+') {
            FastLED.setBrightness(min(255, FastLED.getBrightness() + 16));
            Serial.print(F("Brightness: ")); Serial.println(FastLED.getBrightness());
        } else if (c == '-') {
            FastLED.setBrightness(max(10, FastLED.getBrightness() - 16));
            Serial.print(F("Brightness: ")); Serial.println(FastLED.getBrightness());
        }
    }

    // 2. Animate crawling dot along rows 0 and 1
    if (millis() - lastCrawl > 35) {
        lastCrawl = millis();

        FastLED.clear();

        // Always illuminate the 4 corners:
        drawCornerBlock(0, 0, CRGB::Red);                     // Bottom-Left
        drawCornerBlock(DISPLAY_WIDTH - 1, 0, CRGB::Green);   // Bottom-Right
        drawCornerBlock(0, DISPLAY_HEIGHT - 1, CRGB::Blue);   // Top-Left
        drawCornerBlock(DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1, CRGB::White); // Top-Right

        // Row 0 crawl: Left -> Right (Yellow)
        // Row 1 crawl: Right -> Left (Cyan)
        if (crawlRow == 0) {
            setPixel(crawlX, 0, CRGB::Yellow);
            crawlX++;
            if (crawlX >= DISPLAY_WIDTH) {
                crawlX = DISPLAY_WIDTH - 1;
                crawlRow = 1;
            }
        } else if (crawlRow == 1) {
            setPixel(crawlX, 1, CRGB::Cyan);
            crawlX--;
            if (crawlX < 0) {
                crawlX = 0;
                crawlRow = 0;
            }
        }

        FastLED.show();
    }

    // Periodic reminder on serial
    if (millis() - lastBannerPrint > 8000) {
        lastBannerPrint = millis();
        Serial.println(F("[CORNER TEST ACTIVE] Check: Bottom-Left=RED, Bottom-Right=GREEN, Top-Left=BLUE, Top-Right=WHITE"));
    }
}
