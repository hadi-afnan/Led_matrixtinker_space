#ifndef TINKERSPACE_ANIMATIONS_H
#define TINKERSPACE_ANIMATIONS_H

#include <Arduino.h>
#include <FastLED.h>
#include "config.h"
#include "display_mapping.h"

enum DisplayMode {
    MODE_RAINBOW_WAVE = 0,
    MODE_FIRE,
    MODE_PLASMA_2D,
    MODE_COLOR_WAVES,
    MODE_CONFETTI,
    MODE_METEOR,
    MODE_BREATHING_PULSE,
    MODE_GRADIENT_SWEEP,
    MODE_SCROLLING_TEXT,
    MODE_CHECKERBOARD,
    MODE_ROW_SCAN,
    MODE_SOLID_RED,
    MODE_SOLID_GREEN,
    MODE_SOLID_BLUE,
    MODE_SOLID_WHITE,
    MODE_RIPPLE,
    MODE_TWINKLE,
    MODE_SINELON,
    MODE_NOISE_FLOW,
    NUM_DISPLAY_MODES
};

class AnimationEngine {
public:
    AnimationEngine(DisplayMapping& display, CRGB* leds);

    void begin();
    void update();
    void nextMode();
    void prevMode();
    void setMode(int mode);
    void togglePause();

    bool isPaused() const { return _isPaused; }
    int getModeIndex() const { return _currentMode; }
    const char* getModeName() const;

private:
    DisplayMapping& _display;
    CRGB* _leds;
    int _currentMode;
    bool _isPaused;
    unsigned long _lastUpdate;
    uint8_t _hue;
    uint16_t _stepCounter;
    int _scrollOffset;

    // Buffer for 2D Fire simulation
    uint8_t _heatMap[DISPLAY_WIDTH][DISPLAY_HEIGHT];

    // Individual animation renderers
    void renderRainbowWave();
    void renderFire();
    void renderPlasma2D();
    void renderColorWaves();
    void renderConfetti();
    void renderMeteor();
    void renderBreathingPulse();
    void renderGradientSweep();
    void renderScrollingText();
    void renderCheckerboard();
    void renderRowScan();
    void renderSolidColor(CRGB color);
    void renderRipple();
    void renderTwinkle();
    void renderSinelon();
    void renderNoiseFlow();
};

#endif // TINKERSPACE_ANIMATIONS_H
