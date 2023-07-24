#include <Wire.h>
#include <ZumoShield.h>

#define LED_PIN 13

ZumoMotors motors;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // Uncomment one or both of the following lines if your motors' directions need to be flipped
  // motors.flipLeftMotor(true);
  // motors.flipRightMotor(true);
}

void Straight(int duration) {
  motors.setLeftSpeed(200);   
  motors.setRightSpeed(200);  
  delay(duration);

}

void turnLeft(int duration) {
  motors.setLeftSpeed(-200);   
  motors.setRightSpeed(200); 
  delay(duration);
}

void stop(int duration) {
  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);
  delay(duration);
}

void maze(){

  Straight(2000);

    turnLeft(300);

    Straight(1500);

    turnLeft(300);

    Straight(1000);

    turnLeft(300);

    Straight(500);

    turnLeft(300);

}


void loop() {
  
  maze();

  stop(500);


}



