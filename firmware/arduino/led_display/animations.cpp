#include "animations.h"

static const char* MODE_NAMES[NUM_DISPLAY_MODES] = {
    "Rainbow Wave",
    "2D Fire Simulation",
    "2D Plasma",
    "Color Waves",
    "Confetti Sparkles",
    "Meteor Rain",
    "Breathing Color Pulse",
    "Gradient Sweep",
    "Scrolling Text Banner",
    "Checkerboard Pattern",
    "Row Scan Test",
    "Solid Red Test",
    "Solid Green Test",
    "Solid Blue Test",
    "Solid White (50% Power Test)",
    "Rainbow Ripple",
    "Starfield Twinkle",
    "Sinelon Sweep",
    "Flowing Noise",
    "Module Bring-Up Sweep",
    "10x10 Module Grid Test",
    "Serpentine Data Flow Walk"
};

AnimationEngine::AnimationEngine(DisplayMapping& display, CRGB* leds)
    : _display(display),
      _leds(leds),
      _currentMode(MODE_RAINBOW_WAVE),
      _isPaused(false),
      _lastUpdate(0),
      _hue(0),
      _stepCounter(0),
      _scrollOffset(DISPLAY_WIDTH) {
    memset(_heatMap, 0, sizeof(_heatMap));
}

void AnimationEngine::begin() {
    _display.clear();
    _lastUpdate = millis();
}

const char* AnimationEngine::getModeName() const {
    if (_currentMode >= 0 && _currentMode < NUM_DISPLAY_MODES) {
        return MODE_NAMES[_currentMode];
    }
    return "Unknown Mode";
}

void AnimationEngine::nextMode() {
    _currentMode = (_currentMode + 1) % NUM_DISPLAY_MODES;
    _stepCounter = 0;
    _scrollOffset = DISPLAY_WIDTH;
    _display.clear();
    memset(_heatMap, 0, sizeof(_heatMap));
    Serial.print(F("Switched to Mode: "));
    Serial.println(getModeName());
}

void AnimationEngine::prevMode() {
    _currentMode = (_currentMode - 1 + NUM_DISPLAY_MODES) % NUM_DISPLAY_MODES;
    _stepCounter = 0;
    _scrollOffset = DISPLAY_WIDTH;
    _display.clear();
    memset(_heatMap, 0, sizeof(_heatMap));
    Serial.print(F("Switched to Mode: "));
    Serial.println(getModeName());
}

void AnimationEngine::setMode(int mode) {
    if (mode >= 0 && mode < NUM_DISPLAY_MODES) {
        _currentMode = mode;
        _stepCounter = 0;
        _scrollOffset = DISPLAY_WIDTH;
        _display.clear();
        memset(_heatMap, 0, sizeof(_heatMap));
    }
}

void AnimationEngine::togglePause() {
    _isPaused = !_isPaused;
    Serial.println(_isPaused ? F("Animation PAUSED") : F("Animation RESUMED"));
}

void AnimationEngine::update() {
    if (_isPaused) {
        return; // Don't advance animation frames if paused
    }

    _hue++;
    _stepCounter++;

    switch (_currentMode) {
        case MODE_RAINBOW_WAVE:     renderRainbowWave(); break;
        case MODE_FIRE:             renderFire(); break;
        case MODE_PLASMA_2D:        renderPlasma2D(); break;
        case MODE_COLOR_WAVES:      renderColorWaves(); break;
        case MODE_CONFETTI:         renderConfetti(); break;
        case MODE_METEOR:           renderMeteor(); break;
        case MODE_BREATHING_PULSE:  renderBreathingPulse(); break;
        case MODE_GRADIENT_SWEEP:   renderGradientSweep(); break;
        case MODE_SCROLLING_TEXT:   renderScrollingText(); break;
        case MODE_CHECKERBOARD:     renderCheckerboard(); break;
        case MODE_ROW_SCAN:         renderRowScan(); break;
        case MODE_SOLID_RED:        renderSolidColor(CRGB::Red); break;
        case MODE_SOLID_GREEN:      renderSolidColor(CRGB::Green); break;
        case MODE_SOLID_BLUE:       renderSolidColor(CRGB::Blue); break;
        case MODE_SOLID_WHITE:      renderSolidColor(CRGB(128, 128, 128)); break;
        case MODE_RIPPLE:           renderRipple(); break;
        case MODE_TWINKLE:          renderTwinkle(); break;
        case MODE_SINELON:          renderSinelon(); break;
        case MODE_NOISE_FLOW:       renderNoiseFlow(); break;
        case MODE_MODULE_SWEEP:     renderModuleSweep(); break;
        case MODE_MODULE_GRID:      renderModuleGrid(); break;
        case MODE_SERPENTINE_FLOW:  renderSerpentineFlow(); break;
        default: break;
    }
}

