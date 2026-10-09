#include "web_server_handler.h"

WebServerHandler::WebServerHandler(DisplayMapping& display, AnimationEngine& engine, TextRenderer& textRenderer, CRGB* leds)
    : _display(display),
      _engine(engine),
      _textRenderer(textRenderer),
      _leds(leds),
      _server(WEB_SERVER_PORT),
      _activeSource(SOURCE_ANIMATION),
      _powerOn(true) {}

void WebServerHandler::begin() {
    // 1. Start Wi-Fi in SoftAP + Station mode
    WiFi.mode(WIFI_AP_STA);

    // Setup Access Point for standalone direct connection
    bool apSuccess = WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD, WIFI_AP_CHANNEL);
    Serial.println();
    Serial.println(F("=========================================="));
    Serial.println(F("   TINKERSPACE WI-FI & WEB CONTROLLER     "));
    Serial.println(F("=========================================="));
    if (apSuccess) {
        Serial.print(F("Access Point SSID : ")); Serial.println(WIFI_AP_SSID);
        Serial.print(F("AP IP Address     : http://")); Serial.println(WiFi.softAPIP());
    } else {
        Serial.println(F("Warning: Failed to start Wi-Fi Access Point!"));
    }

    // Connect to Station if credentials supplied
    if (String(WIFI_STA_SSID).length() > 0) {
        Serial.print(F("Connecting to Local Wi-Fi: ")); Serial.println(WIFI_STA_SSID);
        WiFi.begin(WIFI_STA_SSID, WIFI_STA_PASSWORD);
        unsigned long startAttempt = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 8000) {
            delay(250);
            Serial.print(F("."));
        }
        Serial.println();
        if (WiFi.status() == WL_CONNECTED) {
            Serial.print(F("Station IP Address: http://")); Serial.println(WiFi.localIP());
        } else {
            Serial.println(F("Station connect timed out. Continuing with AP mode."));
        }
    }

    // 2. Start mDNS (http://tinkerspace.local)
    if (MDNS.begin(MDNS_HOSTNAME)) {
        Serial.print(F("mDNS Responder    : http://"));
        Serial.print(MDNS_HOSTNAME);
        Serial.println(F(".local"));
        MDNS.addService("http", "tcp", WEB_SERVER_PORT);
    }

    // 3. Register HTTP Routes
    registerRoutes();

    // 4. Start HTTP Server
    _server.begin();
    Serial.println(F("HTTP Web Server running on port 80."));
    Serial.println(F("=========================================="));
}

void WebServerHandler::handleClient() {
    _server.handleClient();
}

void WebServerHandler::sendJsonResponse(int code, const String& json) {
    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    _server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    _server.send(code, "application/json", json);
}

void WebServerHandler::registerRoutes() {
    // OPTIONS preflight handler for CORS
    _server.onNotFound([this]() {
        if (_server.method() == HTTP_OPTIONS) {
            _server.sendHeader("Access-Control-Allow-Origin", "*");
            _server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            _server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
            _server.send(204);
        } else {
            _server.send(404, "text/plain", "Not Found");
        }
    });

    // Root UI Page
    _server.on("/", HTTP_GET, [this]() { handleRoot(); });

    // REST API Routes
    _server.on("/api/status", HTTP_GET, [this]() { handleApiStatus(); });
    _server.on("/api/brightness", HTTP_POST, [this]() { handleApiBrightness(); });
    _server.on("/api/power", HTTP_POST, [this]() { handleApiPower(); });
    _server.on("/api/clear", HTTP_POST, [this]() { handleApiClear(); });
    _server.on("/api/mode", HTTP_POST, [this]() { handleApiMode(); });
    _server.on("/api/pixel", HTTP_POST, [this]() { handleApiPixel(); });
    _server.on("/api/frame_raw", HTTP_POST, [this]() { handleApiFrameRaw(); });
    _server.on("/api/frame", HTTP_GET, [this]() { handleApiGetFrame(); });
    _server.on("/api/text", HTTP_POST, [this]() { handleApiText(); });
    _server.on("/api/image", HTTP_POST, [this]() { handleApiImage(); });
    _server.on("/api/wifi", HTTP_POST, [this]() { handleApiWiFi(); });
}

