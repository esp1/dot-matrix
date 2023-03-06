#pragma once

#include <Arduino.h>

/**
 * @brief Number of MAX72XX LED Matrix modules connected together in the
 * display.
 */
#define NUM_LED_MATRIX_MODULES 4

/**
 * @brief The name to use for the captive portal wifi network.
 */
#define CAPTIVE_PORTAL_SSID "dot matrix"

/**
 * @brief The mDNS host name to use.
 * This is the host name for the LED matrix that will be advertised via mDNS.
 * The full host name will be '<MDNS_HOST_NAME>.local' - so e.g. if the
 * MDNS_HOST_NAME is set to 'dotmatrix', then the fully qualified host name will
 * be 'dotmatrix.local'.
 */
#define MDNS_HOST_NAME "dotmatrix"