// -----------------------------------------------------------------------------
// Effect: Rainbow Wave
// -----------------------------------------------------------------------------
void AnimationEngine::renderRainbowWave() {
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            uint8_t pixelHue = _hue + (x * 4) + (y * 6);
            _display.setPixel(x, y, CHSV(pixelHue, 240, 255));
        }
    }
}

// -----------------------------------------------------------------------------
// Effect: 2D Fire Simulation
// -----------------------------------------------------------------------------
void AnimationEngine::renderFire() {
    // Step 1: Cool down every cell
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            uint8_t cooldown = random8(0, 18);
            if (cooldown > _heatMap[x][y]) {
                _heatMap[x][y] = 0;
            } else {
                _heatMap[x][y] -= cooldown;
            }
        }
    }

    // Step 2: Heat drifts upward from bottom (y=0) towards top (y=DISPLAY_HEIGHT-1)
    for (int y = DISPLAY_HEIGHT - 1; y > 0; y--) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int left = (x > 0) ? x - 1 : 0;
            int right = (x < DISPLAY_WIDTH - 1) ? x + 1 : DISPLAY_WIDTH - 1;
            _heatMap[x][y] = (_heatMap[left][y - 1] + _heatMap[x][y - 1] + _heatMap[right][y - 1]) / 3;
        }
    }

    // Step 3: Randomly ignite new sparks at bottom row (y = 0)
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        if (random8() < 140) {
            _heatMap[x][0] = qadd8(_heatMap[x][0], random8(160, 255));
        }
    }

    // Step 4: Map heat values to FastLED HeatColor palette
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            _display.setPixel(x, y, HeatColor(_heatMap[x][y]));
        }
    }
}

// -----------------------------------------------------------------------------
// Effect: 2D Trigonometric Plasma
// -----------------------------------------------------------------------------
void AnimationEngine::renderPlasma2D() {
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            uint8_t v1 = sin8((x * 8) + (_stepCounter * 2));
            uint8_t v2 = sin8((y * 8) + (_stepCounter * 3));
            uint8_t v3 = sin8((x * 4 + y * 4) + _hue);
            uint8_t colorIndex = (v1 + v2 + v3) / 3;
            _display.setPixel(x, y, ColorFromPalette(RainbowColors_p, colorIndex + _hue, 255, LINEARBLEND));
        }
    }
}

// -----------------------------------------------------------------------------
// Effect: Color Waves
// -----------------------------------------------------------------------------
void AnimationEngine::renderColorWaves() {
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            int distCenter = abs(x - (DISPLAY_WIDTH / 2)) + abs(y - (DISPLAY_HEIGHT / 2));
            uint8_t wave = sin8(distCenter * 14 - (_stepCounter * 4));
            _display.setPixel(x, y, CHSV(_hue + wave / 2, 220, wave));
        }
    }
}

// -----------------------------------------------------------------------------
// Effect: Confetti
// -----------------------------------------------------------------------------
void AnimationEngine::renderConfetti() {
    // Fade existing pixels to black
    fadeToBlackBy(_leds, TOTAL_LEDS, 16);

    // Add 4 new random sparkle pixels per frame
    for (int i = 0; i < 4; i++) {
        int rx = random(DISPLAY_WIDTH);
        int ry = random(DISPLAY_HEIGHT);
        _display.setPixel(rx, ry, CHSV(_hue + random8(64), 200, 255));
    }
}

// -----------------------------------------------------------------------------
// Effect: Meteor Rain
// -----------------------------------------------------------------------------
void AnimationEngine::renderMeteor() {
    // Fade trail
    for (int i = 0; i < TOTAL_LEDS; i++) {
        if (random8() < 40) {
            _leds[i].fadeToBlackBy(48);
        }
    }

    // Draw active meteors falling from top (sky) to bottom (ground)
    int meteorY = (DISPLAY_HEIGHT + 10) - (_stepCounter % (DISPLAY_HEIGHT + 10));
    int meteorX = (_stepCounter / 2) % DISPLAY_WIDTH;

    for (int k = 0; k < 3; k++) {
        int xPos = (meteorX + (k * 15)) % DISPLAY_WIDTH;
        int yPos = meteorY - (k * 4);
        if (yPos >= 0 && yPos < DISPLAY_HEIGHT) {
            _display.setPixel(xPos, yPos, CRGB::White);
            if (yPos + 1 < DISPLAY_HEIGHT) _display.setPixel(xPos, yPos + 1, CRGB(100, 180, 255));
        }
    }
}

// -----------------------------------------------------------------------------
// Effect: Breathing Color Pulse
// -----------------------------------------------------------------------------
void AnimationEngine::renderBreathingPulse() {
    uint8_t brightness = beatsin8(20, 30, 255); // 20 BPM pulse
    CRGB color = CHSV(_hue, 240, brightness);
    _display.fill(color);
}

