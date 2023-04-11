#include "render.h"
#include "led_matrix.h"

namespace {

/**
 * @brief Number of milliseconds to delay in the main loop.
 * Adjusting this to a lower value will increase the refresh rate and
 * scrolling speed of the display, and vice versa.
 */
uint16_t _loop_delay_msec = 100;

/**
 * @brief Timestamp in milliseconds of the previous display update.
 */
unsigned long _prev_update_msec = 0;

void _scroll_horizontal(DotMatrixState *const state, bool scroll_left) {
  auto matrix = state->matrix;
  int16_t display_width = state->matrix->getColumnCount();
  int16_t graphics_width = state->graphics->size();
  auto graphics = *(state->graphics);

  if (state->column_offset >= display_width) {
    // graphics have scrolled off the right side of the display
    // wrap graphics around to the left
    state->column_offset = -graphics_width + 1;
  } else if (state->column_offset < -graphics_width) {
    // graphics have scrolled off the left side of the display
    // wrap graphics around to the right
    state->column_offset = display_width - 1;
  } else {
    // increment the column offset
    state->column_offset += scroll_left ? -1 : 1;
  }

  // shift the display
  matrix->transform(scroll_left ? MD_MAX72XX::TSL : MD_MAX72XX::TSR);

  // bring in data from the graphics buffer to fill the newly vacated column
  // on the edge of the display
  auto display_col = scroll_left ? 0                  // right edge of display
                                 : display_width - 1; // left edge of display

  auto graphics_idx =
      scroll_left
          ? display_width - 1 -
                state->column_offset // index from left edge of graphics
                                     // buffer at right edge of display
          : -(state->column_offset); // index from right edge of graphics
                                     // buffer at left edge of display

  auto col_value = (graphics_idx >= 0) && (graphics_idx < graphics_width)
                       ? graphics[graphics_idx] // index within graphics buffer
                       : 0; // index outside graphics buffer (blank column)

  matrix->setColumn(display_col, col_value);
}

} // namespace

namespace render {

void clear(DotMatrixState *const state) {
  state->matrix->clear();
  state->column_offset = 0;
  state->scroll_dir = SCROLL_NONE;
}

void align(DotMatrixState *const state, Alignment alignment) {
  int16_t display_width = state->matrix->getColumnCount();
  int16_t graphics_width = state->graphics->size();

  state->column_offset =
      alignment == ALIGN_LEFT ? 0 // ALIGN_LEFT
      : alignment == ALIGN_RIGHT
          ? display_width - graphics_width          // ALIGN_RIGHT
          : 0.5 * (display_width - graphics_width); // ALIGN_CENTER
}

void text(DotMatrixState *const state, String str, Alignment alignment) {
  clear(state);

  auto graphics = state->graphics;

  graphics->clear();

  for (unsigned int i = 0; i < str.length(); i++) {
    // add single column space if this is not the first character
    if (i > 0) {
      graphics->push_back(0);
    }

    auto c = str.charAt(i);
    auto char_cols = led_matrix::char_gfx(state->matrix, c);
    graphics->insert(graphics->end(), char_cols->begin(), char_cols->end());
  }

  align(state, alignment);
}

void scroll_text(DotMatrixState *const state, String str,
                 ScrollDirection scroll_dir) {
  text(state, str);

  if (scroll_dir == SCROLL_LEFT) {
    // position graphics buffer just outside the right edge of display
    int16_t display_width = state->matrix->getColumnCount();
    state->column_offset = display_width;
  } else if (scroll_dir == SCROLL_RIGHT) {
    // position graphics buffer just outside the left edge of the display
    int16_t graphics_width = state->graphics->size();
    state->column_offset = -graphics_width;
  }

  state->scroll_dir = scroll_dir;
}

void update_display(DotMatrixState *const state) {
  switch (state->scroll_dir) {
  case SCROLL_LEFT: {
    _scroll_horizontal(state, true);
    break;
  }
  case SCROLL_RIGHT: {
    _scroll_horizontal(state, false);
    break;
  }
  case SCROLL_NONE: {
    int16_t display_width = state->matrix->getColumnCount();
    auto led_column = constrain(display_width - 1 - state->column_offset, 0,
                                display_width - 1);

    // trim any negative (offscreen) portion of graphics buffer
    auto graphics_offset = state->column_offset < 0 ? -state->column_offset : 0;
    auto buf_size = state->graphics->size() - graphics_offset;
    auto buf = state->graphics->data() + graphics_offset;

    state->matrix->setBuffer(led_column, buf_size, buf);
  }
  }
}

uint16_t loop_delay_msec() { return _loop_delay_msec; }

void set_loop_delay_msec(uint16_t delay_msec) { _loop_delay_msec = delay_msec; }

void loop(DotMatrixState *const state) {
  auto current_msec = millis();

  if (current_msec - _prev_update_msec > _loop_delay_msec) {
    update_display(state);
    _prev_update_msec = current_msec;
  }
}

} // namespace render
