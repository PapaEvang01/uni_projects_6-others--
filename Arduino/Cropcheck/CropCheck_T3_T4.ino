/*
===========================================================
 CropCheck - Seed Evaluation & Distance Alert (T3 & T4)
===========================================================
This Arduino sketch implements:

- T3: Seed evaluation using a touch button, servo lid, vibration motor, and LED pulsing.
- T4: Distance-based warning using an ultrasonic sensor, vibrator, and red blinking LEDs.

Author: [Your Name]
*/

#include <Ultrasonic.h>
#include <ESP32Servo.h>
#include <Ticker.h>
#include <Wire.h>
#include <FastLED.h>

// === Pin & Hardware Definitions ===
#define LED_PIN 2
#define NUM_LEDS 30
#define BRIGHTNESS 128
#define POTENTIOMETER_PIN 36
#define VIBRATOR_PIN 18
#define BUTTON_PIN 34
#define SERVO_PIN 16

CRGB leds[NUM_LEDS];
Ultrasonic ultra(12);
Servo myservo;

Ticker ServoTicker;
Ticker UltrasonicTicker;

// Button and servo state tracking
int buttonState = LOW;
int previousButtonState = LOW;
int servoAngle = 0;

// === T3: Seed Evaluation Logic ===
void button() {
  buttonState = digitalRead(BUTTON_PIN); // Read the touch button state

  if (buttonState != previousButtonState) { // Detect state change
    if (buttonState == HIGH) {
      // Toggle the lid angle
      servoAngle = (servoAngle == 0) ? 180 : 0;
      myservo.write(servoAngle);
      delay(50); // Debounce delay
    }
    previousButtonState = buttonState; // Update state
  }

  // Read potentiometer and map to vibration intensity
  int potValue = analogRead(POTENTIOMETER_PIN);
  Serial.println(potValue);
  int vibrationIntensity = map(potValue, 0, 4095, 0, 255);
  vibrationIntensity = constrain(vibrationIntensity, 0, 255);
  analogWrite(VIBRATOR_PIN, vibrationIntensity);

  // Pulse all LEDs in green based on vibration intensity
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, vibrationIntensity, 0); // Green pulse effect
  }

  FastLED.show();
  delay(50); // Small delay to smooth LED transition
}

// === T4: Distance Alert Logic ===
void ultrasonic() {
  long distance = ultra.MeasureInCentimeters();
  Serial.println("THIS IS DISTANCE: " + String(distance));

  if (distance < 30) {
    // Close object detected → warning mode
    ServoTicker.detach(); // Temporarily disable servo updates
    analogWrite(VIBRATOR_PIN, 250); // Strong vibration

    // Blink all LEDs red for 5 cycles
    for (int i = 0; i < 5; i++) {
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      FastLED.show();
      delay(500);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      delay(500);
    }

    ServoTicker.attach(0.1, button); // Re-enable servo logic
  } else {
    analogWrite(VIBRATOR_PIN, 0); // No alert
  }
}

void setup() {
  // Initialize components
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(VIBRATOR_PIN, OUTPUT);

  myservo.attach(SERVO_PIN);

  // Schedule recurring tasks
  UltrasonicTicker.attach(5, ultrasonic);   // Check distance every 5 seconds
  ServoTicker.attach(0.1, button);          // Handle button + vibration nearly continuously

  Serial.begin(9600);
}

void loop() {
  // Nothing here — logic handled by Ticker callbacks
}
