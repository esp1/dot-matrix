#pragma once

#include "types.h"

namespace led_matrix {

// setup function

void setup(MD_MAX72XX *const matrix);

// inspect functions

uint8_t num_modules(MD_MAX72XX *const matrix);
uint16_t display_width(MD_MAX72XX *const matrix);
GraphicsBuffer *get_char_gfx(MD_MAX72XX *const matrix, uint16_t c);

// control functions

/**
 * @brief Clears the LED display.
 */
void clear(MD_MAX72XX *const matrix);

/**
 * @brief Sets the brightness of the LED display.
 * Brightness values range from 0 to 15 (MD_MAX72XX::MAX_INTENSITY).
 */
void set_brightness(MD_MAX72XX *const matrix, int intensity);

/**
 * @brief Sets the pixels in a column of the display.
 * The column value is given as an 8 bit value, with each bit representing a
 * pixel in the column. The least significant bit is the top pixel, and the most
 * significant bit is the bottom pixel.
 */
void set_column(MD_MAX72XX *const matrix, uint16_t column, uint8_t value);

/**
 * @brief Displays the contents of the graphics buffer on the LED display.
 */
void set_buffer(MD_MAX72XX *const matrix, int16_t column_offset,
                GraphicsBuffer *const graphics);

} // namespace led_matrix
