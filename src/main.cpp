#include <Arduino.h>
#include <LittleFS.h>
#include "matrix.h"
#include "matrix_state.h"
#include "wifi_connect.h"

using namespace tint_matrix;
using namespace std::placeholders;

const auto num_matrix_modules = 4;

Matrix matrix{num_matrix_modules};
MatrixState *state = new MatrixState{matrix.display_width(), std::bind(&Matrix::char_gfx, &matrix, _1)};

AsyncWebServer server(80);

uint16_t loop_delay = 100;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("TINT Matrix");
  Serial.println();

  LittleFS.begin();

  wifi_connect::setup(&server, &matrix, state);

  // Application content
  server.serveStatic("/", LittleFS, "/web/app/").setDefaultFile("index.html").setFilter(ON_STA_FILTER);

  server.on("/matrix", [](auto *req)
            {
              if (req->hasParam("text"))
              {
                state = matrix.update(state->clear()->reset_column_offset());
                state = state->text(req->getParam("text")->value().c_str());
              }
              req->redirect("/");
            })
      .setFilter(ON_STA_FILTER);

  server.begin();
}

void loop()
{
  state = wifi_connect::loop(&matrix, state);

  state = matrix.update(state);
  delay(loop_delay);
}