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

NewPing sonar1(leftTrigPin, leftEchoPin, 100);
NewPing sonar2(rightTrigPin, rightEchoPin, 100);

bool isLeftSensorWorking = true;
bool isRightSensorWorking = true;

const int lightThreshold = 30;
const int echoThreshold = 20;
const int TOP_SPEED = 100;

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
    // // No light detected search for light
    currentState = SEARCH;
  }

  if (isLeftSensorWorking && echoLeftValue > echoThreshold)
  {
    Serial.print(" AVOID_LEFT");
    currentState = AVOID_LEFT;
  }

  if (isRightSensorWorking && echoRightValue > echoThreshold)
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
    searchForLight();
    break;

  case AVOID_LEFT:
    avoidObsticleOnLeft();
    break;

  case AVOID_RIGHT:
    avoidObsticleOnRight();
    break;
  }

  delay(200);
}

void follow(int leftEyeValue, int rightEyeValue)
{
  motors.setSpeeds(leftEyeValue * 2, rightEyeValue * 2);
}

void searchForLight()
{
  // LED indicates calibration is happening
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  int i;
  for (i = 0; i < 60; i++)
  {
    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);

    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
    {
      motors.setSpeeds(-TOP_SPEED, TOP_SPEED);
    }
    else
    {
      motors.setSpeeds(TOP_SPEED, -TOP_SPEED);
    }

    int leftEyeValue = analogRead(left);
    int rightEyeValue = analogRead(right);

    if (leftEyeValue > lightThreshold || rightEyeValue > lightThreshold)
    {
      Serial.print(" FOUND");
      currentState = FOLLOW;

      motors.setSpeeds(0, 0);

      // Turn off LED to indicate we are through with calibration
      digitalWrite(13, LOW);

      return;
    }
  }

  motors.setSpeeds(0, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
}

void avoidObsticleOnLeft() // turn right
{
  motors.setSpeeds(-TOP_SPEED, TOP_SPEED);
  delay(100);

  motors.setSpeeds(TOP_SPEED, TOP_SPEED);
  delay(1000);
}

void avoidObsticleOnRight() // turn left // this sensor is not working..... 
{
  motors.setSpeeds(TOP_SPEED, -TOP_SPEED);
  delay(100);

  motors.setSpeeds(TOP_SPEED, TOP_SPEED);
  delay(1000);
}
