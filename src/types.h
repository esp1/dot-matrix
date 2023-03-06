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

typedef struct DotMatrixState {

  DotMatrixState(MD_MAX72XX *const matrix);

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
   * of the display matrix. The way the column offset is interpreted depends on
   * the scroll direction:
   *
   * If the scroll direction is SCROLL_NONE (i.e. when there is no scrolling),
   * then the column offset is the number of pixels from the LEFT edge of the
   * display to the LEFT edge of the graphics buffer, with positive column
   * offset values going to the RIGHT.
   *
   * If the display is set to SCROLL_LEFT or SCROLL_RIGHT, the column offset
   * will indicate the distance from the edge of the display where graphics
   * enter, to the edge of the graphics buffer that first enters the display,
   * with positive column offset values oriented in the scroll direction:
   *
   * If the scroll direction is SCROLL_LEFT, then the column offset is the
   * number of pixels from the RIGHT edge of the display to the LEFT edge of the
   * graphics buffer, with positive column offset values going LEFT.
   *
   * If the scroll direction is SCROLL_RIGHT, then the column offset is the
   * number of pixels from the LEFT edge of the display to the RIGHT edge of the
   * graphics buffer, with positive column offset values going RIGHT.
   */
  int16_t column_offset = 0;

  /**
   * @brief The brightness of the LED display.
   * Brightness values range from 0 to 15 (MD_MAX72XX::MAX_INTENSITY).
   */
  uint8_t brightness = 0;

} MatrixState;