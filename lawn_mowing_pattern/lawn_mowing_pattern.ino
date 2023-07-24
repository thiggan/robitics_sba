#include <ZumoShield.h>
#include <Wire.h>


#define LED_PIN 13

ZumoMotors motors;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // Uncomment one or both of the following lines if your motors' directions need to be flipped
  // motors.flipLeftMotor(true);
  // motors.flipRightMotor(true);
}

void straight(int duration) {
  motors.setLeftSpeed(200);   
  motors.setRightSpeed(200);  
  delay(duration);
}

void turnLeft(int duration) {
  motors.setLeftSpeed(-200);   
  motors.setRightSpeed(200); 
  delay(duration);
}

void turnRight(int duration){
  motors.setLeftSpeed(200);
  motors.setRightSpeed(-200);
  delay(duration);
}

void stop(int duration) {
  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);
  delay(duration);
}

void lawn_mower(){

    straight(2000);

    turnLeft(300);
    straight(500);
    turnLeft(300);

    straight(2000);

    turnRight(300);
    straight(500);
    turnRight(300);

}


//This will go over 4 rotations
void loop() {

  lawn_mower();

  stop(500);
}



