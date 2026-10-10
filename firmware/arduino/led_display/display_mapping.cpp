#include "display_mapping.h"

// Basic 5x7 ASCII font table for printable characters (ASCII 32 ' ' to 126 '~')
// Each byte represents a column of 7 vertical pixels (bit 0 = top, bit 6 = bottom)
static const uint8_t FONT_5X7[][5] PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 32 ' '
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // 33 '!'
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 34 '"'
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // 35 '#'
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // 36 '$'
    {0x23, 0x13, 0x08, 0x64, 0x62}, // 37 '%'
    {0x36, 0x49, 0x55, 0x22, 0x50}, // 38 '&'
    {0x00, 0x05, 0x03, 0x00, 0x00}, // 39 '''
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // 40 '('
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // 41 ')'
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // 42 '*'
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 43 '+'
    {0x00, 0x50, 0x30, 0x00, 0x00}, // 44 ','
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 45 '-'
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 46 '.'
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 47 '/'
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 48 '0'
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 49 '1'
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 50 '2'
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 51 '3'
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 52 '4'
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 53 '5'
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 54 '6'
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 55 '7'
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 56 '8'
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 57 '9'
    {0x00, 0x36, 0x36, 0x00, 0x00}, // 58 ':'
    {0x00, 0x56, 0x36, 0x00, 0x00}, // 59 ';'
    {0x08, 0x14, 0x22, 0x41, 0x00}, // 60 '<'
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 61 '='
    {0x00, 0x41, 0x22, 0x14, 0x08}, // 62 '>'
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 63 '?'
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // 64 '@'
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // 65 'A'
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 66 'B'
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 67 'C'
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 68 'D'
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 69 'E'
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 70 'F'
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 71 'G'
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 72 'H'
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 73 'I'
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 74 'J'
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 75 'K'
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 76 'L'
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 77 'M'
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 78 'N'
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 79 'O'
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 80 'P'
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 81 'Q'
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 82 'R'
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 83 'S'
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 84 'T'
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 85 'U'
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 86 'V'
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 87 'W'
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 88 'X'
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 89 'Y'
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 90 'Z'
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // 91 '['
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 92 '\'
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // 93 ']'
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 94 '^'
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 95 '_'
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 96 '`'
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 97 'a'
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 98 'b'
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 99 'c'
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 100 'd'
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 101 'e'
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 102 'f'
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 103 'g'
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 104 'h'
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 105 'i'
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 106 'j'
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 107 'k'
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 108 'l'
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 109 'm'
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 110 'n'
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 111 'o'
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 112 'p'
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 113 'q'
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 114 'r'
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 115 's'
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 116 't'
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 117 'u'
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 118 'v'
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 119 'w'
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 120 'x'
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 121 'y'
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 122 'z'
    {0x00, 0x08, 0x36, 0x41, 0x00}, // 123 '{'
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 124 '|'
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 125 '}'
    {0x08, 0x08, 0x2A, 0x1C, 0x08}  // 126 '~'
};

MatrixWiringType DisplayMapping::_wiringType = (MatrixWiringType)MATRIX_WIRING_MODULE_SERPENTINE;

DisplayMapping::DisplayMapping(CRGB* ledArray) : _leds(ledArray) {}

