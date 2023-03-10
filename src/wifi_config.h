#pragma once

#include <Arduino.h>

namespace wifi_config {

typedef struct WifiConfig {
  char ssid[256];
  char password[256];
} WifiConfig;

void setup();
WifiConfig *const get();
void save(String ssid, String password);

}