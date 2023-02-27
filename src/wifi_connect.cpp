#include "wifi_connect.h"
#include "captive_portal.h"
#include "render.h"
#include "wifi_config.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <LittleFS.h>

namespace wifi_connect {

namespace {

bool wifi_connected = false;

String ssid;
String password;

void connect_wifi(MatrixState *const state) {
  Serial.println("Attempting to connect to wifi network: '" + ssid + "'");
  render::text(state, "\x1a wifi", ALIGN_CENTER);
  render::update_display(state);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Failed connection attempt");
    ssid.clear();
    password.clear();
  } else {
    Serial.println("Connected! IP address: " + WiFi.localIP().toString());
    render::scroll_text(state, WiFi.localIP().toString());

    wifi_config::save(ssid, password);

    wifi_connected = true;
  }
}

} // namespace

void setup(AsyncWebServer *const server) {
  wifi_config::setup();
  auto wifi_config = wifi_config::get();
  ssid = wifi_config->ssid;
  password = wifi_config->password;

  captive_portal::setup(server, [](auto *req) {
    ssid = req->getParam("wifi-ssid")->value().c_str();
    password = req->getParam("wifi-password")->value().c_str();
  });
}

void loop(MatrixState *const state) {
  if (!wifi_connected) {
    if (ssid.isEmpty()) {
      captive_portal::loop(state);
    } else {
      captive_portal::stop();

      connect_wifi(state);
    }
  }
}

} // namespace wifi_connect