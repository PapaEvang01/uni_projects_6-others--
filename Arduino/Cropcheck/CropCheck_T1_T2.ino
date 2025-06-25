/*
===============================================
 CropCheck - Environmental Monitoring & Display
===============================================
This Arduino sketch implements Tasks T1 and T2:

- T1: Reads ambient light (TSL2561), temperature, and humidity (DHT11) every 5 seconds.
- T2: Displays sensor status on a 30-LED RGB strip:
  • LED 0 → White: light
  • LED 1 → White: temperature
  • LED 2 → White: humidity
  • LEDs 11–20 → Bar visualization (color-coded)
    - Green: within range
    - Yellow: below threshold
    - Blinking red: above threshold

Author: [Your Name]
*/

#include "DHTesp.h"
#include <Ticker.h>
#include <Wire.h>
#include <Digital_Light_TSL2561.h>
#include <FastLED.h>

// ==== LED Strip Configuration ====
#define BRIGHTNESS 128
#define NUM_LEDS 30
#define STRIP_PIN 2

// ==== Sensor Pin Definitions ====
#define DHT_PIN 27

CRGB leds[NUM_LEDS];
DHTesp dhtSensor;

Ticker Toggler_Print_Values;

// Temporary storage for sensor values
float temperature = 0.0;
float humidity = 0.0;

// === Function: Print sensor values and control LEDs ===
void print_values() {
  // --- LIGHT SENSOR ---
  int rawLight = TSL2561.readVisibleLux();     // Read visible light (lux)
  int light = constrain(rawLight, 0, 2000);     // Clamp to expected range
  Serial.print("The Light Level is: ");
  Serial.println(light);

  // --- TEMPERATURE & HUMIDITY SENSOR ---
  TempAndHumidity thValues = dhtSensor.getTempAndHumidity();
  temperature = constrain(thValues.temperature, 20, 30);
  humidity = constrain(thValues.humidity, 20, 80);
  Serial.println("Temperature: " + String(temperature) + " C");
  Serial.println("Humidity: " + String(humidity) + "%");

  // === LED Visualization ===
  for (int i = 0; i < 3; i++) {
    if (i == 0) { // LIGHT
      leds[i] = CRGB::White;

      if (light >= 200 && light <= 1000) {
        int level = map(light, 200, 1000, 1, 10);
        fill_solid(leds + 11, level, CRGB::Green);
      } else if (light < 200) {
        int level = map(light, 0, 200, 1, 10);
        fill_solid(leds + 11, level, CRGB::Yellow);
      } else {
        fill_solid(leds + 11, 10, CRGB::Red);
        for (int j = 0; j < 3; j++) {
          FastLED.show(); delay(500);
          fill_solid(leds + 11, 10, CRGB::Black);
          FastLED.show(); delay(500);
        }
        leds[i] = CRGB::Black;
        FastLED.show();
        continue;
      }

      FastLED.show();
      delay(3000);
      fill_solid(leds + 11, 10, CRGB::Black);
      leds[i] = CRGB::Black;
      FastLED.show();
    }

    if (i == 1) { // TEMPERATURE
      leds[i] = CRGB::White;

      if (temperature >= 23 && temperature <= 27) {
        int level = map(temperature, 23, 27, 1, 10);
        fill_solid(leds + 11, level, CRGB::Green);
      } else if (temperature < 23) {
        fill_solid(leds + 11, 2, CRGB::Yellow);
      } else {
        fill_solid(leds + 11, 10, CRGB::Red);
        for (int j = 0; j < 3; j++) {
          FastLED.show(); delay(500);
          fill_solid(leds + 11, 10, CRGB::Black);
          FastLED.show(); delay(500);
        }
        leds[i] = CRGB::Black;
        FastLED.show();
        continue;
      }

      FastLED.show();
      delay(3000);
      fill_solid(leds + 11, 10, CRGB::Black);
      leds[i] = CRGB::Black;
      FastLED.show();
    }

    if (i == 2) { // HUMIDITY
      leds[i] = CRGB::White;

      if (humidity >= 40 && humidity <= 60) {
        int level = map(humidity, 40, 60, 1, 10);
        fill_solid(leds + 11, level, CRGB::Green);
      } else if (humidity < 40) {
        fill_solid(leds + 11, 2, CRGB::Yellow);
      } else {
        fill_solid(leds + 11, 10, CRGB::Red);
        for (int j = 0; j < 3; j++) {
          FastLED.show(); delay(500);
          fill_solid(leds + 11, 10, CRGB::Black);
          FastLED.show(); delay(500);
        }
        leds[i] = CRGB::Black;
        FastLED.show();
        continue;
      }

      FastLED.show();
      delay(3000);
      fill_solid(leds + 11, 10, CRGB::Black);
      leds[i] = CRGB::Black;
      FastLED.show();
    }
  }
}

void setup() {
  // Initialize RGB LED strip
  FastLED.addLeds<NEOPIXEL, STRIP_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // Initialize sensors
  Wire.begin();
  TSL2561.init();
  dhtSensor.setup(DHT_PIN, DHTesp::DHT11);

  // Start serial communication
  Serial.begin(9600);

  // Schedule print_values every 5 seconds
  Toggler_Print_Values.attach(5, print_values);
}

void loop() {
  // Nothing here - work is done by Ticker every 5 seconds
}
