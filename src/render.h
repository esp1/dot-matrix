#pragma once

#include "types.h"

namespace tint_matrix::render
{
    /**
     * @brief Append text.
     * Appends text into the matrix state data.
     * 
     * @param txt The text to append.
     * @param f_char_gbuf A function to retrieve the graphics buffer for a character.
     * @return MatrixStateData* The updated matrix state data containing the appended text.
     */
    MatrixStateData *append_text(MatrixStateData *const data, const char *txt, GetCharGBufFn f_char_gbuf)
    {
        for (size_t i = 0; i < strlen(txt); i++)
        {
            // add single column space if this is not the first character
            if (i > 0)
            {
                data->graphics.push_back(0);
            }

            std::vector char_cols = f_char_gbuf(txt[i]);
            data->graphics.insert(data->graphics.end(), std::move_iterator(char_cols.begin()), std::move_iterator(char_cols.end()));
        }

        return data;
    }

    /**
     * @brief Adjusts a Graphics object so that it will be aligned in the specified manner on the display.
     * 
     * @param align How the provided graphics should be aligned to the display.
     * @param display_width The width of the matrix display in pixels.
     * @return MatrixStateData* The adjusted matrix state data.
     */
    MatrixStateData *align(MatrixStateData *const data, const Alignment align, const uint8_t display_width)
    {
        data->column_offset = align == ALIGN_LEFT
                                  ? 0
                              : align == ALIGN_RIGHT
                                  ? display_width - data->graphics.size()
                                  : 0.5 * (display_width - data->graphics.size()); // ALIGN_CENTER
        return data;
    }

    /**
     * @brief Set scrolling behavior.
     * Set the scroll direction (default SCROLL_LEFT) and whether scrolling should loop (default true).
     * To turn off scrolling, set scroll direction to SCROLL_NONE.
     * 
     * @param dir The scroll direction.
     * @param loop Whether scrolling should loop.
     * @return MatrixStateData* The matrix state data with scrolling behavior.
     */
    MatrixStateData *scroll(MatrixStateData *const data, ScrollDirection dir, bool loop)
    {
        data->scroll_dir = dir;
        data->scroll_loop = loop;
        return data;
    }

    /**
     * @brief Reset column offset.
     * 
     * @return MatrixStateData* The updated matrix state data.
     */
    MatrixStateData *reset_column_offset(MatrixStateData *const data)
    {
        data->column_offset = 0;
        return data;
    }

    /**
     * @brief Clear the display.
     * Clears the display without resetting scrolling.
     * 
     * @return MatrixStateData*
     */
    MatrixStateData *clear(MatrixStateData *const data)
    {
        data->graphics.clear();
        return data;
    }

}
