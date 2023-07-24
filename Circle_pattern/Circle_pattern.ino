#include <Wire.h>
#include <ZumoShield.h>

#define LED_PIN 13

ZumoMotors motors;

void setup() {
  pinMode(LED_PIN, OUTPUT);

}

void loop() {

  //having the right motor at half the makes the robot move in a circular pattern
  motors.setLeftSpeed(500);
  motors.setRightSpeed(250); 

  delay(3000); 

}
