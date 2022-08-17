#pragma once

/**
 *                          MAX7219
 *           4 8x8 LED matrix modules = 32x8 matrix
 *
 *          <-OUT                                IN<-                  ESP8266
 *                                                                  Amica NodeMCU
 *        [r0, c31]    24       16       8     [r0, c0]
 *   bit  row +--------+--------+--------+--------+   pin         pin
 *     0   0  |        |        |        |        |  label       label
 *     2   1  |        |        |        |        | - CLK   <->    D5  GPIO14  HSCLK
 *     4   2  |        |        |        |        | - CS    <->    D8  GPIO15  HCS
 *     8   3  |    3   |    2   |    1   |    0   | - DIN   <->    D7  GPIO13  HMOSI
 *    16   4  |        |        |        |        | - GND   <->   GND
 *    32   5  |        |        |        |        | - VCC   <->   3V3
 *    64   6  |        |        |        |        |
 *   128   7  |        |        |        |        |
 *            +--------+--------+--------+--------+
 *        [r7, c31]    24       16       8     [r7, c0]
 */

#include "matrix_state.h"
#include "scroller.h"

#define USE_LOCAL_FONT 1
#include "MD_MAX72xx.h"

namespace tint_matrix
{
    class Matrix
    {
    private:
        const uint8_t _num_modules;
        const uint8_t _display_width;

        // configure MAX7219 using default SPI
        MD_MAX72XX matrix;

        Graphics char_buf;

    public:
        /**
         * @brief Construct a new Matrix object.
         *
         * @param num_modules The number of 8x8 MAX72xx matrix modules in the display.
         */
        Matrix(const uint8_t num_modules)
            : _num_modules(num_modules),
              _display_width(num_modules * 8),
              matrix(
                  MD_MAX72XX::FC16_HW,
                  15,         // CS
                  num_modules // # of devices
              )
        {
            matrix.begin();
            matrix.clear();

            // set to minimum brightness
            matrix.control(MD_MAX72XX::INTENSITY, 0);

            char_buf.reserve(matrix.getMaxFontWidth());
        };

        const uint8_t display_width() { return _display_width; }

        /**
         * @brief Get graphics data for a character.
         * Function to look up character graphics from the font info in the matrix library.
         *
         * @param c The character to look up graphics for.
         * @return Graphics Graphics data for the given character.
         */
        Graphics char_gfx(const char c)
        {
            Graphics g;
            auto char_width = matrix.getChar(c, char_buf.capacity(), char_buf.data());
            for (auto i = 0; i < char_width; i++)
            {
                g.push_back(char_buf[i]);
            }
            return g;
        }

        /**
         * @brief Update the display.
         * Updates the matrix display based on the matrix state.
         *
         * @param state The matrix state.
         */
        MatrixState *update(MatrixState *const state)
        {
            static const auto f_set_column = [&](uint16_t col, uint8_t value)
            {
                matrix.setColumn(col, value);
            };

            static const auto f_set_row = [&](uint8_t module_idx, uint8_t row, uint8_t value)
            {
                matrix.setRow(module_idx, module_idx, row, value);
            };

            static const auto f_transform = [&](MD_MAX72XX::transformType_t ttype)
            {
                matrix.wraparound(MD_MAX72XX::OFF);
                matrix.transform(ttype);
            };

            auto g = state->data->graphics;

            if (g.empty())
            {
                matrix.clear();
            }
            else
            {
                switch (state->data->scroll_dir)
                {
                case SCROLL_NONE:
                {
                    matrix.setBuffer(_display_width - 1 - state->data->column_offset, g.size(), g.data());
                    break;
                }
                case SCROLL_LEFT:
                {
                    state->data = scroller::hw_scroll_horizontal(state->data, _display_width, true, f_set_column, std::bind(f_transform, MD_MAX72XX::TSL));
                    break;
                }
                case SCROLL_RIGHT:
                {
                    state->data = scroller::hw_scroll_horizontal(state->data, _display_width, false, f_set_column, std::bind(f_transform, MD_MAX72XX::TSR));
                    break;
                }
                case SCROLL_UP:
                {
                    state->data = scroller::hw_scroll_vertical(state->data, _num_modules, true, f_set_row, std::bind(f_transform, MD_MAX72XX::TSU));
                    break;
                }
                case SCROLL_DOWN:
                {
                    state->data = scroller::hw_scroll_vertical(state->data, _num_modules, false, f_set_row, std::bind(f_transform, MD_MAX72XX::TSD));
                    break;
                }
                }
            }

            return state;
        }
    };
}
