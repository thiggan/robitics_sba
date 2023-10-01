#include <Wire.h>

#include <ZumoShield.h>

#include <Arduino.h>

#include <NewPing.h>

ZumoMotors motors;

enum States
{
  FOLLOW,
  AVOID_LEFT,
  AVOID_RIGHT,
  SEARCH
};

// enum Echo {
//   STOPPED,
//   FIND_LIGHT,
//   AVOIDING_OBSTACLE
// };

const int duration;

States currentState = SEARCH;

// Define LDR pin and motor pins

const int left = A4;
const int right = A5;
const int leftTrigPin = 6;
const int leftEchoPin = 12;
const int rightTrigPin = 1;
const int rightEchoPin = 2;

int leftEyeValue;
int rightEyeValue;

NewPing sonar1(leftTrigPin, leftEchoPin, 150);
NewPing sonar2(rightTrigPin, rightEchoPin, 150);

bool isSensorWorking = false;

const int lightThreshold = 20;
const int echoThreshold = 5;
const int TOP_SPEED = 150;

void setup()
{
  Serial.begin(9600);
  // Initialize motor control pins as OUTPUT
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
}

void loop()
{
  // Read the LDR sensor leftEyeValue
  int leftEyeValue = analogRead(left);
  int rightEyeValue = analogRead(right);

  int echoLeftValue = sonar1.ping_cm(100);
  int echoRightValue = sonar2.ping_cm(100);

  Serial.print(" E L: ");
  Serial.print(echoLeftValue);
  Serial.print(" L L: ");
  Serial.print(leftEyeValue);
  Serial.print(" L R: ");
  Serial.print(rightEyeValue);
  Serial.print(" E R: ");
  Serial.print(echoRightValue);

  if (leftEyeValue > lightThreshold || rightEyeValue > lightThreshold)
  {
    Serial.print(" FOLLOW");
    currentState = FOLLOW;
  }
  else
  {
    Serial.print(" SEARCH");
    // // No light detected
    currentState = SEARCH;
  }

  if (isSensorWorking && echoLeftValue < echoThreshold)
  {
    Serial.print(" AVOID_LEFT");
    currentState = AVOID_LEFT;
  }

  if (isSensorWorking && echoRightValue < echoThreshold)
  {
    Serial.print(" AVOID_RIGHT");
    currentState = AVOID_RIGHT;
  }

  Serial.println("");

  switch (currentState)
  {
  case FOLLOW:
    follow(leftEyeValue, rightEyeValue);
    break;

  case SEARCH:
    searchForLine();
    break;

  case AVOID_LEFT:
    avoidObsticleOnLeft();
    break;
  case AVOID_RIGHT:
    avoidObsticleOnRight();
    break;
  }

  delay(1000);
}

void follow(int leftEyeValue, int rightEyeValue)
{
  motors.setSpeeds(leftEyeValue * 2, rightEyeValue * 2);
}

void searchForLine()
{
  // LED indicates calibration is happening
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait for set time to begin calibration in this case it is 2 seconds

  int i;
  for (i = 0; i < 60; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
    {
      motors.setSpeeds(-TOP_SPEED, TOP_SPEED);
    }
    else
    {
      motors.setSpeeds(TOP_SPEED, -TOP_SPEED);
    }

    // reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
}

void avoidObsticleOnLeft() // turn right
{
  motors.setRightSpeed(TOP_SPEED);
  motors.setLeftSpeed(-TOP_SPEED);
}

void avoidObsticleOnRight() // turn left
{
  motors.setLeftSpeed(TOP_SPEED);
  motors.setRightSpeed(-TOP_SPEED);
}
