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

void setup(AsyncWebServer *const server, ArRequestHandlerFunction callback) {
  // Access point mode (Captive portal)
  server->serveStatic("/", LittleFS, "/web/captive_portal/")
      .setFilter(ON_AP_FILTER);

  server->on("/setup", callback).setFilter(ON_AP_FILTER);
}

void loop(MatrixState *const state) {
  if (!captive_portal_initialized) {
    Serial.println("Starting '" + CAPTIVE_PORTAL_SSID +
                   "' captive portal access point network");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(CAPTIVE_PORTAL_SSID);

    dnsServer.start(53, "*", WiFi.softAPIP());

    render::text(state, "setup", ALIGN_CENTER);
    render::update_display(state);

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