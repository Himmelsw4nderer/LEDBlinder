#pragma once
#include <cstdint>

#include "../fast_led_sk6812.hpp"
#include "led_program.hpp"

/**
 * @brief A specific LED program that sets all LEDs to a single color.
 *
 * This class implements the LEDProgram interface to create a lighting effect
 * where all LEDs in the matrix are set to the same color, using color_1 as
 * the chosen color. Other color parameters and value parameters are ignored.
 */
class AllLEDsOnProgram : public LEDProgram {
 public:
  AllLEDsOnProgram() = default;

  /**
   * @brief Updates the LED colors, setting all to the same specified color
   * (color_1).
   *
   * Implements the abstract update method from LEDProgram to fill the entire
   * LED matrix with a single color. This provides a uniform color across all
   * LEDs.
   *
   * @param leds Pointer to the array of CRGBW structs representing each LED's
   * color.
   * @param color_1 The color to set all LEDs to. This is the primary color used
   * by this program.
   * @param color_2 Ignored in this implementation.
   * @param color_3 Ignored in this implementation.
   * @param color_4 Ignored in this implementation.
   * @param value_1 A generic value that can be used by the program for effect
   * tuning. Ignored in this implementation.
   * @param value_2 Another generic value for additional effect tuning or
   * control. Ignored in this implementation.
   */
  void update(CRGBW* leds, CRGBW* color_1, CRGBW* color_2, CRGBW* color_3,
              CRGBW* color_4, uint8_t value_1, uint8_t value_2) override;
};