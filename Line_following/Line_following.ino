#include <Wire.h>
#include <ZumoShield.h>

// Tracy MacAulay-Higgan
//
//

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
int lastError = 0;

const int TOP_SPEED = 100;
const int LINE_THRESHOLD = 500;
const int SEARCH_DURATION = 1000;

enum RobotState
{
  FOLLOW_PATH,
  LOST_PATH,
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
      currentState = LOST_PATH;
    }
    break;

  case LOST_PATH:
    motors.setSpeeds(0, 0);
    currentState = FIND_PATH;
    break;

  case FIND_PATH:
    searchForLine();

    if (hasLostLine(sensors))
    {
      currentState = LOST_PATH;
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
  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for (i = 0; i < 80; i++)
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

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
}

void follow()
{

  unsigned int sensors[6];

  // Get the position of the line.  Note that we *must* provide the "sensors"
  // argument to readLine() here, even though we are not interested in the
  // individual sensor readings
  int position = reflectanceSensors.readLine(sensors);

  // Our "error" is how far we are away from the center of the line, which
  // corresponds to position 2500.
  int error = position - 2500;

  // Get motor speed difference using proportional and derivative PID terms
  // (the integral term is generally not very useful for line following).
  // Here we are using a proportional constant of 1/4 and a derivative
  // constant of 6, which should work decently for many Zumo motor choices.
  // You probably want to use trial and error to tune these constants for
  // your particular Zumo and line course.
  int speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;

  // Get individual motor speeds.  The sign of speedDifference
  // determines if the robot turns left or right.
  int m1Speed = TOP_SPEED + speedDifference;
  int m2Speed = TOP_SPEED - speedDifference;

  // Here we constrain our motor speeds to be between 0 and TOP_SPEED.
  // Generally speaking, one motor will always be turning at TOP_SPEED
  // and the other will be at TOP_SPEED-|speedDifference| if that is positive,
  // else it will be stationary.  For some applications, you might want to
  // allow the motor speed to go negative so that it can spin in reverse.
  if (m1Speed < 0)
    m1Speed = 0;
  if (m2Speed < 0)
    m2Speed = 0;
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

  reverse(TOP_SPEED);

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
  motors.setLeftSpeed(TOP_SPEED);
  motors.setRightSpeed(-TOP_SPEED);
  delay(duration);
}

void turnRight(int duration)
{
  motors.setLeftSpeed(-TOP_SPEED);
  motors.setRightSpeed(TOP_SPEED);
  delay(duration);
}
