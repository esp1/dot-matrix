#pragma once

#include "types.h"
#include <ESPAsyncWebServer.h>

namespace web_server {

void setup(AsyncWebServer *const server, MatrixState *const state);
void on(AsyncWebServer *const server, const char *uri, ArRequestHandlerFunction callback);

}