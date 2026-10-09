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
    bool isPressed() const;

private:
    uint8_t _pin;
    unsigned long _debounceMs;
    unsigned long _lastDebounceTime;
    bool _lastPhysicalState;
    bool _debouncedState;
    bool _pressedEvent;
};

#endif // TINKERSPACE_BUTTONS_H
