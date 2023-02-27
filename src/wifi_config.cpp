#include "wifi_config.h"
#include <EEPROM.h>

namespace wifi_config {

namespace {

WifiConfig eeprom_wifi_config;

} // namespace

void setup() {
  EEPROM.begin(512);
  EEPROM.get(0, eeprom_wifi_config);
  Serial.printf("wifi config ssid: '%s', password: '%s'\n",
                eeprom_wifi_config.ssid, eeprom_wifi_config.password);
}

WifiConfig *get() { return &eeprom_wifi_config; }

void save(String ssid, String password) {
  if (strcmp(ssid.c_str(), eeprom_wifi_config.ssid) != 0 ||
      strcmp(password.c_str(), eeprom_wifi_config.password) != 0) {
    Serial.println("Saving wifi config to EEPROM");

    strcpy(eeprom_wifi_config.ssid, ssid.c_str());
    strcpy(eeprom_wifi_config.password, password.c_str());

    EEPROM.put(0, eeprom_wifi_config);
    EEPROM.commit();
  }
}

} // namespace wifi_config