#include "all_leds_on_program.hpp"

void AllLEDsOnProgram::update(CRGBW* leds, CRGBW* color_1, CRGBW* color_2,
                              CRGBW* color_3, CRGBW* color_4, uint8_t value_1,
                              uint8_t value_2) {
  for (int i = 0; i < this->matrix_width * this->matrix_height; i++) {
    leds[i] = *color_1;
  }
}