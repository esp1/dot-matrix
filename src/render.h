#pragma once

#include "types.h"
#include <Arduino.h>

namespace render {

/**
 * @brief Clears the state and resets it to initial values.
 */
void clear(DotMatrixState *const state);

/**
 * @brief Updates the column offset in the state according to the requested
 * graphics alignment.
 */
void align(DotMatrixState *const state, Alignment alignment);

/**
 * @brief Sets the scrolling direction, updating the column offset in the state
 * appropriately if the state direction changes.
 */
void set_scroll_dir(DotMatrixState *const state, ScrollDirection scroll_dir);

/**
 * @brief Converts the string into graphics data and puts it in the state's
 * graphics buffer, and sets the column offset according to the alignment.
 */
void text(DotMatrixState *const state, String str,
          Alignment alignment = ALIGN_LEFT);

/**
 * @brief Converts the string into graphics data and puts it in the state's
 * graphics buffer, and sets the scroll direction.
 */
void scroll_text(DotMatrixState *const state, String str,
                 ScrollDirection scroll_dir = SCROLL_LEFT);

/**
 * @brief Updates the LED display from values the state.
 */
void update_display(DotMatrixState *const state);

} // namespace render
