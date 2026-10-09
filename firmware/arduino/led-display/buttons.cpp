#include "buttons.h"

Button::Button(uint8_t pin, unsigned long debounceMs)
    : _pin(pin),
      _debounceMs(debounceMs),
      _lastDebounceTime(0),
      _lastPhysicalState(HIGH),
      _debouncedState(HIGH),
      _pressedEvent(false) {}

void Button::begin() {
    pinMode(_pin, INPUT_PULLUP);
    _lastPhysicalState = digitalRead(_pin);
    _debouncedState = _lastPhysicalState;
}

void Button::update() {
    bool currentReading = digitalRead(_pin);

    if (currentReading != _lastPhysicalState) {
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > _debounceMs) {
        if (currentReading != _debouncedState) {
            _debouncedState = currentReading;
            // Detect transition to LOW (pressed, active-low)
            if (_debouncedState == LOW) {
                _pressedEvent = true;
            }
        }
    }

    _lastPhysicalState = currentReading;
}

bool Button::wasPressed() {
    if (_pressedEvent) {
        _pressedEvent = false;
        return true;
    }
    return false;
}

bool Button::isPressed() const {
    return (_debouncedState == LOW);
}
