#include "strobe_program.hpp"

void StrobeProgram::update(CRGBW* leds) {
  static unsigned long lastToggleTime = 0;
  static bool isOn = false;

  const unsigned long toggleInterval =
      isOn ? this->data->value_1 * 10 : this->data->value_2 * 10;

  unsigned long currentTime = millis();
  if (currentTime - lastToggleTime < toggleInterval) {
    return;
  }
  lastToggleTime = lastToggleTime + toggleInterval;

  isOn = !isOn;

  CRGBW color = isOn ? this->data->color_1 : this->data->color_2;
  uint matrix_amount = this->matrix_width * this->matrix_height;

  for (int i = 0; i < matrix_amount; i++) {
    leds[i] = color;
  }
}