// -----------------------------------------------------------------------------
// Route Handlers
// -----------------------------------------------------------------------------
void WebServerHandler::handleRoot() {
    _server.sendHeader("Content-Encoding", "identity");
    _server.send_P(200, "text/html", WEB_UI_HTML);
}

void WebServerHandler::handleApiStatus() {
    String json = "{";
    json += "\"ok\":true,";
    json += "\"power\":" + String(_powerOn ? "true" : "false") + ",";
    json += "\"brightness\":" + String(FastLED.getBrightness()) + ",";
    json += "\"activeSource\":" + String((int)_activeSource) + ",";
    json += "\"animationIndex\":" + String(_engine.getModeIndex()) + ",";
    json += "\"animationName\":\"" + String(_engine.getModeName()) + "\",";
    json += "\"width\":" + String(DISPLAY_WIDTH) + ",";
    json += "\"height\":" + String(DISPLAY_HEIGHT) + ",";
    json += "\"totalLeds\":" + String(TOTAL_LEDS) + ",";
    json += "\"apIP\":\"" + WiFi.softAPIP().toString() + "\",";
    json += "\"staIP\":\"" + (WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "Disconnected") + "\",";
    json += "\"freeHeap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"uptimeSeconds\":" + String(millis() / 1000);
    json += "}";
    sendJsonResponse(200, json);
}

void WebServerHandler::handleApiBrightness() {
    String body = _server.arg("plain");
    int b = extractJsonInt(body, "brightness", -1);
    if (b >= 0 && b <= 255) {
        FastLED.setBrightness(b);
        FastLED.show();
        sendJsonResponse(200, "{\"ok\":true,\"brightness\":" + String(b) + "}");
    } else {
        sendJsonResponse(400, "{\"ok\":false,\"error\":\"Invalid brightness value\"}");
    }
}

void WebServerHandler::handleApiPower() {
    String body = _server.arg("plain");
    if (body.indexOf("\"power\":false") >= 0 || body.indexOf("\"power\": 0") >= 0) {
        _powerOn = false;
        FastLED.clear();
        FastLED.show();
    } else {
        _powerOn = true;
    }
    sendJsonResponse(200, "{\"ok\":true,\"power\":" + String(_powerOn ? "true" : "false") + "}");
}

void WebServerHandler::handleApiClear() {
    _activeSource = SOURCE_IMAGE_BUFFER;
    _display.clear();
    FastLED.show();
    sendJsonResponse(200, "{\"ok\":true,\"cleared\":true}");
}

void WebServerHandler::handleApiMode() {
    String body = _server.arg("plain");
    int mode = extractJsonInt(body, "mode", 0);
    _engine.setMode(mode);
    _activeSource = SOURCE_ANIMATION;
    sendJsonResponse(200, "{\"ok\":true,\"mode\":" + String(mode) + ",\"name\":\"" + String(_engine.getModeName()) + "\"}");
}

void WebServerHandler::handleApiPixel() {
    String body = _server.arg("plain");
    int x = extractJsonInt(body, "x", -1);
    int y = extractJsonInt(body, "y", -1);
    int r = extractJsonInt(body, "r", 0);
    int g = extractJsonInt(body, "g", 0);
    int b = extractJsonInt(body, "b", 0);

    if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT) {
        _activeSource = SOURCE_IMAGE_BUFFER;
        _display.setPixel(x, y, CRGB(r, g, b));
        FastLED.show();
        sendJsonResponse(200, "{\"ok\":true}");
    } else {
        sendJsonResponse(400, "{\"ok\":false,\"error\":\"Coordinates out of bounds\"}");
    }
}

void WebServerHandler::handleApiFrameRaw() {
    // Direct binary stream (4,500 bytes = 1500 pixels * 3 channels)
    if (_server.hasArg("plain")) {
        const String& body = _server.arg("plain");
        const uint8_t* rawBytes = (const uint8_t*)body.c_str();
        size_t len = body.length();

        size_t pixelCount = min((size_t)TOTAL_LEDS, len / 3);
        _activeSource = SOURCE_IMAGE_BUFFER;

        for (size_t i = 0; i < pixelCount; i++) {
            int x = i % DISPLAY_WIDTH;
            int y = i / DISPLAY_WIDTH;
            _display.setPixel(x, y, CRGB(rawBytes[i * 3], rawBytes[i * 3 + 1], rawBytes[i * 3 + 2]));
        }

        FastLED.show();
        sendJsonResponse(200, "{\"ok\":true,\"pixelsLoaded\":" + String(pixelCount) + "}");
        return;
    }
    sendJsonResponse(400, "{\"ok\":false,\"error\":\"Missing binary payload\"}");
}

