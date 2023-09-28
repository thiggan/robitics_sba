#include <Wire.h>

#include <ZumoShield.h>

#include <Arduino.h> // Include the Arduino library if you're using an Arduino board


ZumoMotors motors;


// Define LDR pin and motor pins


  const int left = A4;
  const int right = A5;

  int leftEyeValue;
  int rightEyeValue;

const int lightThreshold = 20;
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
  // Read the LDR sensor leftEyeValue
  int leftEyeValue = analogRead(left);
  Serial.println(leftEyeValue);
  delay(200);
  int rightEyeValue = analogRead(right);
  Serial.println(rightEyeValue);
  delay(200);

  // Check if light is detected
  if (leftEyeValue > lightThreshold || rightEyeValue > lightThreshold) {
    // straight(100); // Move forward towards the light

    motors.setSpeeds(leftEyeValue *2, rightEyeValue *2);
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


