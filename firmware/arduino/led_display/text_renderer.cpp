#include "text_renderer.h"

TextRenderer::TextRenderer(DisplayMapping& display)
    : _display(display),
      _active(false),
      _line1("TINKERSPACE"),
      _line2("IS OPEN"),
      _line3("WELCOME!"),
      _color1(CRGB(56, 189, 248)),
      _color2(CRGB(52, 211, 153)),
      _color3(CRGB(250, 204, 21)),
      _baseColor1(CRGB(56, 189, 248)),
      _baseColor2(CRGB(52, 211, 153)),
      _baseColor3(CRGB(250, 204, 21)),
      _layout(LAYOUT_3LINE),
      _effect(EFFECT_STATIC),
      _align("center"),
      _badgeIcon("check"),
      _speed(40),
      _scrollOffset(DISPLAY_WIDTH),
      _lastUpdate(0),
      _lastBlinkTime(0),
      _blinkState(true),
      _pulsePhase(0),
      _rainbowHue(0),
      _dirty(true) {}

void TextRenderer::begin() {
    _scrollOffset = DISPLAY_WIDTH;
    _lastUpdate = millis();
    _dirty = true;
}

void TextRenderer::setText(const String& l1, const String& l2, const String& l3) {
    auto sanitize = [](const String& input) {
        String output;
        for (size_t i = 0; i < input.length() && output.length() < 48; i++) {
            char c = input[i];
            output += (c >= 32 && c <= 126) ? c : ' ';
        }
        return output;
    };
    _line1 = sanitize(l1);
    _line2 = sanitize(l2);
    _line3 = sanitize(l3);
    _scrollOffset = DISPLAY_WIDTH;
    _active = true;
    _dirty = true;
}

void TextRenderer::setColors(CRGB c1, CRGB c2, CRGB c3) {
    _baseColor1 = c1;
    _baseColor2 = c2;
    _baseColor3 = c3;
    _color1 = c1;
    _color2 = c2;
    _color3 = c3;
    _dirty = true;
}

void TextRenderer::setLayout(TextLayoutStyle layout) {
    _layout = layout;
    _scrollOffset = DISPLAY_WIDTH;
    _dirty = true;
}

void TextRenderer::setEffect(TextEffectStyle effect) {
    _effect = effect;
    _dirty = true;
}

void TextRenderer::setAlignment(const String& align) {
    _align = align;
    _dirty = true;
}

void TextRenderer::setBadgeIcon(const String& iconName) {
    _badgeIcon = iconName;
    _dirty = true;
}

void TextRenderer::setSpeed(int speed) {
    _speed = constrain(speed, 10, 100);
    _dirty = true;
}

void TextRenderer::drawLine(int y, const String& text, CRGB color, int scale, int xOffset) {
    if (text.length() == 0) return;

    int charWidth = 6 * scale;
    int totalWidth = (text.length() * charWidth) - scale;
    int startX = 1;

    if (_align == "center") {
        startX = (DISPLAY_WIDTH - totalWidth) / 2;
    } else if (_align == "right") {
        startX = DISPLAY_WIDTH - totalWidth - 1;
    }
    startX += xOffset;

    int cursorX = startX;
    for (size_t i = 0; i < text.length(); i++) {
        char c = text[i];
        _display.drawChar(cursorX, y, c, color, scale);
        cursorX += charWidth;
    }
}

void TextRenderer::drawBadgeIcon(const String& icon, int x, int y) {
    // 8x8 Mini Icon Bitmaps for Space Badge mode
    static const uint8_t ICON_CHECK[8] PROGMEM   = {0x01, 0x03, 0x06, 0x0C, 0x1B, 0x38, 0x10, 0x00};
    static const uint8_t ICON_CROSS[8] PROGMEM   = {0xC3, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0xC3, 0x00};
    static const uint8_t ICON_GEAR[8] PROGMEM    = {0x3C, 0x66, 0xC3, 0x99, 0x99, 0xC3, 0x66, 0x3C};
    static const uint8_t ICON_WARNING[8] PROGMEM = {0x18, 0x3C, 0x24, 0x66, 0x42, 0xDB, 0xFF, 0x00};
    static const uint8_t ICON_ROCKET[8] PROGMEM  = {0x18, 0x3C, 0x24, 0x66, 0x7E, 0xFF, 0xA5, 0x3C};
    static const uint8_t ICON_COFFEE[8] PROGMEM  = {0x7C, 0x46, 0x45, 0x46, 0x38, 0x00, 0xFE, 0x00};

    const uint8_t* p = ICON_CHECK;
    CRGB iconColor = CRGB(52, 211, 153); // Green

    if (icon == "cross") { p = ICON_CROSS; iconColor = CRGB(244, 63, 94); }
    else if (icon == "gear") { p = ICON_GEAR; iconColor = CRGB(250, 204, 21); }
    else if (icon == "warning") { p = ICON_WARNING; iconColor = CRGB(245, 158, 11); }
    else if (icon == "rocket") { p = ICON_ROCKET; iconColor = CRGB(168, 85, 247); }
    else if (icon == "coffee") { p = ICON_COFFEE; iconColor = CRGB(245, 158, 11); }

    for (int r = 0; r < 8; r++) {
        uint8_t rowByte = pgm_read_byte(p + r);
        for (int c = 0; c < 8; c++) {
            if (rowByte & (1 << (7 - c))) {
                _display.setPixel(x + c, y + (7 - r), iconColor);
            }
        }
    }
}

