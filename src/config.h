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
const String CAPTIVE_PORTAL_SSID = "dot matrix";

/**
 * @brief Number of milliseconds to delay in the main loop.
 * Adjusting this to a lower value will increase the refresh rate and scrolling
 * speed of the display, and vice versa.
 */
#define LOOP_DELAY_MSEC 100