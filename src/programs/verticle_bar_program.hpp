#pragma once

#include <cstdint>

#include "../fast_led_sk6812.hpp"
#include "led_program.hpp"

/**
 * @brief Displays a vertical bar moving back and forth across the LED matrix.
 *
 * This program creates a moving vertical bar effect where the bar's color is
 * `color_1`. The position of the bar is determined by `value_1`, which
 * represents the progress of the cycle from left to right and back.
 */
class VerticalBarProgram : public LEDProgram {
 public:
  VerticalBarProgram() = default;

  /**
   * Updates the LED matrix to display the moving vertical bar effect.
   *
   * Overrides the abstract update method from LEDProgram. It calculates the
   * current position of the vertical bar based on `value_1` and updates the
   * LEDs in the matrix to reflect this position, using `color_1` for the bar's
   * color. The bar moves in a back-and-forth pattern across the matrix.
   *
   * @param leds Pointer to the array of CRGBW structs, each representing an
   * LED's color.
   * @param color_1 The color used for the vertical bar. This is the primary
   * color for the effect.
   * @param color_2 Unused in this implementation, but included for interface
   * consistency.
   * @param color_3 Unused in this implementation, but included for interface
   * consistency.
   * @param color_4 Unused in this implementation, but included for interface
   * consistency.
   * @param value_1 Controls the progress and direction of the moving bar.
   * Typically, this value is cyclically incremented to animate the bar across
   * the display.
   * @param value_2 Unused in this implementation, but included for potential
   * future enhancements.
   */
  void update(CRGBW* leds, CRGBW* color_1, CRGBW* color_2, CRGBW* color_3,
              CRGBW* color_4, uint8_t value_1, uint8_t value_2) override;
};
