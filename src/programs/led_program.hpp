#pragma once
#include <cstdint>

#include "fast_led_sk6812.hpp"

class LEDProgram {
 public:
  virtual ~LEDProgram() = default;
  virtual void initialize(int matrix_width, int matrix_height) {
    this->matrix_width = matrix_width;
    this->matrix_height = matrix_height;
  }
  virtual void update(CRGBW* leds, CRGBW* color_1, CRGBW* color_2,
                      CRGBW* color_3, CRGBW* color_4, uint8_t value_1,
                      uint8_t value_2) = 0;

 protected:
  int matrix_width;
  int matrix_height;
};