void WebServerHandler::handleApiGetFrame() {
    // Returns current RGB pixel frame for live simulator sync
    size_t dataLen = TOTAL_LEDS * 3;
    uint8_t* buffer = (uint8_t*)malloc(dataLen);
    if (!buffer) {
        sendJsonResponse(500, "{\"ok\":false,\"error\":\"Out of memory\"}");
        return;
    }

    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int idx = (y * DISPLAY_WIDTH + x) * 3;
            CRGB col = _display.getPixel(x, y);
            buffer[idx]     = col.r;
            buffer[idx + 1] = col.g;
            buffer[idx + 2] = col.b;
        }
    }

    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Content-Type", "application/octet-stream");
    _server.send_P(200, "application/octet-stream", (const char*)buffer, dataLen);
    free(buffer);
}

void WebServerHandler::handleApiText() {
    String body = _server.arg("plain");

    // If client sent raw pixels array inside text payload, check for it
    int pixelsIdx = body.indexOf("\"pixels\":[");
    if (pixelsIdx >= 0) {
        // Fast direct pixel extraction from client render
        _activeSource = SOURCE_IMAGE_BUFFER;
        int startPos = pixelsIdx + 10;
        int currentPixel = 0;
        int r = 0, g = 0, b = 0, channel = 0;

        while (startPos < body.length() && currentPixel < TOTAL_LEDS) {
            char ch = body.charAt(startPos);
            if (ch >= '0' && ch <= '9') {
                int val = 0;
                while (startPos < body.length() && body.charAt(startPos) >= '0' && body.charAt(startPos) <= '9') {
                    val = val * 10 + (body.charAt(startPos) - '0');
                    startPos++;
                }
                if (channel == 0) r = val;
                else if (channel == 1) g = val;
                else if (channel == 2) {
                    b = val;
                    int x = currentPixel % DISPLAY_WIDTH;
                    int y = currentPixel / DISPLAY_WIDTH;
                    _display.setPixel(x, y, CRGB(r, g, b));
                    currentPixel++;
                    channel = -1;
                }
                channel++;
            } else if (ch == ']') {
                break;
            } else {
                startPos++;
            }
        }
        FastLED.show();
        sendJsonResponse(200, "{\"ok\":true,\"mode\":\"direct_pixels\"}");
        return;
    }

    // Otherwise, configure native text engine
    String l1 = extractJsonString(body, "line1");
    String l2 = extractJsonString(body, "line2");
    String l3 = extractJsonString(body, "line3");

    CRGB c1 = parseHexColor(extractJsonString(body, "color1"));
    CRGB c2 = parseHexColor(extractJsonString(body, "color2"));
    CRGB c3 = parseHexColor(extractJsonString(body, "color3"));

    String layoutStr = extractJsonString(body, "layout");
    String effectStr = extractJsonString(body, "effect");
    String alignStr  = extractJsonString(body, "align");
    String badgeIcon = extractJsonString(body, "badgeIcon");
    int speed        = extractJsonInt(body, "speed", 40);

    _textRenderer.setText(l1, l2, l3);
    _textRenderer.setColors(c1, c2, c3);
    _textRenderer.setAlignment(alignStr.length() ? alignStr : "center");
    _textRenderer.setBadgeIcon(badgeIcon.length() ? badgeIcon : "check");
    _textRenderer.setSpeed(speed);

    if (layoutStr == "bigheader") _textRenderer.setLayout(LAYOUT_BIGHEADER);
    else if (layoutStr == "badge") _textRenderer.setLayout(LAYOUT_BADGE);
    else if (layoutStr == "marquee") _textRenderer.setLayout(LAYOUT_MARQUEE);
    else _textRenderer.setLayout(LAYOUT_3LINE);

    if (effectStr == "scroll") _textRenderer.setEffect(EFFECT_SCROLL);
    else if (effectStr == "blink") _textRenderer.setEffect(EFFECT_BLINK);
    else if (effectStr == "pulse") _textRenderer.setEffect(EFFECT_PULSE);
    else if (effectStr == "rainbow") _textRenderer.setEffect(EFFECT_RAINBOW);
    else _textRenderer.setEffect(EFFECT_STATIC);

    _textRenderer.setActive(true);
    _activeSource = SOURCE_TEXT_SIGN;

    sendJsonResponse(200, "{\"ok\":true,\"mode\":\"native_text\"}");
}

