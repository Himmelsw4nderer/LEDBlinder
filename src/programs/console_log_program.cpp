#include "console_log_program.hpp"

void ConsoleLogProgram::update(CRGBW* leds) {
  Serial.println("ConsoleLogProgram::update called with the following values:");

  Serial.print("Color 1: R=");
  Serial.print(this->data->color_1.r);
  Serial.print(", G=");
  Serial.print(this->data->color_1.g);
  Serial.print(", B=");
  Serial.print(this->data->color_1.b);
  Serial.print(", W=");
  Serial.println(this->data->color_1.w);
  Serial.print("Color 2: R=");
  Serial.print(this->data->color_2.r);
  Serial.print(", G=");
  Serial.print(this->data->color_2.g);
  Serial.print(", B=");
  Serial.print(this->data->color_2.b);
  Serial.print(", W=");
  Serial.println(this->data->color_2.w);
  Serial.print("Color 3: R=");
  Serial.print(this->data->color_3.r);
  Serial.print(", G=");
  Serial.print(this->data->color_3.g);
  Serial.print(", B=");
  Serial.print(this->data->color_3.b);
  Serial.print(", W=");
  Serial.println(this->data->color_3.w);
  Serial.print("Color 4: R=");
  Serial.print(this->data->color_4.r);
  Serial.print(", G=");
  Serial.print(this->data->color_4.g);
  Serial.print(", B=");
  Serial.print(this->data->color_4.b);
  Serial.print(", W=");
  Serial.println(this->data->color_4.w);

  Serial.print("Value 1: ");
  Serial.println(this->data->value_1);
  Serial.print("Value 2: ");
  Serial.println(this->data->value_2);
}