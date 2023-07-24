#include <Wire.h>
#include <ZumoShield.h>

#define LED_PIN 13

ZumoMotors motors;

void setup() {
  pinMode(LED_PIN, OUTPUT);

}

  void stop(int duration) {
  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);
  delay(duration);
}

void circle(){
 //having the right motor at half the makes the robot move in a circular pattern
  motors.setLeftSpeed(1000);
  motors.setRightSpeed(500); 
}

void loop() {

 circle();

 stop(500);

}
