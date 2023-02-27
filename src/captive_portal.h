#pragma once

#include "types.h"
#include <ESPAsyncWebServer.h>

namespace captive_portal {

void setup(AsyncWebServer *const server, ArRequestHandlerFunction callback);
void loop(MatrixState *const state);
void stop();

}