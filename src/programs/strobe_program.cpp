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

  for (int i = 0; i < this->matrix_width * this->matrix_height; i++) {
    leds[i] = isOn ? *color_1 : *color_2;
  }
}