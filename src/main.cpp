#include <Arduino.h>
#include <ArtnetWifi.h>
#include <FastLED.h>

#include "fast_led_sk6812.hpp"
#include "wifi_credentials.hpp"

const int number_leds = 144;
const int matrix_width = 36;
const int matrix_height = 4;
const int matrix_rows = 2;
const int matrix_collums = 2;
const int number_channels = 4 * matrix_rows * matrix_collums;

const byte data_pin = 0;

CRGBW leds[number_leds];
CRGB* leds_rgb = (CRGB*)&leds[0];

ArtnetWifi artnet;

const int start_universe = 0;

const int max_universe =
    number_channels / 512 + ((number_channels % 512) ? 1 : 0);
bool universes_received[max_universe];
bool send_frame = 1;

bool connect_wifi(char* wlan_ssid, char* wlan_password);
void init_test();
void on_dmx_frame(uint16_t universe, uint16_t length, uint8_t sequence,
                  uint8_t* data);

void setup() {
  Serial.begin(115200);
  connect_wifi(WIFI_SSID, WIFI_PASSWORD);
  artnet.begin();
  FastLED.addLeds<WS2812B, data_pin, RGB>(leds_rgb, getRGBWsize(number_leds));
  init_test();

  memset(universes_received, 0, max_universe);
  artnet.setArtDmxCallback(on_dmx_frame);
}

void loop() { artnet.read(); }

bool connect_wifi(char* wlan_ssid, char* wlan_password) {
  /**
   * Connect to a WiFi network.
   *
   * Tries to connect to the specified WiFi network using the provided SSID and
   * password. Continuously attempts to connect until a connection is
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
   * blue, and then turning them off. Each color is displayed for half a second.
   */
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(127, 0, 0, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(0, 127, 0, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(0, 0, 127, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(0, 0, 0, 127);
  }
  FastLED.show();
  delay(500);
  for (int i = 0; i < number_leds; i++) {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
  FastLED.show();
}

void on_dmx_frame(uint16_t universe, uint16_t length, uint8_t sequence,
                  uint8_t* data) {
  /**
   * Handles a DMX frame for LED control based on the universe and data
   * received.
   *
   * This function is called upon receiving a DMX frame, adjusting LED
   * brightness if the frame is from a specific universe (e.g., 15) and setting
   * LED colors based on the data provided. It ensures that all expected
   * universes have been received before updating the LEDs. The function assumes
   * a global setup for universes_received and leds arrays, which track received
   * universes and store LED states, respectively.
   *
   * Args:
   * - universe (int): The DMX universe from which the frame was received. Used
   * to determine action and whether to process or ignore the frame.
   * - length (int): The length of the DMX data array. Not directly used in this
   * function, but included for compatibility with typical DMX frame handlers.
   * - sequence (int): The sequence number of the DMX frame. Not directly used
   * in this function, but useful for debugging or advanced control logic.
   * - data (list of int or bytearray): The DMX data for the universe. This
   * array contains the actual control values for LEDs or other devices being
   * controlled via DMX.
   *
   * Returns:
   * - None. LED updates are performed within the function based on the received
   * DMX frame data, with no return value.
   */
  send_frame = 1;
  if (universe == 15) {
    FastLED.setBrightness(data[0]);
    FastLED.show();
  }

  if (universe < start_universe) {
    return;
  }
  uint8_t index = universe - start_universe;
  if (index >= max_universe) {
    return;
  }

  universes_received[index] = true;

  for (int i = 0; i < max_universe; i++) {
    if (!universes_received[i]) {
      send_frame = 0;
      break;
    }
  }

  Serial.println("Frame!");
  Serial.print("universe: ");
  Serial.println(universe);
  Serial.print("data: ");
  Serial.println(data[4]);

  for (int led = 0; led < number_leds; led++) {
    int i = 0;
    leds[led] =
        CRGBW(data[i * 3], data[i * 3 + 1], data[i * 3 + 2], data[i * 3 + 3]);
  }
  FastLED.show();

  if (send_frame) {
    FastLED.show();
    memset(universes_received, 0, max_universe);
  }
}