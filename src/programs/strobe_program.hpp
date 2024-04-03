#pragma once
#include <cstdint>

#include "../fast_led_sk6812.hpp"
#include "led_program.hpp"

/**
 * @brief Implements a strobe effect on an LED array.
 *
 * The StrobeProgram class extends the LEDProgram abstract class to create a
 * strobe lighting effect. This effect is achieved by rapidly toggling the LED
 * array between a specified color (color_1) and another color or off state
 * (color_2). The frequency and duration of the strobe effect can be adjusted
 * using the `value_1` and `value_2` parameters, which determine the duration
 * the LEDs stay on the `color_1` and `color_2` states, respectively.
 */
class StrobeProgram : public LEDProgram {
 public:
  StrobeProgram() = default;

  /**
   * @brief Updates the LED array to produce a strobe effect using the specified
   * colors.
   *
   * This method overrides the virtual `update` method from the LEDProgram base
   * class. It toggles each LED in the array between two colors (`color_1` and
   * `color_2`) at an interval specified by `value_1` and `value_2`. This
   * creates a strobe lighting effect across the entire LED matrix.
   *
   * @param leds Pointer to the array of CRGBW structs, each representing an
   * LED's color.
   * @param color_1 The primary color used for the strobe effect.
   * @param color_2 The secondary color used for the strobe effect, typically
   * black (off) or another color for contrast.
   * @param color_3 Unused parameter in this implementation.
   * @param color_4 Unused parameter in this implementation.
   * @param value_1 Controls the duration (in some unit of time) the LEDs stay
   * on `color_1`. Could represent milliseconds.
   * @param value_2 Controls the duration (in some unit of time) the LEDs stay
   * on `color_2`. Could represent milliseconds.
   */
  void update(CRGBW* leds) override;
};