#pragma once

#include "types.h"

namespace tint_matrix::scroller
{
    /**
     * @brief Scroll the display horizontally using hardware scrolling.
     * Hardware scrolling renders data into the edge of the display and uses matrix transform shift instructions to scroll.
     * 
     * @param data The matrix state data to render.
     * @param display_width The width of the matrix display in pixels.
     * @param reverse If true, scroll to the left. If false, scroll to the right.
     * @param f_set_column A function to set column data on the display.
     * @param f_shift_display A function to shift the contents of the display horizontally.
     * @return MatrixStateData* The updated matrix state data after scrolling.
     */
    MatrixStateData *hw_scroll_horizontal(
        MatrixStateData *const data,
        const uint8_t display_width,
        const bool reverse,
        const std::function<void(uint16_t col, uint8_t value)> f_set_column,
        const std::function<void()> f_shift_display)
    {
        auto g = data->graphics;

        if (data->column_offset >= display_width + g.size() && data->scroll_loop)
        {
            data->column_offset = 0;
        }

        if (data->column_offset < display_width + g.size())
        {
            f_shift_display();

            f_set_column(reverse ? 0 : display_width - 1,
                         data->column_offset < g.size()
                             ? g[reverse
                                     ? data->column_offset
                                     : g.size() - 1 - data->column_offset]
                             : 0);
            data->column_offset++;
        }

        return data;
    }

    /**
     * @brief Scroll the display vertically using hardware scrolling.
     * Hardware scrolling renders data into the edge of the display and uses matrix transform shift instructions to scroll.
     * 
     * @param data The matrix state data to render.
     * @param num_modules The number of 8x8 matrix modules in the display.
     * @param reverse If true, scroll up. If false, scroll down.
     * @param f_set_row A function to set row data on the display.
     * @param f_shift_display A function to shift the contents of the display vertically.
     * @return MatrixStateData* The updated matrix state data after scrolling.
     */
    MatrixStateData *hw_scroll_vertical(
        MatrixStateData *const data,
        const uint8_t num_modules,
        const bool reverse,
        const std::function<void(uint8_t module_idx, uint8_t row, uint8_t value)> f_set_row,
        const std::function<void()> f_shift_display)
    {
        auto g = data->graphics;

        if (data->column_offset >= 8 + 8 && data->scroll_loop)
        {
            data->column_offset = 0;
        }

        if (data->column_offset < 8 + 8)
        {
            f_shift_display();

            for (auto module_idx = num_modules - 1; module_idx >= 0; module_idx--)
            {
                uint8_t section = num_modules - 1 - module_idx;
                uint8_t row_data = 0;

                for (auto module_col = 7; module_col >= 0; module_col--)
                {
                    // translate to graphics coordinates (origin on top left)
                    uint8_t x = (section * 8) + (7 - module_col) - data->column_offset;

                    if (x >= 0 && x < g.size())
                    {
                        // get y value data bit at scroll offset position
                        uint8_t val_bitshift = reverse ? data->column_offset : 7 - data->column_offset;
                        uint8_t val_bitmask = 0b1 << val_bitshift;
                        uint8_t val_bit = (g[x] & val_bitmask) >> val_bitshift;

                        // add bit value into row data at matrix module column position
                        row_data += val_bit << module_col;
                    }
                }

                f_set_row(module_idx,
                          reverse ? 7 : 0, // row
                          row_data         // value
                );
            }
            data->column_offset++;
        }

        return data;
    }
}
