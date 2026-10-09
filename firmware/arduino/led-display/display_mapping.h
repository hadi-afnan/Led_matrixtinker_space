#ifndef TINKERSPACE_DISPLAY_MAPPING_H
#define TINKERSPACE_DISPLAY_MAPPING_H

#include <Arduino.h>
#include <FastLED.h>
#include "config.h"

/**
 * @brief Manages 2D continuous coordinate rendering over physical serpentine WS2812B modules.
 */
class DisplayMapping {
public:
    DisplayMapping(CRGB* ledArray);

    /**
     * @brief Translates logical (X, Y) display coordinate into physical 1D LED index.
     * @param x Horizontal pixel position (0 to DISPLAY_WIDTH - 1)
     * @param y Vertical pixel position (0 to DISPLAY_HEIGHT - 1)
     * @return Physical LED index in leds[] array, or -1 if out of bounds.
     */
    static int getPixelIndex(int x, int y);

    /**
     * @brief Sets a pixel color at (X, Y) with boundary safety check.
     */
    void setPixel(int x, int y, CRGB color);

    /**
     * @brief Retrieves pixel color at (X, Y).
     */
    CRGB getPixel(int x, int y) const;

    /**
     * @brief Clears entire matrix to black.
     */
    void clear();

    /**
     * @brief Fills entire matrix with a single color.
     */
    void fill(CRGB color);

    /**
     * @brief Draws a line using Bresenham's algorithm.
     */
    void drawLine(int x0, int y0, int x1, int y1, CRGB color);

    /**
     * @brief Draws a rectangle.
     */
    void drawRect(int x, int y, int w, int h, CRGB color, bool filled = false);

    /**
     * @brief Draws a single ASCII character using a 5x7 font.
     */
    void drawChar(int x, int y, char c, CRGB color);

    /**
     * @brief Renders a text string.
     */
    void drawString(int x, int y, const char* str, CRGB color);

    int getWidth() const { return DISPLAY_WIDTH; }
    int getHeight() const { return DISPLAY_HEIGHT; }
    int getTotalLeds() const { return TOTAL_LEDS; }

private:
    CRGB* _leds;
};

#endif // TINKERSPACE_DISPLAY_MAPPING_H
