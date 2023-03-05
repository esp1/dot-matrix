#pragma once

#include "types.h"
#include <ESPAsyncWebServer.h>

namespace wifi_connect {

void setup(AsyncWebServer *const server);
void loop(DotMatrixState *const state);

} // namespace wifi_connect