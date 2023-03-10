#pragma once

#include "types.h"

namespace led_matrix {

void setup(MD_MAX72XX *const matrix);

GraphicsBuffer *const get_char_gfx(MD_MAX72XX *const matrix, uint16_t c);

} // namespace led_matrix
