#include "wifi_connect.h"
#include "captive_portal.h"
#include "config.h"
#include "render.h"
#include "web_server.h"
#include "wifi_config.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <LittleFS.h>

namespace {

bool _wifi_connected = false;

String _ssid;
String _password;

void _connect_wifi(DotMatrixState *const state) {
  Serial.println("Attempting to connect to wifi network: '" + _ssid + "'");
  render::text(state, "\x1a wifi", ALIGN_CENTER);
  render::update_display(state);

  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Failed to connect to '" + _ssid + "'");
    _ssid.clear();
    _password.clear();
  } else {
    Serial.println("Connected to '" + _ssid +
                   "' with IP address: " + WiFi.localIP().toString());
    wifi_config::save(_ssid, _password);
    _wifi_connected = true;

    if (MDNS.begin(MDNS_HOST_NAME)) {
      Serial.println("mDNS host name: " + String(MDNS_HOST_NAME) + ".local");
      render::scroll_text(state, String(MDNS_HOST_NAME) + ".local - " +
                                     WiFi.localIP().toString());
    } else {
      Serial.println("Failed to start mDNS");
      render::scroll_text(state, WiFi.localIP().toString());
    }
  }
}

} // namespace

namespace wifi_connect {

void setup(AsyncWebServer *const server) {
  wifi_config::setup();
  auto wifi_config = wifi_config::get();
  _ssid = wifi_config->ssid;
  _password = wifi_config->password;

  server->on("/wifi-config", [](auto *const req) {
    _wifi_connected = false;

    if (req->hasParam("wifi-ssid", true) &&
        req->hasParam("wifi-password", true)) {
      _ssid = req->getParam("wifi-ssid", true)->value().c_str();
      _password = req->getParam("wifi-password", true)->value().c_str();
      req->send(200, "text/plain", "OK");
    } else {
      req->send(400, "text/plain",
                "Missing 'wifi-ssid' and 'wifi-password' parameters");
    }
  });
}

void loop(DotMatrixState *const state) {
  if (!_wifi_connected) {
    if (WiFi.isConnected()) {
      Serial.println("Disconnecting from '" + WiFi.SSID() + "'");
      WiFi.disconnect();
      Serial.println(WiFi.isConnected() ? "connected" : "disconnected");
      Serial.println("wifi_connected = false");

      if (MDNS.end()) {
        Serial.println("Closed mDNS responder");
      } else {
        Serial.println("Failed to close mDNS responder");
      }
    }

    if (_ssid.isEmpty()) {
      captive_portal::loop(state);
    } else {
      captive_portal::stop();

      _connect_wifi(state);
    }
  } else {
    MDNS.update();
  }
}

} // namespace wifi_connect