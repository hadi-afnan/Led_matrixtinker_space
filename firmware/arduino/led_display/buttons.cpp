#include "buttons.h"

Button::Button(uint8_t pin, unsigned long debounceMs)
    : _pin(pin),
      _debounceMs(debounceMs),
      _lastDebounceTime(0),
      _pressStartTime(0),
      _lastPhysicalState(HIGH),
      _debouncedState(HIGH),
      _pressedEvent(false),
      _longPressTriggered(false),
      _longPressEvent(false) {}

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
            // Transition to LOW (pressed, active-low)
            if (_debouncedState == LOW) {
                _pressStartTime = millis();
                _longPressTriggered = false;
            } else {
                // Transition to HIGH (released)
                if (!_longPressTriggered) {
                    _pressedEvent = true;
                }
                _pressStartTime = 0;
            }
        }
    }

    // Detect continuous hold (long press)
    if (_debouncedState == LOW && !_longPressTriggered && _pressStartTime > 0) {
        if ((millis() - _pressStartTime) >= 1500) {
            _longPressTriggered = true;
            _longPressEvent = true;
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

bool Button::wasLongPressed(unsigned long thresholdMs) {
    if (_longPressEvent) {
        _longPressEvent = false;
        return true;
    }
    return false;
}

bool Button::isPressed() const {
    return (_debouncedState == LOW);
}

unsigned long Button::getPressedDuration() const {
    if (_debouncedState == LOW && _pressStartTime > 0) {
        return millis() - _pressStartTime;
    }
    return 0;
}
