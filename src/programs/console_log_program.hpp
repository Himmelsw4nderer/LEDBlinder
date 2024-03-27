#pragma once
#include <cstdint>

#include "../fast_led_sk6812.hpp"
#include "led_program.hpp"

/**
 * @brief A diagnostic LED program that logs color and value parameters to the
 * console.
 *
 * This class extends LEDProgram to provide a utility for logging the parameters
 * passed to the `update` method. It is intended for debugging and does not
 * manipulate LED colors. Each call to `update` results in the parameters being
 * printed to the Serial monitor, facilitating inspection of program behavior.
 */
class ConsoleLogProgram : public LEDProgram {
 public:
  ConsoleLogProgram() = default;

  /**
   * @brief Logs the provided color and value parameters to the console (Serial
   * monitor).
   *
   * Overrides the `update` method from LEDProgram. Instead of updating LED
   * colors, this method prints the details of all parameters to the Serial
   * monitor. It's useful for debugging and understanding the data flow through
   * LED programs.
   *
   * @param leds Ignored in this implementation. Included for interface
   * compatibility.
   * @param color_1 Primary color, logged to the console.
   * @param color_2 Secondary color, logged to the console.
   * @param color_3 Tertiary color, logged to the console.
   * @param color_4 Quaternary color, logged to the console.
   * @param value_1 A generic value, logged to the console.
   * @param value_2 Another generic value, logged to the console.
   */
  void update(CRGBW* leds, CRGBW* color_1, CRGBW* color_2, CRGBW* color_3,
              CRGBW* color_4, uint8_t value_1, uint8_t value_2) override;
};