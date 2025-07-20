#include "Robot.h"

uint8_t leftPins[3]{ 8, 9, 5 };
uint8_t rightPins[3]{ 6, 7, 11};
uint8_t servoPin = 10;
uint8_t sonarPins[2]{ 3, 4 };
uint8_t buzzerPin = 13;
uint8_t sliderPin = 12;
uint8_t buttonPin = 2;

Robot autobot(
  leftPins,
  rightPins,
  servoPin,
  sonarPins,
  buzzerPin,
  sliderPin,
  buttonPin);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  autobot.initialize();
}

void loop() {
  autobot.controller();
}
