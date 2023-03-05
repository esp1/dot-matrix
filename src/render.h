#pragma once

#include "types.h"
#include <Arduino.h>

namespace render {

void clear(MatrixState *const state);
void align(MatrixState *const state, Alignment alignment);
void set_scroll_dir(MatrixState *const state, ScrollDirection scroll_dir);
void text(MatrixState *const state, String str,
          Alignment alignment = ALIGN_LEFT);
void scroll_text(MatrixState *const state, String text,
                 ScrollDirection scroll_dir = SCROLL_LEFT);
void update_display(MatrixState *const state);

} // namespace render
