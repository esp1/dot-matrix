#include "web_server.h"
#include "led_matrix.h"
#include "render.h"
#include <LittleFS.h>

namespace web_server {

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

  // Application handlers
  server->on("/matrix-text", [state](auto *req) {
    if (req->hasParam("matrix-text", true)) {
      render::scroll_text(state, req->getParam("matrix-text", true)->value());
      req->send(200, "text/plain", "OK");
    } else {
      req->send(400, "text/plain", "Missing 'matrix-text' parameter");
    }
    req->redirect("/");
  });

  server->on("/brightness", [state](auto *req) {
    if (req->hasParam("brightness", true)) {
      led_matrix::set_brightness(
          state->matrix, req->getParam("brightness", true)->value().toInt());
      req->send(200, "text/plain", "OK");
    } else {
      req->send(400, "text/plain", "Missing 'brightness' parameter");
    }
  });

}

} // namespace web_server
