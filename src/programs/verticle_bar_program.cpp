#include "verticle_bar_program.hpp"

void VerticalBarProgram::update(CRGBW* leds) {
  for (int i = 0; i < matrix_width * matrix_height; ++i) {
    leds[i] = CRGBW{0, 0, 0, 0};
  }
  int totalCycleLength = matrix_width * 2 - 2;
  int cyclePosition = this->data->value_1 * totalCycleLength / 255;
  int columnPosition = cyclePosition < matrix_width
                           ? cyclePosition
                           : totalCycleLength - cyclePosition;

  for (int row = 0; row < matrix_height; ++row) {
    int index;
    if (row % 2 == 0) {
      index = row * matrix_width + columnPosition;
    } else {
      index = row * matrix_width + (matrix_width - 1 - columnPosition);
    }
    leds[index] = this->data->color_1;
  }
}
