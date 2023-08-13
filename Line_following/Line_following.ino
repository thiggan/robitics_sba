// Tracy MacAulay-Higgan
//
//

#include <Wire.h>
#include <ZumoShield.h>



ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
int lastError = 0;

const int TOP_SPEED = 100;
const int LINE_THRESHOLD = 500;
const int SEARCH_DURATION = 1000;

enum RobotState
{
  FOLLOW_PATH,
  FIND_PATH
};

RobotState currentState = FIND_PATH;

void setup()
{
  reflectanceSensors.init();
  calibration();
}

bool hasLostLine(unsigned int sensors[6])
{
  if (sensors[6] < LINE_THRESHOLD)
  {
    return true; // Robot has lost the line
  }
  else
  {
    return false; // Robot has not lost the line
  }
}

void loop()
{
  unsigned int sensors[6];
  int position = reflectanceSensors.readLine(sensors);

  switch (currentState)
  {
  case FOLLOW_PATH:
    follow();
    if (hasLostLine(sensors))
    {
      currentState = FIND_PATH;
    }
    else
    {
      currentState = FOLLOW_PATH;
    }
    break;

  case FIND_PATH:
    searchForLine();

    if (hasLostLine(sensors))
    {
      currentState = FIND_PATH;
    }
    else
    {
      currentState = FOLLOW_PATH;
    }
    break;
  }
}

void calibration()
{
  // LED indicates calibration is happening
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait for set time to begin calibration in this case it is 1 second
 
  delay(1000);
  int i;
  for (i = 0; i < 150; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
    {
      motors.setSpeeds(TOP_SPEED, -TOP_SPEED);
    }
    else
    {
      motors.setSpeeds(-TOP_SPEED, TOP_SPEED);
    }

    reflectanceSensors.calibrate();

    // Since our counter runs to 100, the total delay will be
    // 100*20 = 2000 ms.
    delay(20);
  }
  motors.setSpeeds(0, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
}

void follow()
{

  unsigned int sensors[6];


  // position of line and sensors
  int position = reflectanceSensors.readLine(sensors);


  // this is how far away from the centre of the line the robot is 
  int error = position - 2500;

  // motor speed difference for the motors 
  int speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;

  //individual motor speeds + turns left - turns right
  
  int m1Speed = TOP_SPEED + speedDifference;
  int m2Speed = TOP_SPEED - speedDifference;

 
// Setting motor speeds and setting foward and backwards

  if (m1Speed < -TOP_SPEED)
    m1Speed = -TOP_SPEED;
  if (m2Speed < -TOP_SPEED)
    m2Speed = -TOP_SPEED;
  if (m1Speed > TOP_SPEED)
    m1Speed = TOP_SPEED;
  if (m2Speed > TOP_SPEED)
    m2Speed = TOP_SPEED;

  motors.setSpeeds(m1Speed, m2Speed);
}

void searchForLine()
{
  digitalWrite(13, HIGH);

  // turnLeft(250);
  // straight(250);
  // turnRight(250);
  // straight(250);

  reverse(1000);

   digitalWrite(13, LOW);
}

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
