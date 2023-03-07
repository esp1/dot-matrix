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

/**
 * @brief Returns the current loop delay in milliseconds.
 */
uint16_t loop_delay_msec();

/**
 * @brief Sets the current loop delay in milliseconds.
 */
void set_loop_delay_msec(uint16_t delay_msec);

/**
 * @brief Render function to call within the main loop() function.
 */
void loop(DotMatrixState *const state);

} // namespace render
