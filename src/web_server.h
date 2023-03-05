#pragma once

#include "types.h"
#include <ESPAsyncWebServer.h>

namespace web_server {

void setup(AsyncWebServer *const server, DotMatrixState *const state);

} // namespace web_server