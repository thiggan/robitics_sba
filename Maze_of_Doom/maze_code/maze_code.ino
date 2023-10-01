#include <Wire.h>

#include <ZumoShield.h>

#include <Arduino.h> 

#include <NewPing.h>


ZumoMotors motors;

enum States {
  FOLLOW,
  AVOID_LEFT,
  AVOID_RIGHT,
  SEARCH
};

enum Echo {
  STOPPED,
  FIND_LIGHT,
  AVOIDING_OBSTACLE
};

const int duration; 

States currentState = SEARCH;


// Define LDR pin and motor pins


  const int left = A4;
  const int right = A5;
  const int leftTrigPin = 1;
  const int leftEchoPin = 2;
  const int rightTrigPin = 6;
  const int rightEchoPin = 12;

  int leftEyeValue;
  int rightEyeValue;

  NewPing sonar1(leftTrigPin, leftEchoPin, 100);
  NewPing sonar2(rightTrigPin, rightEchoPin, 100);

  bool isSensorWorking = false;

const int lightThreshold = 20;
const int TOP_SPEED = 100;


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

   if (leftEyeValue > lightThreshold || rightEyeValue > lightThreshold)
 {
    currentState = FOLLOW;
  } else {
    // // No light detected
    currentState = SEARCH;
  }

  int distance1 = sonar1.ping_cm();
  int distance2 = sonar2.ping_cm();

  if(isSensorWorking && distance1 < 20){
    currentState = AVOID_LEFT;
  }

  if(isSensorWorking && distance2 < 20){
    currentState = AVOID_RIGHT;
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
    avoidObsticleOnRight();
    break;
  case AVOID_RIGHT:
    avoidObsticleOnLeft();
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

  // Wait for set time to begin calibration in this case it is 2 seconds

  delay(2000);
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

    //reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
}

void avoidObsticleOnLeft()
{
    
    motors.setLeftSpeed(TOP_SPEED);
  motors.setRightSpeed(-TOP_SPEED);
  delay(duration);
}

void avoidObsticleOnRight() {

    motors.setLeftSpeed(TOP_SPEED);
  motors.setRightSpeed(-TOP_SPEED);
  delay(duration);
}
