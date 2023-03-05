#include "led_matrix.h"

namespace led_matrix {

namespace {

/**
 * @brief C array used to store character graphics data from
 * MD_MAX72XX::getChar() function.
 */
uint8_t *char_buf;

/**
 * @brief std::vector used to return character graphics data.
 */
GraphicsBuffer char_gfx;

} // namespace

// setup function

void setup(MD_MAX72XX *const matrix) {
  matrix->begin();

  // getMaxFontWidth() has to be called after matrix begin()
  char_buf = (uint8_t *)malloc(matrix->getMaxFontWidth() * sizeof(uint8_t));

  clear(matrix);
  set_brightness(matrix, 0);
}

// inspect functions

uint8_t num_modules(MD_MAX72XX *const matrix) {
  return matrix->getDeviceCount();
}

uint16_t display_width(MD_MAX72XX *const matrix) {
  return matrix->getColumnCount();
}

GraphicsBuffer *get_char_gfx(MD_MAX72XX *const matrix, uint16_t c) {
  auto char_width = matrix->getChar(c, matrix->getMaxFontWidth(), char_buf);

  char_gfx.clear();
  for (uint8_t i = 0; i < char_width; i++) {
    char_gfx.push_back(char_buf[i]);
  }
  return &char_gfx;
}

// control functions

void clear(MD_MAX72XX *matrix) { matrix->clear(); }

void set_brightness(MD_MAX72XX *const matrix, int intensity) {
  matrix->control(MD_MAX72XX::INTENSITY, intensity);
}

void set_column(MD_MAX72XX *const matrix, uint16_t column, uint8_t value) {
  matrix->setColumn(column, value);
}

void set_buffer(MD_MAX72XX *const matrix, uint16_t column_offset,
                GraphicsBuffer *const graphics) {
  matrix->setBuffer(column_offset, graphics->size(), graphics->data());
}

} // namespace led_matrix
