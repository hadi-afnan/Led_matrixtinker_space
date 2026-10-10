#ifndef TINKERSPACE_BUTTONS_H
#define TINKERSPACE_BUTTONS_H

#include <Arduino.h>

/**
 * @brief Active-low tactile switch handler with non-blocking software debounce.
 */
class Button {
public:
    Button(uint8_t pin, unsigned long debounceMs = 50);

    void begin();
    void update();
    bool wasPressed();
    bool wasLongPressed(unsigned long thresholdMs = 1500);
    bool isPressed() const;
    unsigned long getPressedDuration() const;

private:
    uint8_t _pin;
    unsigned long _debounceMs;
    unsigned long _lastDebounceTime;
    unsigned long _pressStartTime;
    bool _lastPhysicalState;
    bool _debouncedState;
    bool _pressedEvent;
    bool _longPressTriggered;
    bool _longPressEvent;
};

#endif // TINKERSPACE_BUTTONS_H
