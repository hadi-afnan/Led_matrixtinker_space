#ifndef TINKERSPACE_CONFIG_H
#define TINKERSPACE_CONFIG_H

#include <Arduino.h>

// =============================================================================
// LAYOUT SELECTION
// =============================================================================
// Select ONE layout definition by commenting / uncommenting:
#define CONFIG_LAYOUT_5X3      // Physical Prototype: 5x3 modules (50x30 = 1,500 LEDs)
// #define CONFIG_LAYOUT_4X3   // Earlier 4:3 Target: 4x3 modules (40x30 = 1,200 LEDs)

#if defined(CONFIG_LAYOUT_5X3)
  #define MATRIX_MODULES_X   5
  #define MATRIX_MODULES_Y   3
  #define LAYOUT_NAME        "5x3 Prototype (50x30)"
#elif defined(CONFIG_LAYOUT_4X3)
  #define MATRIX_MODULES_X   4
  #define MATRIX_MODULES_Y   3
  #define LAYOUT_NAME        "4x3 Target (40x30)"
#else
  #error "Please define either CONFIG_LAYOUT_5X3 or CONFIG_LAYOUT_4X3 in config.h"
#endif

// =============================================================================
// MATRIX GEOMETRY DEFINITIONS
// =============================================================================
#define MODULE_PIXELS_X      10
#define MODULE_PIXELS_Y      10
#define LEDS_PER_MODULE      (MODULE_PIXELS_X * MODULE_PIXELS_Y) // 100 LEDs per module

#define DISPLAY_WIDTH        (MATRIX_MODULES_X * MODULE_PIXELS_X)
#define DISPLAY_HEIGHT       (MATRIX_MODULES_Y * MODULE_PIXELS_Y)
#define TOTAL_LEDS           (DISPLAY_WIDTH * DISPLAY_HEIGHT)

// =============================================================================
// PINOUT DEFINITIONS (ESP32-S3)
// =============================================================================
// Data output connected to 74AHCT125 level shifter input (pin 1A)
#define PIN_LED_DATA         4

// Tactile Buttons (Active LOW with internal INPUT_PULLUP)
#define PIN_BTN_NEXT        14   // Button 1: Next Animation Pattern
#define PIN_BTN_PAUSE       15   // Button 2: Pause / Resume Animation

// =============================================================================
// ELECTRICAL & POWER SAFETY LIMITS
// =============================================================================
#define LED_TYPE             WS2812B
#define COLOR_ORDER          GRB

// Global brightness (0 to 255)
#define DEFAULT_BRIGHTNESS   120
#define MAX_BRIGHTNESS       200

// FastLED Dynamic Milliamps Limiter
// Prevents pulling excessive current from the 5V power supply
#define POWER_LIMIT_VOLTS    5
#define POWER_LIMIT_MILLIAMPS 25000 // 25 Amps (125 Watts limit for safe operation)

// Frame rate throttle (FPS)
#define TARGET_FRAME_RATE    60

#endif // TINKERSPACE_CONFIG_H
