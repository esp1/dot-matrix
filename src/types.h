#pragma once

#include "MD_MAX72xx.h"

/**
 * @brief Data structure that holds graphics data.
 * A std::vector of uint8_t values where each value is a bit representation of a
 * column of pixels. The first value in the vector represents the leftmost
 * column of graphics data. The least significant bit of each column value is
 * the top pixel, and the most significant bit is the bottom pixel.
 */
typedef std::vector<uint8_t> GraphicsBuffer;

/**
 * @brief The graphics alignment.
 * Indicates how graphics content should be aligned on the display.
 */
enum Alignment { ALIGN_CENTER, ALIGN_LEFT, ALIGN_RIGHT };

enum ScrollDirection {
  SCROLL_NONE,
  SCROLL_LEFT,
  SCROLL_RIGHT,
};

typedef struct MatrixState {

  MatrixState(MD_MAX72XX *const matrix);

  /**
   * @brief The physical LED matrix display.
   */
  MD_MAX72XX *matrix;

  /**
   * @brief The graphics data to be rendered.
   */
  GraphicsBuffer *graphics;

  /**
   * @brief The scroll direction.
   * The direction the matrix display should scroll.
   */
  ScrollDirection scroll_dir = SCROLL_NONE;

  /**
   * @brief The column offset.
   * The number of pixels that the edge of the display buffer is from the edge
   * of the display matrix. A zero column offset indicates the opposite edge
   * from the scroll direction, with positive values oriented in the scroll
   * direction. This means that for example when the scroll direction is LEFT,
   * positive column offset values indicate the distance to the left of the
   * right edge of the display.
   */
  uint16_t column_offset = 0;

} MatrixState;