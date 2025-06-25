/*
====================================================================
 WeatherDisplaySystem.ino - Real-Time Weather Visualization
====================================================================

This Arduino ESP32 project retrieves weather data from OpenWeatherMap API
and maps key parameters to physical actuators:

- 🌡️ Temperature → RGB LED Strip (color-coded + blinking)
- 💨 Wind Speed → Servo Motor (sweep speed)
- 💧 Humidity → Vibration Motor (intensity)

Displays temperature and humidity on an I2C LCD.
Connects to Wi-Fi for API access.

Author: Vaggelis Papaioannou
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <FastLED.h>
#include <ESP32Servo.h>

// === LED Strip Configuration ===
#define LED_PIN 2
#define NUM_LEDS 30
CRGB leds[NUM_LEDS];

// === Actuator Pins ===
const int vibratorPin = 27;
const int servoPin = 18;
Servo myservo;

// === LCD Configuration (I2C address 0x3F or 0x27) ===
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// === Wi-Fi Credentials ===
const char* ssid = "iPhone - Σωτήρης"; //replace it with your
const char* password = "23122002G4"; //same here

// === Weather API Configuration ===
String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "ddf36dffb091a94d4215d3b39083ed35";
String lat = "-57.8503240437811";  // Replace with your latitude
String lon = "-17.92440820077199"; // Replace with your longitude

void setup() {
  // Initialize servo motor
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin);

  // Initialize vibration motor and LED strip
  pinMode(vibratorPin, OUTPUT);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS); 

  // Start Serial Monitor
  Serial.begin(115200);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP address:");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Create full API URL with parameters
    http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

    int httpCode = http.GET(); // Send HTTP GET request

    if (httpCode > 0) {
      // Parse JSON response
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);

      // Extract required weather info
      const char* country = doc["sys"]["country"];
      const char* city = doc["name"];
      float temp = doc["main"]["temp"];
      float humidity = doc["main"]["humidity"];
      const char* clouds = doc["weather"][0]["description"];
      float windSpeed = doc["wind"]["speed"];

      // Display data on LCD
      lcd.clear();
      lcd.print("Temp: ");
      lcd.print(temp);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(humidity);
      lcd.print("%");

      // Debug print to Serial
      Serial.printf("Country: %s\nCity: %s\nClouds: %s\n", country, city, clouds);
      Serial.printf("Temperature: %.1f C\nHumidity: %.1f %%\nWind Speed: %.1f m/s\n", temp, humidity, windSpeed);

      // === Actuator Control Logic ===

      // Map wind speed (0-20 m/s) to delay (2000–1100 ms approx.)
      windSpeed = constrain(windSpeed, 0, 20);
      int servoDelay = map(windSpeed, 0, 20, 900, 0);

      // Map humidity (0-100%) to vibration intensity (0–255)
      humidity = constrain(humidity, 0, 100);
      int vibration = map(humidity, 0, 100, 0, 255);
      analogWrite(vibratorPin, vibration);

      // Control LEDs and Servo based on temperature
      for (int i = 0; i < 30; i++) {
        if (temp < 0) {
          // Blinking blue for freezing conditions
          fill_solid(leds, NUM_LEDS, CRGB::Blue);
          FastLED.show();
          myservo.write(0);
          delay(2000 - servoDelay);
          myservo.write(180);
          fill_solid(leds, NUM_LEDS, CRGB::Black);
          FastLED.show();
          delay(2000 - servoDelay);
        }
        else if (temp < 8) {
          fill_solid(leds, NUM_LEDS, CRGB::Cyan);
          FastLED.show();
          myservo.write(0);
          delay(2000 - servoDelay);
          myservo.write(180);
          delay(2000 - servoDelay);
        }
        else if (temp < 16) {
          fill_solid(leds, NUM_LEDS, CRGB::Green);
          FastLED.show();
          myservo.write(0);
          delay(2000 - servoDelay);
          myservo.write(180);
          delay(2000 - servoDelay);
        }
        else if (temp < 24) {
          fill_solid(leds, NUM_LEDS, CRGB::Yellow);
          FastLED.show();
          myservo.write(0);
          delay(2000 - servoDelay);
          myservo.write(180);
          delay(2000 - servoDelay);
        }
        else if (temp < 32) {
          fill_solid(leds, NUM_LEDS, CRGB::Orange);
          FastLED.show();
          myservo.write(0);
          delay(2000 - servoDelay);
          myservo.write(180);
          delay(2000 - servoDelay);
        }
        else {
          // Blinking red for hot conditions
          fill_solid(leds, NUM_LEDS, CRGB::Red);
          FastLED.show();
          myservo.write(0);
          delay(2000 - servoDelay);
          myservo.write(180);
          fill_solid(leds, NUM_LEDS, CRGB::Black);
          FastLED.show();
          delay(2000 - servoDelay);
        }
      }
    }
    else {
      // Failed HTTP request
      Serial.println("Error retrieving weather data.");
      lcd.clear();
      lcd.print("Can't Get DATA!");
    }

    // End HTTP connection
    http.end();
  }
}
