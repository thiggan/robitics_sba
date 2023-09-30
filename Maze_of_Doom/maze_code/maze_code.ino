#include <Wire.h>

#include <ZumoShield.h>

#include <Arduino.h> // Include the Arduino library if you're using an Arduino board


ZumoMotors motors;

enum States {
  FOLLOW,
  AVOID_LEFT,
  AVOID_RIGHT,
  SEARCH
};

States currentState = SEARCH;


// Define LDR pin and motor pins


  const int left = A4;
  const int right = A5;

  int leftEyeValue;
  int rightEyeValue;

const int lightThreshold = 20;
const int TOP_SPEED = 250;


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

  if (leftEyeValue > lightThreshold || rightEyeValue > lightThreshold) {
    currentState = FOLLOW;
  } else {
    // // No light detected, make a decision (you can customize this logic)
    currentState = SEARCH;
  }


  switch (currentState)
  {
  case FOLLOW:
    follow(leftEyeValue, rightEyeValue);
    break;

  case SEARCH:
    searchForLine();
    break;

  case AVOID_LEFT:
    break;
  case AVOID_RIGHT:
    break;
  }
}

void follow(int leftEyeValue, int rightEyeValue)
{
  motors.setSpeeds(leftEyeValue *2, rightEyeValue *2);
}

void searchForLine()
{
  // LED indicates calibration is happening
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait for set time to begin calibration in this case it is 1 second

  delay(1000);
  int i;
  for (i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
    {
      motors.setSpeeds(-TOP_SPEED, TOP_SPEED);
    }
    else
    {
      motors.setSpeeds(TOP_SPEED, -TOP_SPEED);
    }

    //reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
}

