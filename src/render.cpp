#include "render.h"
#include "led_matrix.h"

namespace render {

namespace {

void scroll_horizontal(DotMatrixState *const state, bool scroll_left) {
  auto matrix = state->matrix;
  auto display_width = led_matrix::display_width(state->matrix);
  auto graphics_width = state->graphics->size();
  auto graphics = *(state->graphics);

  if (state->column_offset >= (int16_t)(display_width + graphics_width)) {
    // graphics have scrolled off of the display
    // reset the column offset to loop
    state->column_offset = 0;
  } else {
    // shift the display
    matrix->transform(scroll_left ? MD_MAX72XX::TSL : MD_MAX72XX::TSR);

    // bring in data from the graphics buffer to fill the newly vacated column
    // on the edge of the display
    auto display_col = scroll_left ? 0                  // right edge
                                   : display_width - 1; // left edge

    auto graphics_idx =
        scroll_left
            ? state->column_offset                       // index from left
            : graphics_width - 1 - state->column_offset; // index from right

    auto col_value =
        (graphics_idx >= 0) && (graphics_idx < graphics_width)
            ? graphics[graphics_idx] // index within graphics buffer
            : 0; // index outside graphics buffer (blank column)

    led_matrix::set_column(matrix, display_col, col_value);

    // increment the column offset
    state->column_offset++;
  }
}

} // namespace

void clear(DotMatrixState *const state) {
  led_matrix::clear(state->matrix);
  state->column_offset = 0;
  set_scroll_dir(state, SCROLL_NONE);
}

/**
 * @brief Adjusts a Graphics object so that it will be aligned in the specified
 * manner on the display.
 *
 * @param align How the provided graphics should be aligned to the display.
 * @return MatrixStateData* The adjusted matrix state data.
 */
void align(DotMatrixState *const state, Alignment alignment) {
  auto display_width = led_matrix::display_width(state->matrix);
  auto graphics_width = state->graphics->size();

  state->column_offset =
      alignment == ALIGN_LEFT ? 0
      : alignment == ALIGN_RIGHT
          ? display_width - graphics_width
          : 0.5 * (display_width - graphics_width); // ALIGN_CENTER
}

void set_scroll_dir(DotMatrixState *const state, ScrollDirection scroll_dir) {
  // update column offset based on scroll direciton changes
  if (scroll_dir == SCROLL_NONE) {
    // changing to SCROLL_NONE
    if (state->scroll_dir == SCROLL_LEFT) {
      // changing from SCROLL_LEFT
      auto display_width = led_matrix::display_width(state->matrix);
      state->column_offset = display_width - state->column_offset;
    } else if (state->scroll_dir == SCROLL_RIGHT) {
      // changing from SCROLL_RIGHT
      auto graphics_width = state->graphics->size();
      state->column_offset = state->column_offset - graphics_width;
    }
  } else {
    // changing to SCROLL_LEFT or SCROLL_RIGHT
    if (state->scroll_dir == SCROLL_NONE) {
      if (scroll_dir == SCROLL_LEFT) {
        // changing from SCROLL_NONE to SCROLL_LEFT
        auto display_width = led_matrix::display_width(state->matrix);
        state->column_offset = display_width - state->column_offset;
      } else if (scroll_dir == SCROLL_RIGHT) {
        // changing from SCROLL_NONE to SCROLL_RIGHT
        auto graphics_width = state->graphics->size();
        state->column_offset = state->column_offset + graphics_width;
      }
    } else if (((state->scroll_dir == SCROLL_LEFT) &&
                (scroll_dir == SCROLL_RIGHT)) ||
               ((state->scroll_dir == SCROLL_RIGHT) &&
                (scroll_dir == SCROLL_LEFT))) {
      // changing from SCROLL_LEFT to SCROLL_RIGHT or vice versa
      auto display_width = led_matrix::display_width(state->matrix);
      auto graphics_width = state->graphics->size();
      state->column_offset =
          display_width + graphics_width - state->column_offset;
    }
  }

  state->scroll_dir = scroll_dir;
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
    auto char_cols = led_matrix::get_char_gfx(state->matrix, c);
    graphics->insert(graphics->end(), char_cols->begin(), char_cols->end());
  }

  align(state, alignment);
}

void scroll_text(DotMatrixState *const state, String str,
                 ScrollDirection scroll_dir) {
  text(state, str);
  set_scroll_dir(state, scroll_dir);
}

void update_display(DotMatrixState *const state) {
  switch (state->scroll_dir) {
  case SCROLL_LEFT: {
    scroll_horizontal(state, true);
    break;
  }
  case SCROLL_RIGHT: {
    scroll_horizontal(state, false);
    break;
  }
  default: {
    auto display_width = led_matrix::display_width(state->matrix);
    led_matrix::set_buffer(state->matrix,
                           display_width - 1 - state->column_offset,
                           state->graphics);
  }
  }
}

} // namespace render