// -----------------------------------------------------------------------------
// Effect: Gradient Sweep
// -----------------------------------------------------------------------------
void AnimationEngine::renderGradientSweep() {
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        CRGB col = blend(CRGB::DeepPink, CRGB::Cyan, sin8((x * 6) + _stepCounter));
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            _display.setPixel(x, y, col);
        }
    }
}

// -----------------------------------------------------------------------------
// Effect: Scrolling Text Banner
// -----------------------------------------------------------------------------
void AnimationEngine::renderScrollingText() {
    _display.clear();
    const char* message = "TINKERSPACE MODULAR LED DISPLAY * ESP32-S3 * OPEN SOURCE";
    int messageWidth = strlen(message) * 6;

    _display.drawString(_scrollOffset, (DISPLAY_HEIGHT / 2) - 4, message, CHSV(_hue, 220, 255));

    _scrollOffset--;
    if (_scrollOffset < -messageWidth) {
        _scrollOffset = DISPLAY_WIDTH;
    }
}

// -----------------------------------------------------------------------------
// Test Pattern: Checkerboard
// -----------------------------------------------------------------------------
void AnimationEngine::renderCheckerboard() {
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            bool isEven = ((x / 2) + (y / 2)) % 2 == 0;
            _display.setPixel(x, y, isEven ? CRGB::White : CRGB::Black);
        }
    }
}

// -----------------------------------------------------------------------------
// Test Pattern: Row Scan
// -----------------------------------------------------------------------------
void AnimationEngine::renderRowScan() {
    _display.clear();
    int activeRow = (_stepCounter / 2) % DISPLAY_HEIGHT;
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        _display.setPixel(x, activeRow, CRGB::Green);
    }
}

// -----------------------------------------------------------------------------
// Test Pattern: Solid Color Fill
// -----------------------------------------------------------------------------
void AnimationEngine::renderSolidColor(CRGB color) {
    _display.fill(color);
}

void AnimationEngine::renderRipple() {
    const int cx = DISPLAY_WIDTH / 2;
    const int cy = DISPLAY_HEIGHT / 2;
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            uint8_t wave = sin8((abs(x - cx) + abs(y - cy)) * 18 - _stepCounter * 6);
            _display.setPixel(x, y, CHSV(_hue + wave / 3, 230, wave));
        }
    }
}

void AnimationEngine::renderTwinkle() {
    fadeToBlackBy(_leds, TOTAL_LEDS, 22);
    for (uint8_t i = 0; i < 5; i++) {
        _display.setPixel(random(DISPLAY_WIDTH), random(DISPLAY_HEIGHT),
                          CHSV(_hue + random8(96), 100, 255));
    }
}

void AnimationEngine::renderSinelon() {
    fadeToBlackBy(_leds, TOTAL_LEDS, 24);
    int x = beatsin8(13, 0, DISPLAY_WIDTH - 1);
    int y = beatsin8(17, 0, DISPLAY_HEIGHT - 1);
    _display.setPixel(x, y, CHSV(_hue, 255, 255));
    _display.setPixel(DISPLAY_WIDTH - 1 - x, y, CHSV(_hue + 96, 255, 220));
}

void AnimationEngine::renderNoiseFlow() {
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            uint8_t noise = inoise8(x * 26, y * 26, _stepCounter * 4);
            _display.setPixel(x, y, ColorFromPalette(PartyColors_p, noise + _hue, noise, LINEARBLEND));
        }
    }
}

// -----------------------------------------------------------------------------
// Diagnostic 1: Module-by-Module Sweep (Tests all 15 modules one-by-one)
// -----------------------------------------------------------------------------
void AnimationEngine::renderModuleSweep() {
    fadeToBlackBy(_leds, TOTAL_LEDS, 50);
    int activeMod = (_stepCounter / 15) % (MATRIX_MODULES_X * MATRIX_MODULES_Y);
    _display.drawModuleHighlight(activeMod, CHSV(activeMod * 17, 240, 255), true, true);
}

// -----------------------------------------------------------------------------
// Diagnostic 2: 10x10 Module Grid Alignment (Verifies all 15 module seams)
// -----------------------------------------------------------------------------
void AnimationEngine::renderModuleGrid() {
    _display.drawModuleGrid(CHSV(_hue, 220, 200), CRGB::White);
}

// -----------------------------------------------------------------------------
// Diagnostic 3: Physical Serpentine Flow Walk (Visualizes exact data path)
// -----------------------------------------------------------------------------
void AnimationEngine::renderSerpentineFlow() {
    fadeToBlackBy(_leds, TOTAL_LEDS, 25);
    int head = (_stepCounter * 3) % TOTAL_LEDS;
    for (int i = 0; i < 4; i++) {
        int idx = (head + i) % TOTAL_LEDS;
        _leds[idx] = (i == 3) ? CRGB::White : CHSV(_hue + (i * 20), 220, 240);
    }
}
