#include "captive_portal.h"
#include "config.h"
#include "render.h"
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>

namespace captive_portal {

namespace {

bool captive_portal_initialized = false;

DNSServer dnsServer;

} // namespace

void loop(DotMatrixState *const state) {
  if (!captive_portal_initialized) {
    Serial.println("Starting '" + CAPTIVE_PORTAL_SSID +
                   "' captive portal access point network");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(CAPTIVE_PORTAL_SSID);

    dnsServer.start(53, "*", WiFi.softAPIP());

    render::scroll_text(state, "dot matrix");

    captive_portal_initialized = true;
  } else {
    dnsServer.processNextRequest();
  }
}

void stop() {
  if (captive_portal_initialized) {
    Serial.println("Shutting down '" + CAPTIVE_PORTAL_SSID +
                   "' captive portal access point network");

    dnsServer.stop();

    WiFi.disconnect();

    captive_portal_initialized = false;
  }
}

} // namespace captive_portal