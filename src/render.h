#pragma once

#include "types.h"
#include <Arduino.h>

namespace render {

void clear(DotMatrixState *const state);
void align(DotMatrixState *const state, Alignment alignment);
void set_scroll_dir(DotMatrixState *const state, ScrollDirection scroll_dir);
void text(DotMatrixState *const state, String str,
          Alignment alignment = ALIGN_LEFT);
void scroll_text(DotMatrixState *const state, String text,
                 ScrollDirection scroll_dir = SCROLL_LEFT);
void update_display(DotMatrixState *const state);

} // namespace render
