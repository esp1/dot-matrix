#include "web_server.h"
#include "ArduinoJson.h"
#include "led_matrix.h"
#include "render.h"
#include <LittleFS.h>

namespace web_server {

namespace {

/**
 * @brief Converts a given speed value to an associated loop delay in
 * milliseconds. Speed values are constrained to the range from 1 to 20. A speed
 * value of 20 corresponds to the maximum speed, or a 10 milliseconds loop
 * delay. A speed value of 1 corresponds to the minimum speed, or approximately
 * a 100 millisecond loop delay.
 */
uint16_t _speed_to_loop_delay_msec(long s) {
  auto speed = constrain(s, 1, 20);
  uint16_t loop_delay_msec = 10 + pow(20 - speed, 1.5);
  return loop_delay_msec;
}

uint8_t _loop_delay_msec_to_speed(uint16_t loop_delay_msec) {
  auto speed = 20 - pow(loop_delay_msec - 10, 0.6666666667);
  return constrain(speed, 1, 20);
}

String _state_json(DotMatrixState *const state) {
  StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
  doc["brightness"] = state->brightness;
  doc["speed_dir"] = state->scroll_dir == SCROLL_NONE
                         ? 0
                         : (state->scroll_dir == SCROLL_LEFT ? -1 : 1) *
                               _loop_delay_msec_to_speed(render::loop_delay_msec());

  String output = "";
  serializeJson(doc, output);

  return output;
}

} // namespace

void setup(AsyncWebServer *const server, MatrixState *const state) {
  LittleFS.begin();

  // Static files, default file
  server->serveStatic("/", LittleFS, "/web/").setDefaultFile("index.html");

  // Not Found
  server->onNotFound([](auto *req) {
    if (ON_AP_FILTER(req)) {
      req->redirect("http://" + WiFi.softAPIP().toString() + "/index.html");
    }
  });

  // Dot Matrix state
  server->on("/state", [state](auto *req) {
    req->send(200, "application/json", _state_json(state));
  });

  // Application handlers
  server->on("/matrix-text", [state](auto *req) {
    if (req->hasParam("matrix-text", true)) {
      render::scroll_text(state, req->getParam("matrix-text", true)->value(), state->scroll_dir);
      req->send(200, "text/plain", "OK");
    } else {
      req->send(400, "text/plain", "Missing 'matrix-text' parameter");
    }
    req->redirect("/");
  });

  server->on("/speed-dir", [state](auto *req) {
    if (req->hasParam("speed-dir", true)) {
      auto speed_dir = req->getParam("speed-dir", true)->value().toInt();

      if (speed_dir < 0) {
        state->scroll_dir = SCROLL_LEFT;
        render::set_loop_delay_msec(_speed_to_loop_delay_msec(-speed_dir));
      } else if (speed_dir > 0) {
        state->scroll_dir = SCROLL_RIGHT;
        render::set_loop_delay_msec(_speed_to_loop_delay_msec(speed_dir));
      } else {
        state->scroll_dir = SCROLL_NONE;
      }

      req->send(200, "text/plain", "OK");
    } else {
      req->send(400, "text/plain", "Missing 'speed-dir' parameter");
    }
  });

  server->on("/brightness", [state](auto *req) {
    if (req->hasParam("brightness", true)) {
      state->brightness = req->getParam("brightness", true)->value().toInt();
      led_matrix::set_brightness(state->matrix, state->brightness);
      req->send(200, "text/plain", "OK");
    } else {
      req->send(400, "text/plain", "Missing 'brightness' parameter");
    }
  });
}

} // namespace web_server
