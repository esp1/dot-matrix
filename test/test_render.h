#pragma once

#include "render.h"
#include "types.h"

using namespace tint_matrix;

void test_scroll(void) {
    MatrixStateData data;

    render::scroll(&data, SCROLL_LEFT, false);
    TEST_ASSERT_EQUAL(SCROLL_LEFT, data.scroll_dir);
    TEST_ASSERT_EQUAL(false, data.scroll_loop);

    render::scroll(&data, SCROLL_RIGHT, true);
    TEST_ASSERT_EQUAL(SCROLL_RIGHT, data.scroll_dir);
    TEST_ASSERT_EQUAL(true, data.scroll_loop);
}

void test_render(void) {
    RUN_TEST(test_scroll);
}
