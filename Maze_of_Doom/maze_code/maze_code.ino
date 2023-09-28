#include <Wire.h>

#include <ZumoShield.h>

#include <Arduino.h> // Include the Arduino library if you're using an Arduino board


ZumoMotors motors;


// Define LDR pin and motor pins


  const int left = A5;
  const int right = A4;

  int value;
  int value1;

const int lightThreshold = 60;
const int TOP_SPEED = 250;

void straight(int duration)
{
  motors.setLeftSpeed(TOP_SPEED);
  motors.setRightSpeed(TOP_SPEED);
  delay(duration);
}

void reverse(int duration)
{
  motors.setLeftSpeed(-TOP_SPEED);
  motors.setRightSpeed(-TOP_SPEED);
  delay(duration);
}

void turnLeft(int duration)
{
  motors.setLeftSpeed(-TOP_SPEED);
  motors.setRightSpeed(TOP_SPEED);
  delay(duration);
}

void turnRight(int duration)
{
  motors.setLeftSpeed(TOP_SPEED);
  motors.setRightSpeed(-TOP_SPEED);
  delay(duration);
}

void setup() {
  Serial.begin(9600);
  // Initialize motor control pins as OUTPUT
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);

}


void loop() {
  // Read the LDR sensor value
  int value = analogRead(left);
  Serial.println(value);
  delay(200);
  int value1 = analogRead(right);
  Serial.println(value1);
  delay(200);

  // Check if light is detected
  if (value > lightThreshold || value1 > lightThreshold) {
    straight(100); // Move forward towards the light
  } else {
    // // No light detected, make a decision (you can customize this logic)
    // int randomDecision = random(4); // Generate a random decision (0-3)
    
    // // Perform different actions based on the random decision
    // switch (randomDecision) {
    //   case 0:
    //     left1(); // Turn left
    //     break;
    //   case 1:
    //     right2(); // Turn right
    //     break;
    //   case 2:
    //     backward(); // Move backward
    //     break;
    //   default:
    //     forward(); // Default to moving forward
    //     break;
    // }
  }
}