int DisplayMapping::getPixelIndex(int x, int y) {
    // 1. Boundary Check
    if (x < 0 || x >= DISPLAY_WIDTH || y < 0 || y >= DISPLAY_HEIGHT) {
        return -1;
    }

    switch (_wiringType) {
        case WIRING_MODULE_SERPENTINE: {
            // Mode 2: Chained 10x10 modular tiles (horizontal serpentine between tiles, serpentine inside)
            const int moduleX = x / MODULE_PIXELS_X;
            const int moduleY = y / MODULE_PIXELS_Y;
            const int localX = x % MODULE_PIXELS_X;
            const int localY = y % MODULE_PIXELS_Y;
            const int moduleIndex = (moduleY & 1)
                ? (moduleY * MATRIX_MODULES_X) + (MATRIX_MODULES_X - 1 - moduleX)
                : (moduleY * MATRIX_MODULES_X) + moduleX;
            const int pixelInModule = (localY & 1)
                ? (localY * MODULE_PIXELS_X) + (MODULE_PIXELS_X - 1 - localX)
                : (localY * MODULE_PIXELS_X) + localX;
            return (moduleIndex * LEDS_PER_MODULE) + pixelInModule;
        }

        case WIRING_MODULE_PROGRESSIVE: {
            // Mode 3: Modular 10x10 progressive rows (left-to-right each module row, serpentine inside)
            const int moduleX = x / MODULE_PIXELS_X;
            const int moduleY = y / MODULE_PIXELS_Y;
            const int localX = x % MODULE_PIXELS_X;
            const int localY = y % MODULE_PIXELS_Y;
            const int moduleIndex = (moduleY * MATRIX_MODULES_X) + moduleX;
            const int pixelInModule = (localY & 1)
                ? (localY * MODULE_PIXELS_X) + (MODULE_PIXELS_X - 1 - localX)
                : (localY * MODULE_PIXELS_X) + localX;
            return (moduleIndex * LEDS_PER_MODULE) + pixelInModule;
        }

        case WIRING_COLUMN_SERPENTINE: {
            // Mode 4: Continuous column-serpentine (vertical 30-pixel columns alternating up and down)
            if ((x & 1) == 0) {
                return (x * DISPLAY_HEIGHT) + y;
            } else {
                return (x * DISPLAY_HEIGHT) + (DISPLAY_HEIGHT - 1 - y);
            }
        }

        case WIRING_ROW_SERPENTINE:
        default: {
            // Mode 1: Continuous row-serpentine across full display width (even rows L->R, odd rows R->L)
            if ((y & 1) == 0) {
                return (y * DISPLAY_WIDTH) + x;
            } else {
                return (y * DISPLAY_WIDTH) + (DISPLAY_WIDTH - 1 - x);
            }
        }
    }
}

void DisplayMapping::setPixel(int x, int y, CRGB color) {
    int index = getPixelIndex(x, y);
    if (index >= 0 && index < TOTAL_LEDS) {
        _leds[index] = color;
    }
}

CRGB DisplayMapping::getPixel(int x, int y) const {
    int index = getPixelIndex(x, y);
    if (index >= 0 && index < TOTAL_LEDS) {
        return _leds[index];
    }
    return CRGB::Black;
}

void DisplayMapping::clear() {
    fill(CRGB::Black);
}

void DisplayMapping::fill(CRGB color) {
    for (int i = 0; i < TOTAL_LEDS; i++) {
        _leds[i] = color;
    }
}

void DisplayMapping::drawLine(int x0, int y0, int x1, int y1, CRGB color) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        setPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void DisplayMapping::drawRect(int x, int y, int w, int h, CRGB color, bool filled) {
    if (filled) {
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                setPixel(x + i, y + j, color);
            }
        }
    } else {
        drawLine(x, y, x + w - 1, y, color);
        drawLine(x + w - 1, y, x + w - 1, y + h - 1, color);
        drawLine(x + w - 1, y + h - 1, x, y + h - 1, color);
        drawLine(x, y + h - 1, x, y, color);
    }
}

void DisplayMapping::drawChar(int x, int y, char c, CRGB color, int scale) {
    if (c < 32 || c > 126) return;
    int charIndex = c - 32;

    for (int col = 0; col < 5; col++) {
        uint8_t line = pgm_read_byte(&(FONT_5X7[charIndex][col]));
        for (int row = 0; row < 7; row++) {
            if (line & (1 << row)) {
                int px = x + col * scale;
                int py = y + (6 - row) * scale;
                if (scale <= 1) {
                    setPixel(px, py, color);
                } else {
                    for (int dx = 0; dx < scale; dx++) {
                        for (int dy = 0; dy < scale; dy++) {
                            setPixel(px + dx, py + dy, color);
                        }
                    }
                }
            }
        }
    }
}

