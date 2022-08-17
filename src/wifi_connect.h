#pragma once

#include <DNSServer.h>
#include <EEPROM.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>

namespace tint_matrix::wifi_connect
{
    DNSServer dnsServer;

    namespace
    {
        String CAPTIVE_PORTAL_AP_NAME = "TINT Matrix";

        bool captive_portal_initialized = false;
        bool wifi_connected = false;

        struct wifiConfig_t {
            char ssid[256];
            char password[256];
        } eeprom_wifi_config;

        String ssid;
        String password;
    }

    void setup(AsyncWebServer *const server, Matrix *const matrix, MatrixState *const state)
    {
        EEPROM.begin(512);
        EEPROM.get(0, eeprom_wifi_config);
        ssid = eeprom_wifi_config.ssid;
        password = eeprom_wifi_config.password;
        // Serial.printf("wifi config ssid: '%s', password: '%s'\n", ssid, password);

        // Captive portal
        server->serveStatic("/", LittleFS, "/web/captive_portal/").setFilter(ON_AP_FILTER);

        server->on("/matrix", [matrix, state](auto *req)
            {
              if (req->hasParam("text"))
              {
                matrix->update(state->clear()->reset_column_offset());
                state->text(req->getParam("text")->value().c_str())->scroll();
              }
              req->redirect("/");
            });
        
        server->on("/setup", [](auto *req)
                   {
                       ssid = req->getParam("wifi-ssid")->value().c_str();
                       password = req->getParam("wifi-password")->value().c_str(); })
            .setFilter(ON_AP_FILTER);
        
        // Not Found
        server->onNotFound([](auto *req)
                           {
                               if (ON_AP_FILTER(req))
                               {
                                   req->redirect("http://" + WiFi.softAPIP().toString() + "/index.html");
                               } });
    }

    MatrixState *loop(Matrix *const matrix, MatrixState *const state)
    {
        MatrixState *end_state = state;

        if (!wifi_connected)
        {
            if (ssid.isEmpty())
            {
                if (!captive_portal_initialized)
                {
                    Serial.println("Starting '" + CAPTIVE_PORTAL_AP_NAME + "' captive portal access point network");
                    WiFi.mode(WIFI_AP);
                    WiFi.softAP(CAPTIVE_PORTAL_AP_NAME);
                    dnsServer.start(53, "*", WiFi.softAPIP());
                    captive_portal_initialized = true;
                    end_state = state->text("setup")->align(ALIGN_CENTER);
                }
                else
                {
                    dnsServer.processNextRequest();
                }
            }
            else
            {
                if (captive_portal_initialized)
                {
                    Serial.println("Shutting down '" + CAPTIVE_PORTAL_AP_NAME + "' captive portal access point network");
                    dnsServer.stop();
                    WiFi.disconnect();
                    captive_portal_initialized = false;
                }

                Serial.printf("Attempting to connect to wifi network: '%s'\n", ssid.c_str());
                end_state = matrix->update(state->text("\x1a wifi")->align(ALIGN_CENTER));
                WiFi.mode(WIFI_STA);
                WiFi.begin(ssid, password);
                if (WiFi.waitForConnectResult() != WL_CONNECTED)
                {
                    Serial.println("Failed connection attempt");
                    ssid.clear();
                    password.clear();
                }
                else
                {
                    Serial.println("Connected! IP address: " + WiFi.localIP().toString());

                    if (strcmp(ssid.c_str(), eeprom_wifi_config.ssid) != 0 || strcmp(password.c_str(), eeprom_wifi_config.password) != 0) {
                        Serial.println("Saving wifi config to EEPROM");
                        strcpy(eeprom_wifi_config.ssid, ssid.c_str());
                        strcpy(eeprom_wifi_config.password, password.c_str());
                        EEPROM.put(0, eeprom_wifi_config);
                        EEPROM.commit();
                    }

                    wifi_connected = true;
                    end_state = matrix->update(state->clear()->reset_column_offset());
                    end_state = state->text(WiFi.localIP().toString().c_str())->scroll();
                }
            }
        }

        return end_state;
    }
}
