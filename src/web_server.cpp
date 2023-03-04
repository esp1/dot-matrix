#include "web_server.h"
#include "render.h"
#include <LittleFS.h>

namespace web_server {

void setup(AsyncWebServer *const server, MatrixState *const state) {
  LittleFS.begin();

  // Station mode
  server->serveStatic("/", LittleFS, "/web/").setDefaultFile("index.html");

  server->on("/matrix-text", [state](auto *req) {
    if (req->hasParam("matrix-text", true)) {
      render::scroll_text(state, req->getParam("matrix-text", true)->value());
      req->send(200, "text/plain", "OK");
    } else {
      req->send(400, "text", "Missing 'matrix-text' parameter");
    }
    req->redirect("/");
  });

  // Not Found
  server->onNotFound([](auto *req) {
    if (ON_AP_FILTER(req)) {
      req->redirect("http://" + WiFi.softAPIP().toString() + "/index.html");
    }
  });
}

} // namespace web_server
