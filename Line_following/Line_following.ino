// Tracy MacAulay-Higgan
//
//

#include <Wire.h>
#include <ZumoShield.h>

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
int lastError = 0;

const int TOP_SPEED = 250;
const int LINE_THRESHOLD = 60;
const int SEARCH_DURATION = 500;

enum RobotState
{
  FOLLOW_PATH,
  FIND_PATH
};

RobotState currentState = FOLLOW_PATH;

void setup()
{
  Serial.begin(9600);
  reflectanceSensors.init();
  calibration();
}

bool hasLostLine(unsigned int sensors[6])
{
  if (sensors[0] < LINE_THRESHOLD 
      && sensors[1] < LINE_THRESHOLD 
      && sensors[2] < LINE_THRESHOLD 
      && sensors[3] < LINE_THRESHOLD 
      && sensors[4] < LINE_THRESHOLD 
      && sensors[5] < LINE_THRESHOLD)
  {
    return true;
  }
  else
  {
    return false;
  }
}

//tracking the sample times for the data, 

void output(unsigned int sensors[6])
{
  static uint16_t priorTestMeasure = 0;

  if ((uint16_t)(millis() - priorTestMeasure) >= 100)
  {
    priorTestMeasure = millis();

    // // Read the reflectance sensors.
    // uint16_t sensorValues[6]
    // reflectanceSensors.read(sensorValues, QTR_EMITTERS_ON);

    // Send the results to the serial monitor, an out put in 4 digit lots to make it easier to read - it has an e and the end to signify the end of that piece of data. The number could be changed to have bigger or smaller numbers too but 4 is good to read, it also formats the line and adds a new line character.
    char buffer[80];
    sprintf(buffer, "%4d %4d %4d %4d %4d %4d %c\n",
      sensors[0],
      sensors[1],
      sensors[2],
      sensors[3],
      sensors[4],
      sensors[5],
      'E'
    );
    Serial.print(buffer);
  }
}

// when it starts is on follow path-driving then when is loses the line is goes into find path/serach for line this is reversing until it finds the line again. 
void loop()
{
  unsigned int sensors[6];
  int position = reflectanceSensors.readLine(sensors);

  output(sensors);

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


// this is some code I borrowed form the example code. 
void calibration()
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

  // position of line and sensors
  int position = reflectanceSensors.readLine(sensors);

  // this is how far away from the centre of the line the robot is
  int error = position - 2500;

  // motor speed difference for the motors
  int speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;

  // individual motor speeds + turns left - turns right

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

  reverse(100);

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
