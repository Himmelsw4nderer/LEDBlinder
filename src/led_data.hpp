#pragma once
#include <cstdint>

#include "fast_led_sk6812.hpp"

struct LedData {
  CRGBW color_1;
  CRGBW color_2;
  CRGBW color_3;
  CRGBW color_4;
  uint8_t value_1;
  uint8_t value_2;
};