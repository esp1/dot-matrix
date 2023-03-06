#include "captive_portal.h"
#include "config.h"
#include "render.h"
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>

namespace captive_portal {

namespace {

bool _captive_portal_initialized = false;

DNSServer _dnsServer;

} // namespace

void loop(DotMatrixState *const state) {
  if (!_captive_portal_initialized) {
    Serial.println("Starting '" + String(CAPTIVE_PORTAL_SSID) +
                   "' captive portal access point network");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(CAPTIVE_PORTAL_SSID);

    _dnsServer.start(53, "*", WiFi.softAPIP());

    render::scroll_text(state, "dot matrix");

    _captive_portal_initialized = true;
  } else {
    _dnsServer.processNextRequest();
  }
}

void stop() {
  if (_captive_portal_initialized) {
    Serial.println("Shutting down '" + String(CAPTIVE_PORTAL_SSID) +
                   "' captive portal access point network");

    _dnsServer.stop();

    WiFi.disconnect();

    _captive_portal_initialized = false;
  }
}

} // namespace captive_portal