void WebServerHandler::handleApiImage() {
    String body = _server.arg("plain");

    // Fast integer parser for pixels array
    int pixelsIdx = body.indexOf("\"pixels\":[");
    if (pixelsIdx >= 0) {
        _activeSource = SOURCE_IMAGE_BUFFER;
        int startPos = pixelsIdx + 10;
        int currentPixel = 0;
        int r = 0, g = 0, b = 0, channel = 0;

        while (startPos < body.length() && currentPixel < TOTAL_LEDS) {
            char ch = body.charAt(startPos);
            if (ch >= '0' && ch <= '9') {
                int val = 0;
                while (startPos < body.length() && body.charAt(startPos) >= '0' && body.charAt(startPos) <= '9') {
                    val = val * 10 + (body.charAt(startPos) - '0');
                    startPos++;
                }
                if (channel == 0) r = val;
                else if (channel == 1) g = val;
                else if (channel == 2) {
                    b = val;
                    int x = currentPixel % DISPLAY_WIDTH;
                    int y = currentPixel / DISPLAY_WIDTH;
                    _display.setPixel(x, y, CRGB(r, g, b));
                    currentPixel++;
                    channel = -1;
                }
                channel++;
            } else if (ch == ']') {
                break;
            } else {
                startPos++;
            }
        }
        FastLED.show();
        sendJsonResponse(200, "{\"ok\":true,\"pixelsLoaded\":" + String(currentPixel) + "}");
        return;
    }
    sendJsonResponse(400, "{\"ok\":false,\"error\":\"Invalid image payload\"}");
}

void WebServerHandler::handleApiWiFi() {
    String body = _server.arg("plain");
    String ssid = extractJsonString(body, "ssid");
    String pass = extractJsonString(body, "password");

    if (ssid.length() > 0) {
        WiFi.begin(ssid.c_str(), pass.c_str());
        sendJsonResponse(200, "{\"ok\":true,\"status\":\"Connecting to " + ssid + "\"}");
    } else {
        sendJsonResponse(400, "{\"ok\":false,\"error\":\"SSID cannot be empty\"}");
    }
}

// -----------------------------------------------------------------------------
// Parsing Utilities
// -----------------------------------------------------------------------------
CRGB WebServerHandler::parseHexColor(const String& hex) {
    String cleanHex = hex;
    if (cleanHex.startsWith("#")) cleanHex = cleanHex.substring(1);
    if (cleanHex.length() != 6) return CRGB::White;

    long number = strtol(cleanHex.c_str(), NULL, 16);
    uint8_t r = (number >> 16) & 0xFF;
    uint8_t g = (number >> 8) & 0xFF;
    uint8_t b = number & 0xFF;
    return CRGB(r, g, b);
}

String WebServerHandler::extractJsonString(const String& json, const String& key) {
    String searchKey = "\"" + key + "\":\"";
    int start = json.indexOf(searchKey);
    if (start < 0) return "";
    start += searchKey.length();
    int end = json.indexOf("\"", start);
    if (end < 0) return "";
    return json.substring(start, end);
}

int WebServerHandler::extractJsonInt(const String& json, const String& key, int defaultVal) {
    String searchKey = "\"" + key + "\":";
    int start = json.indexOf(searchKey);
    if (start < 0) return defaultVal;
    start += searchKey.length();
    while (start < json.length() && (json.charAt(start) == ' ' || json.charAt(start) == '\"')) start++;
    int end = start;
    while (end < json.length() && ((json.charAt(end) >= '0' && json.charAt(end) <= '9') || json.charAt(end) == '-')) end++;
    if (start == end) return defaultVal;
    return json.substring(start, end).toInt();
}
