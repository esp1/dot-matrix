#pragma once

#include "types.h"
#include <ESPAsyncWebServer.h>

namespace web_server {

void setup(AsyncWebServer *const server, MatrixState *const state);

} // namespace web_server