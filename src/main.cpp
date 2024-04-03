#include <Arduino.h>
#include <ArtnetWifi.h>
#include <FastLED.h>

#include <cstdint>

#include "fast_led_sk6812.hpp"
#include "programs/all_leds_on_program.hpp"
#include "programs/console_log_program.hpp"
#include "programs/led_program.hpp"
#include "programs/strobe_program.hpp"
#include "programs/verticle_bar_program.hpp"
#include "wifi_credentials.hpp"

const int number_leds = 144;
const int matrix_width = 36;
const int matrix_height = 4;
const int number_channels = 19;

const byte data_pin = 0;

CRGBW leds[number_leds];
CRGB* leds_rgb = (CRGB*)&leds[0];

ArtnetWifi artnet;

LEDProgram* current_program = nullptr;
uint8_t current_program_number = -1;
LEDProgram* programs[] = {new ConsoleLogProgram(), new AllLEDsOnProgram(),
                          new StrobeProgram(), new VerticalBarProgram()};
const int number_programs = sizeof(programs) / sizeof(programs[0]);

const int start_universe = 0;
const int start_channel = 0;
const int max_universe =
    number_channels / 512 + ((number_channels % 512) ? 1 : 0);
bool universes_received[max_universe];
bool send_frame = 1;

struct LedData {
  CRGBW color_1;
  CRGBW color_2;
  CRGBW color_3;
  CRGBW color_4;
  uint8_t value_1;
  uint8_t value_2;
};

QueueHandle_t ledDataQueue;

bool connect_wifi(char* wlan_ssid, char* wlan_password);
void init_test();
void on_dmx_frame(uint16_t universe, uint16_t length, uint8_t sequence,
                  uint8_t* data);
void updateTask(void* pvParameters);

void setup() {
  Serial.begin(115200);
  connect_wifi(WIFI_SSID, WIFI_PASSWORD);
  artnet.begin();
  FastLED.addLeds<WS2812B, data_pin, RGB>(leds_rgb, getRGBWsize(number_leds));
  init_test();

  memset(universes_received, 0, max_universe);
  artnet.setArtDmxCallback(on_dmx_frame);

  ledDataQueue = xQueueCreate(1, sizeof(LedData));  // Queue für 10 Nachrichten
  if (ledDataQueue == NULL) {
    Serial.println("Failed to create the queue");
  }

  xTaskCreate(updateTask,        /* Task-Funktion */
              "Update LED Task", /* Name des Tasks */
              10000,             /* Stack-Größe in Wörtern */
              NULL,              /* Parameter für die Task-Funktion */
              0,                 /* Priorität des Tasks */
              NULL               /* Task-Handle */
  );
}

void loop() { artnet.read(); }

bool connect_wifi(char* wlan_ssid, char* wlan_password) {
  /**
   * Connect to a WiFi network.
   *
   * Tries to connect to the specified WiFi network using the provided SSID
   * and password. Continuously attempts to connect until a connection is
   * established or the attempt limit is reached.
   *
   * Args:
   *     wlan_ssid (str): The SSID of the WiFi network to connect to.
   *     wlan_password (str): The password of the WiFi network.
   *
   * Returns:
   *     bool: True if the connection was successful, False otherwise.
   */
  bool state = true;
  int i = 0;

  WiFi.begin(wlan_ssid, wlan_password);
  Serial.println("\nConnecting to WiFi");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(wlan_ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

void init_test() {
  /**
   * Initializes and tests the LED strip by lighting up LEDs in red, green,
   * blue, and then turning them off. Each color is displayed for half a
   * second.
   */
  Serial.println("Starting LED test...");
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(127, 0, 0, 0);
  }
  FastLED.show();
  delay(100);
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(0, 127, 0, 0);
  }
  FastLED.show();
  delay(100);
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(0, 0, 127, 0);
  }
  FastLED.show();
  delay(100);
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(0, 0, 0, 127);
  }
  FastLED.show();
  delay(100);
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
  FastLED.show();
  Serial.println("LED test completed.");
}

void on_dmx_frame(uint16_t universe, uint16_t length, uint8_t sequence,
                  uint8_t* data) {
  /**
   * Handles a DMX frame for LED control based on the universe and data
   * received.
   *
   * This function is called upon receiving a DMX frame, adjusting LED
   * brightness if the frame is from a specific universe (e.g., 15) and
   * setting LED colors based on the data provided. It ensures that all
   * expected universes have been received before updating the LEDs. The
   * function assumes a global setup for universes_received and leds arrays,
   * which track received universes and store LED states, respectively.
   *
   * Args:
   * - universe (int): The DMX universe from which the frame was received.
   * Used to determine action and whether to process or ignore the frame.
   * - length (int): The length of the DMX data array. Not directly used in
   * this function, but included for compatibility with typical DMX frame
   * handlers.
   * - sequence (int): The sequence number of the DMX frame. Not directly used
   * in this function, but useful for debugging or advanced control logic.
   * - data (list of int or bytearray): The DMX data for the universe. This
   * array contains the actual control values for LEDs or other devices being
   * controlled via DMX.
   *
   * Returns:
   * - None. LED updates are performed within the function based on the
   * received DMX frame data, with no return value.
   */
  int offset = start_channel;

  if (length < (start_channel + 19)) {
    Serial.println("Data length too short, ignoring frame.");
    return;
  }

  uint8_t program_selector = data[start_channel];

  if (current_program == nullptr ||
      current_program_number != program_selector) {
    if (program_selector >= 0 && program_selector < number_programs) {
      current_program = programs[program_selector];
      current_program->initialize(matrix_width, matrix_height);
      current_program_number = program_selector;
    } else {
      Serial.println("Invalid program selector, ignoring frame.");
      return;
    }
  }

  if (current_program != nullptr) {
    LedData msg;
    LedData dummyData;
    msg.color_1 = CRGBW(data[offset + 1], data[offset + 2], data[offset + 3],
                        data[offset + 4]);
    msg.color_2 = CRGBW(data[offset + 5], data[offset + 6], data[offset + 7],
                        data[offset + 8]);
    msg.color_3 = CRGBW(data[offset + 9], data[offset + 10], data[offset + 11],
                        data[offset + 12]);
    msg.color_4 = CRGBW(data[offset + 13], data[offset + 14], data[offset + 15],
                        data[offset + 16]);
    msg.value_1 = data[offset + 17];
    msg.value_2 = data[offset + 18];

    xQueueReceive(ledDataQueue, &dummyData, (TickType_t)0);
    if (xQueueSend(ledDataQueue, &msg, portMAX_DELAY) != pdPASS) {
      Serial.println("Failed to send to the queue");
    }

    // current_program->update(leds, &color_1, &color_2, &color_3, &color_4,
    //                         value_1, value_2);
    FastLED.show();
  }
}

void updateTask(void* pvParameters) {
  LedData msg;
  for (;;) {
    xQueueReceive(ledDataQueue, &msg, portMAX_DELAY);
    if (current_program != nullptr) {
      current_program->update(leds, &msg.color_1, &msg.color_2, &msg.color_3,
                              &msg.color_4, msg.value_1, msg.value_2);
      Serial.println(millis());
      // vTaskDelay(pdMS_TO_TICKS(20));
    }
  }
}
