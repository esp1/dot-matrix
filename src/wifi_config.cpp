#include "wifi_config.h"
#include <EEPROM.h>

namespace {

wifi_config::WifiConfig _eeprom_wifi_config;

} // namespace

namespace wifi_config {

void setup() {
  EEPROM.begin(512);
  EEPROM.get(0, _eeprom_wifi_config);
  // Serial.printf("wifi config ssid: '%s', password: '%s'\n",
  //               eeprom_wifi_config.ssid, eeprom_wifi_config.password);
}

WifiConfig *const get() { return &_eeprom_wifi_config; }

void save(String ssid, String password) {
  if ((strcmp(ssid.c_str(), _eeprom_wifi_config.ssid) != 0) ||
      (strcmp(password.c_str(), _eeprom_wifi_config.password) != 0)) {
    Serial.println("Saving wifi config to EEPROM");

    strcpy(_eeprom_wifi_config.ssid, ssid.c_str());
    strcpy(_eeprom_wifi_config.password, password.c_str());

    EEPROM.put(0, _eeprom_wifi_config);
    EEPROM.commit();
  }
}

} // namespace wifi_config