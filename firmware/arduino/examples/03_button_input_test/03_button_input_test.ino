/**
 * Example 03: Tactile Button Debounce & Event Test
 * 
 * Purpose:
 * Validates hardware wiring and software debouncing of tactile switches
 * connected to GPIO 14 (Next) and GPIO 15 (Pause).
 */

#include <Arduino.h>

#define PIN_BTN1 14
#define PIN_BTN2 15

unsigned long lastDebounce1 = 0;
unsigned long lastDebounce2 = 0;
const unsigned long DEBOUNCE_DELAY = 50;

bool lastReading1 = HIGH;
bool debouncedState1 = HIGH;

bool lastReading2 = HIGH;
bool debouncedState2 = HIGH;

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println(F("=========================================="));
    Serial.println(F("   TinkerSpace Button Hardware Test       "));
    Serial.println(F("=========================================="));
    Serial.println(F("Button 1: GPIO 14 (Next Mode)"));
    Serial.println(F("Button 2: GPIO 15 (Pause / Resume)"));
    Serial.println(F("Press either button to observe debounced triggers."));
    Serial.println(F("------------------------------------------"));

    pinMode(PIN_BTN1, INPUT_PULLUP);
    pinMode(PIN_BTN2, INPUT_PULLUP);
}

void loop() {
    // Check Button 1
    bool r1 = digitalRead(PIN_BTN1);
    if (r1 != lastReading1) lastDebounce1 = millis();
    if ((millis() - lastDebounce1) > DEBOUNCE_DELAY) {
        if (r1 != debouncedState1) {
            debouncedState1 = r1;
            if (debouncedState1 == LOW) {
                Serial.println(F("[EVENT] Button 1 (GPIO 14) PRESSED -> Trigger Next Mode"));
            } else {
                Serial.println(F("[INFO]  Button 1 (GPIO 14) RELEASED"));
            }
        }
    }
    lastReading1 = r1;

    // Check Button 2
    bool r2 = digitalRead(PIN_BTN2);
    if (r2 != lastReading2) lastDebounce2 = millis();
    if ((millis() - lastDebounce2) > DEBOUNCE_DELAY) {
        if (r2 != debouncedState2) {
            debouncedState2 = r2;
            if (debouncedState2 == LOW) {
                Serial.println(F("[EVENT] Button 2 (GPIO 15) PRESSED -> Trigger Pause / Resume"));
            } else {
                Serial.println(F("[INFO]  Button 2 (GPIO 15) RELEASED"));
            }
        }
    }
    lastReading2 = r2;
}
