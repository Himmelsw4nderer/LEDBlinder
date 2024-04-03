#pragma once
#include <cstdint>

#include "fast_led_sk6812.hpp"
#include "led_data.hpp"

/**
 * @brief Abstract base class representing a generic LED program.
 *
 * This class provides a framework for defining LED lighting programs. Each
 * program is expected to initialize with a specific matrix size and update LED
 * colors based on program logic. It uses CRGBW structs for color
 * representation, supporting RGBW LEDs.
 */
class LEDProgram {
 public:
  virtual ~LEDProgram() = default;

  /**
   * @brief Initializes the LED program with the dimensions of the LED matrix.
   *
   * This method sets the internal dimensions of the LED matrix, which can be
   * used by the program to determine layout and indexing of the LEDs.
   *
   * @param matrix_width The width (number of columns) of the LED matrix.
   * @param matrix_height The height (number of rows) of the LED matrix.
   */
  virtual void initialize(int matrix_width, int matrix_height) {
    this->matrix_width = matrix_width;
    this->matrix_height = matrix_height;
  }

  /**
   * @brief Updates the LED matrix based on program logic.
   *
   * Derived classes must implement this method to define the program's
   * behavior. This typically involves setting LED colors based on the program's
   * state.
   *
   * @param leds Pointer to the array of CRGBW structs representing each LED's
   * color.
   */
  virtual void update(CRGBW* leds) = 0;

  /**
   * @brief Updates the LED matrix based on program logic.
   *
   * Derived classes must implement this method to define the program's
   * behavior. This typically involves setting LED colors based on the program's
   * state.
   *
   * @param leds Pointer to the array of CRGBW structs representing each LED's
   * color.
   * @param data The data containing colors and values
   */
  void set(CRGBW* leds, LedData* data) {
    this->data = data;
    this->update(leds);
  };

 protected:
  int matrix_width;
  int matrix_height;
  LedData* data;
};