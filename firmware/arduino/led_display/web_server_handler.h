#ifndef TINKERSPACE_WEB_SERVER_HANDLER_H
#define TINKERSPACE_WEB_SERVER_HANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <FastLED.h>

#include "config.h"
#include "display_mapping.h"
#include "animations.h"
#include "text_renderer.h"
#include "web_ui.h"

enum ActiveDisplaySource {
    SOURCE_ANIMATION = 0,
    SOURCE_TEXT_SIGN,
    SOURCE_IMAGE_BUFFER
};

class WebServerHandler {
public:
    WebServerHandler(DisplayMapping& display, AnimationEngine& engine, TextRenderer& textRenderer, CRGB* leds);

    void begin();
    void handleClient();

    ActiveDisplaySource getActiveSource() const { return _activeSource; }
    void setActiveSource(ActiveDisplaySource src) { _activeSource = src; }

    bool isPowerOn() const { return _powerOn; }
    void setPowerOn(bool on) { _powerOn = on; }

private:
    DisplayMapping& _display;
    AnimationEngine& _engine;
    TextRenderer& _textRenderer;
    CRGB* _leds;

    WebServer _server;
    ActiveDisplaySource _activeSource;
    bool _powerOn;

    void registerRoutes();
    void handleRoot();
    void handleApiStatus();
    void handleApiBrightness();
    void handleApiPower();
    void handleApiClear();
    void handleApiMode();
    void handleApiPixel();
    void handleApiFrameRaw();
    void handleApiGetFrame();
    void handleApiText();
    void handleApiImage();
    void handleApiWiFi();

    // CORS and helper utilities
    void sendJsonResponse(int code, const String& json);
    CRGB parseHexColor(const String& hex);
    String extractJsonString(const String& json, const String& key);
    int extractJsonInt(const String& json, const String& key, int defaultVal = 0);
};

#endif // TINKERSPACE_WEB_SERVER_HANDLER_H
