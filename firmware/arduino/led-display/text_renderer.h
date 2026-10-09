#ifndef TINKERSPACE_TEXT_RENDERER_H
#define TINKERSPACE_TEXT_RENDERER_H

#include <Arduino.h>
#include <FastLED.h>
#include "config.h"
#include "display_mapping.h"

enum TextLayoutStyle {
    LAYOUT_3LINE = 0,
    LAYOUT_BIGHEADER,
    LAYOUT_BADGE,
    LAYOUT_MARQUEE
};

enum TextEffectStyle {
    EFFECT_STATIC = 0,
    EFFECT_SCROLL,
    EFFECT_BLINK,
    EFFECT_PULSE,
    EFFECT_RAINBOW
};

class TextRenderer {
public:
    TextRenderer(DisplayMapping& display);

    void begin();
    void update();

    void setText(const String& l1, const String& l2, const String& l3);
    void setColors(CRGB c1, CRGB c2, CRGB c3);
    void setLayout(TextLayoutStyle layout);
    void setEffect(TextEffectStyle effect);
    void setAlignment(const String& align); // "left", "center", "right"
    void setBadgeIcon(const String& iconName);
    void setSpeed(uint8_t speed);

    bool isActive() const { return _active; }
    void setActive(bool active) { _active = active; }

private:
    DisplayMapping& _display;
    bool _active;

    String _line1;
    String _line2;
    String _line3;

    CRGB _color1;
    CRGB _color2;
    CRGB _color3;

    TextLayoutStyle _layout;
    TextEffectStyle _effect;
    String _align;
    String _badgeIcon;
    uint8_t _speed;

    int _scrollOffset;
    unsigned long _lastUpdate;
    unsigned long _lastBlinkTime;
    bool _blinkState;
    uint8_t _pulsePhase;

    void render3Line();
    void renderBigHeader();
    void renderBadge();
    void renderMarquee();
    void drawBadgeIcon(const String& icon, int x, int y);
    void drawLine(int y, const String& text, CRGB color, int scale = 1, int xOffset = 0);
};

#endif // TINKERSPACE_TEXT_RENDERER_H
