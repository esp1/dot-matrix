#pragma once

#include "scroller.h"
#include "types.h"

using namespace tint_matrix;

void test_hw_scroll_left(void) {
    MatrixStateData data;

    // 1 2 3
    data.graphics.push_back(1);
    data.graphics.push_back(2);
    data.graphics.push_back(3);
    data.scroll_loop = true;
    data.column_offset = 0;
    auto shift_count = 0;
    auto column = -1;
    auto value = -1;

    auto f_set_col = [&](auto c, auto v) { column = c; value = v; };
    auto f_shift_display = [&]() { shift_count++; };

    // Start shifting data in from the right edge of the display
    scroller::hw_scroll_horizontal(&data, 3, true, f_set_col, f_shift_display);
    TEST_ASSERT_EQUAL_MESSAGE(1, shift_count, "iteration 0 shift count");
    TEST_ASSERT_EQUAL_MESSAGE(0, column, "iteration 0 column");
    TEST_ASSERT_EQUAL_MESSAGE(1, value, "iteration 0 value");
    TEST_ASSERT_EQUAL_MESSAGE(1, data.column_offset, "scroll offset 0 column offset");

    scroller::hw_scroll_horizontal(&data, 3, true, f_set_col, f_shift_display);
    TEST_ASSERT_EQUAL_MESSAGE(2, shift_count, "iteration 1 shift count");
    TEST_ASSERT_EQUAL_MESSAGE(0, column, "iteration 1 column");
    TEST_ASSERT_EQUAL_MESSAGE(2, value, "iteration 1 value");
    TEST_ASSERT_EQUAL_MESSAGE(2, data.column_offset, "iteration 1 column offset");

    scroller::hw_scroll_horizontal(&data, 3, true, f_set_col, f_shift_display);
    TEST_ASSERT_EQUAL_MESSAGE(3, shift_count, "iteration 2 shift count");
    TEST_ASSERT_EQUAL_MESSAGE(0, column, "iteration 2 column");
    TEST_ASSERT_EQUAL_MESSAGE(3, value, "iteration 2 value");
    TEST_ASSERT_EQUAL_MESSAGE(3, data.column_offset, "iteration 2 column offset");
    // Data is fully displayed now
    // No more data, start shifting in blank columns
    scroller::hw_scroll_horizontal(&data, 3, true, f_set_col, f_shift_display);
    TEST_ASSERT_EQUAL_MESSAGE(4, shift_count, "iteration 3 shift count");
    TEST_ASSERT_EQUAL_MESSAGE(0, column, "iteration 3 column");
    TEST_ASSERT_EQUAL_MESSAGE(0, value, "iteration 3 value");
    TEST_ASSERT_EQUAL_MESSAGE(4, data.column_offset, "iteration 3 column offset");

    scroller::hw_scroll_horizontal(&data, 3, true, f_set_col, f_shift_display);
    TEST_ASSERT_EQUAL_MESSAGE(5, shift_count, "iteration 4 shift count");
    TEST_ASSERT_EQUAL_MESSAGE(0, column, "iteration 4 column");
    TEST_ASSERT_EQUAL_MESSAGE(0, value, "iteration 4 value");
    TEST_ASSERT_EQUAL_MESSAGE(5, data.column_offset, "iteration 4 column offset");

    scroller::hw_scroll_horizontal(&data, 3, true, f_set_col, f_shift_display);
    TEST_ASSERT_EQUAL_MESSAGE(6, shift_count, "iteration 5 shift count");
    TEST_ASSERT_EQUAL_MESSAGE(0, column, "iteration 5 column");
    TEST_ASSERT_EQUAL_MESSAGE(0, value, "iteration 5 value");
    TEST_ASSERT_EQUAL_MESSAGE(6, data.column_offset, "iteration 5 column offset");
    // Display is blank now
    // Loop = true, so start shifting in data again
    scroller::hw_scroll_horizontal(&data, 3, true, f_set_col, f_shift_display);
    TEST_ASSERT_EQUAL_MESSAGE(7, shift_count, "iteration 6 shift count");
    TEST_ASSERT_EQUAL_MESSAGE(0, column, "iteration 6 column");
    TEST_ASSERT_EQUAL_MESSAGE(1, value, "iteration 6 value");
    TEST_ASSERT_EQUAL_MESSAGE(1, data.column_offset, "iteration 6 column offset");
}

void test_scroller(void) {
    RUN_TEST(test_hw_scroll_left);
}
