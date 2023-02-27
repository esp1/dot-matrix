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

void clear(MD_MAX72XX *const matrix);
void set_brightness(MD_MAX72XX *const matrix, int intensity);
void set_column(MD_MAX72XX *const matrix, uint16_t column, uint8_t value);
void set_row(MD_MAX72XX *const matrix, uint8_t row, uint8_t value);
void set_buffer(MD_MAX72XX *const matrix, uint16_t column_offset,
                GraphicsBuffer *const graphics);

} // namespace led_matrix
