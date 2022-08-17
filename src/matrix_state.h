#pragma once

#include <Arduino.h>
#include "render.h"
#include "types.h"

namespace tint_matrix
{
    /**
     * @brief The matrix state.
     * This is a thin shell around MatrixStateData.
     * It binds render functions to minimal display state (display width and font lookup function)
     * to make calling rendering functions more convenient,
     * while keeping the rendering functions themselves pure.
     */
    class MatrixState
    {
    private:
        const uint8_t display_width;
        const GetCharGBufFn f_char_gbuf;

    public:
        MatrixStateData *data;

        MatrixState(uint8_t display_width, GetCharGBufFn f_char_gbuf) : display_width(display_width), f_char_gbuf(f_char_gbuf)
        {
            data = new MatrixStateData();
        }

        ~MatrixState()
        {
            delete data;
        }

        /**
         * @brief Append text.
         * Appends text into the matrix state.
         * 
         * @param txt The text to append.
         * @param f_char_gbuf A function to retrieve the graphics buffer for a character.
         * @return MatrixState* The updated matrix state containing the appended text.
         */
        MatrixState *append_text(const char *txt)
        {
            data = render::append_text(data, txt, f_char_gbuf);
            return this;
        }

        /**
         * @brief Render text.
         * Renders text into the matrix state, replacing any existing graphics.
         * 
         * @param txt The text to render.
         * @param f_char_gbuf A function to retrieve the graphics buffer for a character.
         * @return MatrixState* The updated matrix state containing the rendered text.
         */
        MatrixState *text(const char *txt)
        {
            clear();
            reset_column_offset();
            return append_text(txt);
        }

        /**
         * @brief Adjusts a Graphics object so that it will be aligned in the specified manner on the display.
         * 
         * @param align How the provided graphics should be aligned to the display.
         * @return MatrixState* The adjusted matrix state.
         */
        MatrixState *align(const Alignment align)
        {
            data = render::align(data, align, display_width);
            return this;
        }

        /**
         * @brief Set scrolling behavior.
         * Set the scroll direction (default SCROLL_LEFT) and whether scrolling should loop (default true).
         * To turn off scrolling, set scroll direction to SCROLL_NONE.
         * 
         * @param dir The scroll direction.
         * @param loop Whether scrolling should loop.
         * @return MatrixState* The matrix state with scrolling behavior.
         */
        MatrixState *scroll(ScrollDirection dir = SCROLL_LEFT, bool loop = true)
        {
            data = render::scroll(data, dir, loop);
            return this;
        }

        /**
         * @brief Reset column offset.
         * 
         * @return MatrixState* The updated matrix state.
         */
        MatrixState *reset_column_offset()
        {
            data = render::reset_column_offset(data);
            return this;
        }

        /**
         * @brief Clear the display.
         * Clears the display without resetting scrolling.
         * 
         * @return MatrixState* 
         */
        MatrixState *clear()
        {
            data = render::clear(data);
            return this;
        }
        
    };
}
