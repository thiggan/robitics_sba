#include <Wire.h>
#include <ZumoShield.h>

#define LED_PIN 13

ZumoMotors motors;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void straight(int duration) {
  motors.setLeftSpeed(100);   
  motors.setRightSpeed(100);  
  delay(duration);
 
}

void turnLeft(int duration) {
  motors.setLeftSpeed(-400);   
  motors.setRightSpeed(200); 
  delay(duration); 
  
}

  void turnRight(int duration) {
    motors.setLeftSpeed(200); 
    motors.setRightSpeed(-400);
    delay(duration);
  }

  void stop(int duration) {
  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);
  delay(duration);
}

void zig_zag(){
   straight(1000);

    turnLeft(200);

    straight(1000);

    turnRight(200);

    straight(1000);

    turnLeft(200);

    straight(1000);

    turnRight(200);

}

void loop() {
zig_zag();

stop(500);
  
}



