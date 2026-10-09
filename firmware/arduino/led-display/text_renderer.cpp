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
      _layout(LAYOUT_3LINE),
      _effect(EFFECT_STATIC),
      _align("center"),
      _badgeIcon("check"),
      _speed(40),
      _scrollOffset(DISPLAY_WIDTH),
      _lastUpdate(0),
      _lastBlinkTime(0),
      _blinkState(true),
      _pulsePhase(0) {}

void TextRenderer::begin() {
    _scrollOffset = DISPLAY_WIDTH;
    _lastUpdate = millis();
}

void TextRenderer::setText(const String& l1, const String& l2, const String& l3) {
    _line1 = l1;
    _line2 = l2;
    _line3 = l3;
    _scrollOffset = DISPLAY_WIDTH;
    _active = true;
}

void TextRenderer::setColors(CRGB c1, CRGB c2, CRGB c3) {
    _color1 = c1;
    _color2 = c2;
    _color3 = c3;
}

void TextRenderer::setLayout(TextLayoutStyle layout) {
    _layout = layout;
    _scrollOffset = DISPLAY_WIDTH;
}

void TextRenderer::setEffect(TextEffectStyle effect) {
    _effect = effect;
}

void TextRenderer::setAlignment(const String& align) {
    _align = align;
}

void TextRenderer::setBadgeIcon(const String& iconName) {
    _badgeIcon = iconName;
}

void TextRenderer::setSpeed(uint8_t speed) {
    _speed = constrain(speed, 10, 100);
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
        if (scale == 1) {
            _display.drawChar(cursorX, y, c, color);
        } else if (scale == 2) {
            // 2x Scaled Header Font
            for (int dx = 0; dx < 2; dx++) {
                for (int dy = 0; dy < 2; dy++) {
                    _display.drawChar(cursorX + dx, y + dy, c, color);
                }
            }
        }
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
                _display.setPixel(x + c, y + r, iconColor);
            }
        }
    }
}

void TextRenderer::render3Line() {
    drawLine(2, _line1, _color1, 1, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
    drawLine(11, _line2, _color2, 1, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
    drawLine(20, _line3, _color3, 1, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
}

void TextRenderer::renderBigHeader() {
    drawLine(2, _line1, _color1, 2, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
    drawLine(19, _line2, _color2, 1, (_effect == EFFECT_SCROLL ? _scrollOffset : 0));
}

void TextRenderer::renderBadge() {
    drawBadgeIcon(_badgeIcon, 2, 11);
    drawLine(6, _line1, _color1, 1, 14);
    drawLine(16, _line2, _color2, 1, 14);
}

void TextRenderer::renderMarquee() {
    String fullMsg = _line1;
    if (_line2.length() > 0) fullMsg += " * " + _line2;
    if (_line3.length() > 0) fullMsg += " * " + _line3;
    drawLine(11, fullMsg, _color1, 1, _scrollOffset);
}

void TextRenderer::update() {
    if (!_active) return;

    // Effect Timing Handlers
    if (_effect == EFFECT_BLINK) {
        if (millis() - _lastBlinkTime > 500) {
            _lastBlinkTime = millis();
            _blinkState = !_blinkState;
        }
        if (!_blinkState) {
            _display.clear();
            return;
        }
    }

    if (_effect == EFFECT_PULSE) {
        _pulsePhase += 3;
        uint8_t b = sin8(_pulsePhase);
        _color1.nscale8(b);
        _color2.nscale8(b);
        _color3.nscale8(b);
    }

    if (_effect == EFFECT_SCROLL) {
        unsigned long interval = 110 - _speed; // 10ms to 100ms
        if (millis() - _lastUpdate > interval) {
            _lastUpdate = millis();
            _scrollOffset--;
            int totalTextLen = max(_line1.length(), max(_line2.length(), _line3.length()));
            if (_scrollOffset < -(int)(totalTextLen * 6 + 10)) {
                _scrollOffset = DISPLAY_WIDTH;
            }
        }
    }

    _display.clear();

    switch (_layout) {
        case LAYOUT_3LINE:     render3Line(); break;
        case LAYOUT_BIGHEADER: renderBigHeader(); break;
        case LAYOUT_BADGE:     renderBadge(); break;
        case LAYOUT_MARQUEE:   renderMarquee(); break;
    }
}
