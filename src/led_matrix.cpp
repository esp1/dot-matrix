#include "led_matrix.h"

namespace {

/**
 * @brief C array used to store character graphics data from
 * MD_MAX72XX::getChar() function.
 */
uint8_t *_char_buf;

/**
 * @brief std::vector used to return character graphics data.
 */
GraphicsBuffer _char_gfx;

} // namespace

namespace led_matrix {

void setup(MD_MAX72XX *const matrix) {
  matrix->begin();

  // getMaxFontWidth() has to be called after matrix begin()
  _char_buf = (uint8_t *)malloc(matrix->getMaxFontWidth() * sizeof(uint8_t));

  matrix->clear();
  matrix->control(MD_MAX72XX::INTENSITY, 0);
}

GraphicsBuffer *const get_char_gfx(MD_MAX72XX *const matrix, uint16_t c) {
  auto char_width = matrix->getChar(c, matrix->getMaxFontWidth(), _char_buf);

  _char_gfx.clear();
  for (uint8_t i = 0; i < char_width; i++) {
    _char_gfx.push_back(_char_buf[i]);
  }
  return &_char_gfx;
}

} // namespace led_matrix
