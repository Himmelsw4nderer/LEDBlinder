#include "all_leds_on_program.hpp"

void AllLEDsOnProgram::update(CRGBW* leds) {
  for (int i = 0; i < this->matrix_width * this->matrix_height; i++) {
    leds[i] = this->data->color_1;
  }
}