#include "config.h"
#include "led_matrix.h"
#include "render.h"
#include "types.h"
#include "web_server.h"
#include "wifi_connect.h"
#include <ESPAsyncWebServer.h>

MD_MAX72XX matrix(MD_MAX72XX::FC16_HW,
                  15,                    // CS
                  NUM_LED_MATRIX_MODULES // # of devices
);
MatrixState state(&matrix);
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("TINT Matrix");
  Serial.println();

  led_matrix::setup(&matrix);
  web_server::setup(&server, &state);
  wifi_connect::setup(&server);

  server.begin();
}

void loop() {
  wifi_connect::loop(&state);
  render::update_display(&state);

  delay(LOOP_DELAY_MSEC);
}