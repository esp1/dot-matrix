#include "render.h"
#include "led_matrix.h"

namespace render {

namespace {

void scroll_horizontal(MatrixState *const state, bool scroll_left) {
  auto matrix = state->matrix;
  auto display_width = led_matrix::display_width(state->matrix);
  auto graphics_width = state->graphics->size();
  auto graphics = *(state->graphics);

  if (state->column_offset >= display_width + graphics_width) {
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
        state->column_offset < graphics.size()
            ? graphics[graphics_idx] // index within graphics buffer
            : 0; // index outside graphics buffer (blank column)

    led_matrix::set_column(matrix, display_col, col_value);

    // increment the column offset
    state->column_offset++;
  }
}

} // namespace

void clear(MatrixState *const state) {
  led_matrix::clear(state->matrix);
  state->column_offset = 0;
  state->scroll_dir = SCROLL_NONE;
}

/**
 * @brief Adjusts a Graphics object so that it will be aligned in the specified
 * manner on the display.
 *
 * @param align How the provided graphics should be aligned to the display.
 * @return MatrixStateData* The adjusted matrix state data.
 */
void align(MatrixState *const state, Alignment alignment) {
  auto display_width = led_matrix::display_width(state->matrix);
  auto graphics_width = state->graphics->size();

  state->column_offset =
      alignment == ALIGN_LEFT ? 0
      : alignment == ALIGN_RIGHT
          ? display_width - graphics_width
          : 0.5 * (display_width - graphics_width); // ALIGN_CENTER
}

void text(MatrixState *const state, String str, Alignment alignment) {
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

void scroll_text(MatrixState *const state, String str,
                 ScrollDirection scroll_dir) {
  text(state, str);
  state->scroll_dir = scroll_dir;
}

void update_display(MatrixState *const state) {
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
    led_matrix::set_buffer(state->matrix, state->column_offset,
                           state->graphics);
  }
  }
}

} // namespace render
