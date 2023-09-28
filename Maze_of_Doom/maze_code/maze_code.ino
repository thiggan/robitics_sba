#include <Wire.h>

#include <ZumoShield.h>

#include <Arduino.h> // Include the Arduino library if you're using an Arduino board


ZumoMotors motors;


// Define LDR pin and motor pins

const int leftMotorPin = 5;  // Left motor control pin
const int rightMotorPin = 4; // Right motor control pin


const int lightThreshold = 40;

// Define motor control functions
void forward() {
  digitalWrite(leftMotorPin, HIGH);
  digitalWrite(rightMotorPin, HIGH);
   motors.setLeftSpeed(200);   
  motors.setRightSpeed(200);  

}

void left() {
  digitalWrite(leftMotorPin, LOW);
  digitalWrite(rightMotorPin, HIGH);
  motors.setLeftSpeed(-200);   
  motors.setRightSpeed(200); 


}

void right() {
  digitalWrite(leftMotorPin, HIGH);
  digitalWrite(rightMotorPin, LOW);
motors.setLeftSpeed(200);
  motors.setRightSpeed(-200);
  

}

void backward() {
  digitalWrite(leftMotorPin, LOW);
  digitalWrite(rightMotorPin, LOW);
}

void setup() {
  Serial.begin(9600);
  // Initialize motor control pins as OUTPUT
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

}


void loop() {
  // Read the LDR sensor value
  int lightValue = analogRead(leftMotorPin);
  Serial.println(leftMotorPin);
  delay(2000);
  int lightValue1 = analogRead(rightMotorPin);
  Serial.println(rightMotorPin);
  delay(2000);

  // Check if light is detected
  if (lightValue > lightThreshold || lightValue1 > lightThreshold) {
    forward(); // Move forward towards the light
  } else {
    // // No light detected, make a decision (you can customize this logic)
    // int randomDecision = random(4); // Generate a random decision (0-3)
    
    // // Perform different actions based on the random decision
    // switch (randomDecision) {
    //   case 0:
    //     left(); // Turn left
    //     break;
    //   case 1:
    //     right(); // Turn right
    //     break;
    //   case 2:
    //     backward(); // Move backward
    //     break;
    //   default:
    //     forward(); // Default to moving forward
    //     break;
    }
  }
}