void DisplayMapping::drawString(int x, int y, const char* str, CRGB color) {
    int cursorX = x;
    while (*str) {
        drawChar(cursorX, y, *str, color);
        cursorX += 6; // 5 pixels width + 1 pixel letter space
        str++;
    }
}

void DisplayMapping::drawModuleHighlight(int moduleIndex, CRGB color, bool drawBorder, bool drawNumber) {
    if (moduleIndex < 0 || moduleIndex >= (MATRIX_MODULES_X * MATRIX_MODULES_Y)) return;

    int mx, my;
    if (_wiringType == WIRING_MODULE_SERPENTINE) {
        my = moduleIndex / MATRIX_MODULES_X;
        mx = (my & 1) ? (MATRIX_MODULES_X - 1 - (moduleIndex % MATRIX_MODULES_X)) : (moduleIndex % MATRIX_MODULES_X);
    } else {
        my = moduleIndex / MATRIX_MODULES_X;
        mx = moduleIndex % MATRIX_MODULES_X;
    }

    int startX = mx * MODULE_PIXELS_X;
    int startY = my * MODULE_PIXELS_Y;

    // 1. Fill entire 10x10 module body
    for (int x = 0; x < MODULE_PIXELS_X; x++) {
        for (int y = 0; y < MODULE_PIXELS_Y; y++) {
            setPixel(startX + x, startY + y, color);
        }
    }

    // 2. Optional white bounding border around module
    if (drawBorder) {
        drawRect(startX, startY, MODULE_PIXELS_X, MODULE_PIXELS_Y, CRGB::White, false);
    }

    // 3. Optional module index number in black/dark center
    if (drawNumber) {
        if (moduleIndex < 10) {
            drawChar(startX + 3, startY + 2, '0' + moduleIndex, CRGB::Black);
        } else {
            drawChar(startX + 1, startY + 2, '1', CRGB::Black);
            drawChar(startX + 5, startY + 2, '0' + (moduleIndex - 10), CRGB::Black);
        }
    }
}

void DisplayMapping::drawModuleGrid(CRGB borderColor, CRGB dotColor) {
    clear();

    // Draw module perimeter boundaries every 10 pixels
    for (int x = 0; x < DISPLAY_WIDTH; x += MODULE_PIXELS_X) {
        drawLine(x, 0, x, DISPLAY_HEIGHT - 1, borderColor);
        drawLine(x + MODULE_PIXELS_X - 1, 0, x + MODULE_PIXELS_X - 1, DISPLAY_HEIGHT - 1, borderColor);
    }
    for (int y = 0; y < DISPLAY_HEIGHT; y += MODULE_PIXELS_Y) {
        drawLine(0, y, DISPLAY_WIDTH - 1, y, borderColor);
        drawLine(0, y + MODULE_PIXELS_Y - 1, DISPLAY_WIDTH - 1, y + MODULE_PIXELS_Y - 1, borderColor);
    }

    // Draw corner markers for each 10x10 module
    for (int mx = 0; mx < MATRIX_MODULES_X; mx++) {
        for (int my = 0; my < MATRIX_MODULES_Y; my++) {
            int sx = mx * MODULE_PIXELS_X;
            int sy = my * MODULE_PIXELS_Y;
            setPixel(sx, sy, dotColor);
            setPixel(sx + MODULE_PIXELS_X - 1, sy, dotColor);
            setPixel(sx, sy + MODULE_PIXELS_Y - 1, dotColor);
            setPixel(sx + MODULE_PIXELS_X - 1, sy + MODULE_PIXELS_Y - 1, dotColor);
        }
    }
}
