#include "web_server.h"
#include "render.h"
#include <LittleFS.h>

namespace web_server {

void setup(AsyncWebServer *const server, MatrixState *const state) {
  LittleFS.begin();

  // Station mode
  server->serveStatic("/", LittleFS, "/web/app/")
      .setDefaultFile("index.html")
      .setFilter(ON_STA_FILTER);

  server->on("/matrix", [state](auto *req) {
    if (req->hasParam("text")) {
      render::scroll_text(state, req->getParam("text")->value());
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
