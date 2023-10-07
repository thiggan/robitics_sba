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
const int echoThreshold = 15;
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

  // we are assuming that a value of zero means there was no reading or the devicie is disconnected
  if(echoLeftValue == 0)
  {
    isLeftSensorWorking = false;
  }
  else
  {
    isLeftSensorWorking = true;
  }

  // we are assuming that a value of zero means there was no reading or the devicie is disconnected
  if(echoRightValue == 0)
  {
    isRightSensorWorking = false;
  }
  else
  {
    isRightSensorWorking = true;
  }
 
  Serial.print(" E L: ");
  if(isLeftSensorWorking == true)
  {
    Serial.print(echoLeftValue);
  }
  else
  {
    Serial.print(" (no-value) ");
  }
  
  Serial.print(" L L: ");
  Serial.print(leftEyeValue);
  Serial.print(" L R: ");
  Serial.print(rightEyeValue);
  Serial.print(" E R: ");
  if(isRightSensorWorking == true)
  {
    Serial.print(echoRightValue);
  }
  else
  {
    Serial.print(" (no-value) ");
  }

  if (leftEyeValue > lightThreshold || rightEyeValue > lightThreshold)
  {
    Serial.print(" FOLLOW");
    currentState = FOLLOW;
  }
  else
  {
    Serial.print(" SEARCH");
    // No light detected search for light
    currentState = SEARCH;
  }

  if (isLeftSensorWorking && echoLeftValue <= echoThreshold)
  {
    Serial.print(" AVOID_LEFT");
    currentState = AVOID_LEFT;
  }
  if (isRightSensorWorking && echoRightValue <= echoThreshold) //not working
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
    avoidObstacleOnLeft();
    break;

  case AVOID_RIGHT:
    avoidObstacleOnRight();
    break;
  }

  delay(100);
}

void follow(int leftEyeValue, int rightEyeValue)
{
  motors.setSpeeds(leftEyeValue * 2, rightEyeValue * 2);
}

void searchForLight()
{
  motors.setSpeeds(TOP_SPEED, TOP_SPEED);
  delay(500);
}

// void searchForLight()
// {
//   // LED indicates calibration is happening
//   pinMode(13, OUTPUT);
//   digitalWrite(13, HIGH);

//   int i;
//   for (i = 0; i < 60; i++)
//   {
//     // Since our counter runs to 60, the total delay will be
//     // 60*20 = 1200 ms.
//     delay(20);

//     if ((i > 30 && i <= 100) || (i > 50 && i <= 70))
//     {
//       motors.setSpeeds(-TOP_SPEED, TOP_SPEED);
//     }
//     else
//     {
//       motors.setSpeeds(TOP_SPEED, TOP_SPEED);
//     }

//     int leftEyeValue = analogRead(left);
//     int rightEyeValue = analogRead(right);

//     if (leftEyeValue < lightThreshold || rightEyeValue < lightThreshold)
//     {
//       Serial.print("FOLLOW");
//       currentState = FOLLOW;

//       // motors.setSpeeds(0, 0);

//       // Turn off LED to indicate we are through with calibration
//       digitalWrite(13, LOW);

//       return;
//     }
//   }

//   motors.setSpeeds(0, 0);

//   // Turn off LED to indicate we are through with calibration
//   digitalWrite(13, LOW);
// }

void avoidObstacleOnLeft() // turn right
{
  motors.setSpeeds(TOP_SPEED, -TOP_SPEED);
  delay(100);

}

void avoidObstacleOnRight() // turn left // this sensor is not working..... 
{
  motors.setSpeeds(-TOP_SPEED, TOP_SPEED);
    delay(100);
}