void TextRenderer::render3Line() {
    // In bottom-left coordinate system:
    // Line 1 (Top header)    at y = 21 (rows 21..27)
    // Line 2 (Middle status) at y = 11 (rows 11..17)
    // Line 3 (Bottom callout)at y = 2  (rows 2..8)
    drawLine(21, _line1, _color1, 1, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
    drawLine(11, _line2, _color2, 1, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
    drawLine(2,  _line3, _color3, 1, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
}

void TextRenderer::renderBigHeader() {
    // Big 2x Header on top (y = 14 occupies rows 14..27)
    // Subtitle on bottom (y = 3 occupies rows 3..9)
    drawLine(14, _line1, _color1, 2, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
    drawLine(3,  _line2, _color2, 1, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
}

void TextRenderer::renderBadge() {
    // Left badge icon centered at y = 11 (rows 11..18)
    // Line 1 on top right (y = 17)
    // Line 2 on bottom right (y = 6)
    drawBadgeIcon(_badgeIcon, 2, 11);
    drawLine(17, _line1, _color1, 1, 14);
    drawLine(6,  _line2, _color2, 1, 14);
}

void TextRenderer::renderMarquee() {
    String fullMsg = _line1;
    if (_line2.length() > 0) fullMsg += " * " + _line2;
    if (_line3.length() > 0) fullMsg += " * " + _line3;
    drawLine(11, fullMsg, _color1, 1, _scrollOffset);
}

bool TextRenderer::update() {
    if (!_active) return false;

    bool frameChanged = _dirty;

    // Effect Timing Handlers
    if (_effect == EFFECT_STATIC) {
        _color1 = _baseColor1;
        _color2 = _baseColor2;
        _color3 = _baseColor3;
    } else if (_effect == EFFECT_BLINK) {
        if (millis() - _lastBlinkTime > 500) {
            _lastBlinkTime = millis();
            _blinkState = !_blinkState;
            frameChanged = true;
        }
        if (!_blinkState) {
            if (frameChanged) _display.clear();
            _dirty = false;
            return frameChanged;
        }
        _color1 = _baseColor1;
        _color2 = _baseColor2;
        _color3 = _baseColor3;
    } else if (_effect == EFFECT_PULSE) {
        _pulsePhase += 4;
        uint8_t b = beatsin8(30, 30, 255);
        _color1 = _baseColor1; _color1.nscale8(b);
        _color2 = _baseColor2; _color2.nscale8(b);
        _color3 = _baseColor3; _color3.nscale8(b);
        frameChanged = true;
    } else if (_effect == EFFECT_RAINBOW) {
        _rainbowHue += 2;
        _color1 = CHSV(_rainbowHue, 220, 255);
        _color2 = CHSV(_rainbowHue + 64, 220, 255);
        _color3 = CHSV(_rainbowHue + 128, 220, 255);
        frameChanged = true;
    }

    bool shouldScroll = (_effect == EFFECT_SCROLL) || (_layout == LAYOUT_MARQUEE);
    if (shouldScroll) {
        unsigned long interval = 110 - _speed; // 10ms to 100ms
        if (millis() - _lastUpdate > interval) {
            _lastUpdate = millis();
            _scrollOffset--;
            frameChanged = true;
            int totalTextLen = 0;
            if (_layout == LAYOUT_MARQUEE) {
                totalTextLen = _line1.length();
                if (_line2.length() > 0) totalTextLen += 3 + _line2.length();
                if (_line3.length() > 0) totalTextLen += 3 + _line3.length();
            } else {
                totalTextLen = max(_line1.length(), max(_line2.length(), _line3.length()));
            }
            if (_scrollOffset < -(int)(totalTextLen * 6 + 10)) {
                _scrollOffset = DISPLAY_WIDTH;
            }
        }
    }

    if (!frameChanged) return false;

    _display.clear();

    switch (_layout) {
        case LAYOUT_3LINE:     render3Line(); break;
        case LAYOUT_BIGHEADER: renderBigHeader(); break;
        case LAYOUT_BADGE:     renderBadge(); break;
        case LAYOUT_MARQUEE:   renderMarquee(); break;
    }
    _dirty = false;
    return true;
}
