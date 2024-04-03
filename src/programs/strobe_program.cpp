#include "strobe_program.hpp"

void StrobeProgram::update(CRGBW* leds, CRGBW* color_1, CRGBW* color_2,
                           CRGBW* color_3, CRGBW* color_4, uint8_t value_1,
                           uint8_t value_2) {
  static unsigned long lastToggleTime = 0;
  static bool isOn = false;

  const unsigned long toggleInterval = isOn ? value_1 * 10 : value_2 * 10;

  unsigned long currentTime = millis();
  if (currentTime - lastToggleTime < toggleInterval) {
    return;
  }
  lastToggleTime = lastToggleTime + toggleInterval;

  isOn = !isOn;

  CRGBW color = isOn ? *color_1 : *color_2;
  uint matrix_amount = this->matrix_width * this->matrix_height;

  for (int i = 0; i < matrix_amount; i++) {
    leds[i] = color;
  }
}