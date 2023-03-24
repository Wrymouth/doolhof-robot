// left motor
#define leftMotorDirection 13
#define leftMotorSpeed 11
// right motor
#define rightMotorDirection 12
#define rightMotorSpeed 3
// default values for motor
#define adapterDefaultSpeed 65
#define batteryDefaultSpeed 80
// set the default speed based on the power source
const bool onBattery = false;
const int defaultSpeed = (onBattery) ? batteryDefaultSpeed : adapterDefaultSpeed;

// 7 segment display
#define u1 1
#define u2 2
const int displayPins[] = {4, 5, 6, 7, 8, 9, 10};

// pingsensor
const int trigPin = 0;
const int echoPin = A5;
// linesensor
const int lineSensorPins[] = {A0, A1, A2, A3, A4};
const int lineSensorCount = sizeof lineSensorPins / sizeof lineSensorPins[0];
bool lineSensorState[] = {false, false, false, false, false};

// value to determine if the robot should run
bool initialized = false;
unsigned long pingSensorMillis = millis();
void setup()
{
    // setup the line sensors
    for (int i = 0; i < lineSensorCount; i++)
    {
        pinMode(lineSensorPins[i], INPUT);
    }

    // setup left motor
    pinMode(leftMotorDirection, OUTPUT);
    pinMode(leftMotorSpeed, OUTPUT);

    // setup right motor
    pinMode(rightMotorDirection, OUTPUT);
    pinMode(rightMotorSpeed, OUTPUT);
    // setup ping sensor
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // start the motors and go forward
    setRightMotorDirection(true);
    setLeftMotorDirection(true);
}

void loop()
{
    if (!checkIfRobotIsInitialized())
        return;
    // make the robot go forward
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);

    // check if there is a object in front of the robot
    if (millis() - pingSensorMillis > 500)
    {
        turnAroundIfObjectDetected();
        pingSensorMillis = millis();
    }

    // check the line sensors
    checkLineSensor();
    correctLinePosition();
    // check if the robot is on the finish line or if it should turn right
    finishOrTurnRight();
    correctLinePosition();
    // check if the robot is able to turn right
    turnRightIfPossible();
    correctLinePosition();
    // check if the robot should go left
    turnLeftWhenNothingDetected();
    correctLinePosition();
}

/**
 * This function checks if the robot is already initialized, if not it checks if the robot is on the start line and initializes the robot
 * @return true if the robot is initialized, false if the robot is not initialized
 */
bool checkIfRobotIsInitialized()
{
    if (initialized)
        return true;
    checkLineSensor();
    if (lineSensorState[0] && lineSensorState[1] && !lineSensorState[2] && lineSensorState[3] && lineSensorState[4])
    {
        // add countdown on display here using millis and a while loop(preferably in a function to keep the code clean)
        initialized = true;
    }

    return initialized;
}

/**
 * Check the ping sensor and turn around if an object is detected
 */
void turnAroundIfObjectDetected()
{
    // give 10 microseconds pulse on trig pin to send a sound wave
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // measure duration of pulse from ECHO pin
    float duration = pulseIn(echoPin, HIGH);
    float cm = (duration / 2) / 29.1;
    // if the distance is less than 10 cm, turn around
    if (cm < 10 && cm > 0)
    {
        turnAround();
    }
}
/**
 * if the robot is on the finish line, stop the robot and set the initialized variable to false. this will allow the robot to run again when it gets placed back on the line
 */
void finish()
{
    makeCompleteStop();
    // reset the initialized variable for the next run
    initialized = false;
}

/**
 * this function checks if the robot is on the finish line. if it is not, it turns right
 * if the robot is on the finish line, it runs the finish function
 */
void finishOrTurnRight()
{
    if (!lineSensorState[0] && !lineSensorState[1] && !lineSensorState[2] && !lineSensorState[3] && !lineSensorState[4])
    {
        unsigned long startMillis = millis();
        // let the robot drive for 280 ms
        while (millis() - startMillis < 280)
        {
            checkLineSensor();
            // if the robot detects it is not on the finish line, turn right
            if (lineSensorState[0] && lineSensorState[4])
            {
                turnTillLineFound(true);
                return;
            }
        }
        // if the robot still detects black on all sensors, it is on the finish line
        finish();
    }
}

/**
 * check if the robot is able to turn right, if it is, turn right
 */
void turnRightIfPossible()
{
    if (!lineSensorState[4])
    {
        setRightMotorSpeed(0);
        while (!lineSensorState[3] && !lineSensorState[4])
        {
            checkLineSensor();
        }
        setRightMotorSpeed(defaultSpeed);
    }
}

/**
 * turn the robot left or right till the line is found
 * @param direction true for right, false for left
 */
void turnTillLineFound(bool direction)
{
    makeCompleteStop();
    (direction) ? setRightMotorDirection(false) : setLeftMotorDirection(false);
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
    while (lineSensorState[2])
    {
        checkLineSensor();
    }
    makeCompleteStop();

    (direction) ? setRightMotorDirection(true) : setLeftMotorDirection(true);
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
}
/**
 * turn the robot 180 degrees
 */
void turnAround()
{
    makeCompleteStop();
    setRightMotorDirection(false);
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
    while (!lineSensorState[2])
    {
        checkLineSensor();
    }
    while (lineSensorState[2])
    {
        checkLineSensor();
    }
    makeCompleteStop();
    setRightMotorDirection(true);
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
}
/**
 * turn left if the robots sensors detect no line
 */
void turnLeftWhenNothingDetected()
{
    if (lineSensorState[0] && lineSensorState[1] && lineSensorState[2] && lineSensorState[3] && lineSensorState[4])
    {
        turnTillLineFound(false);
    }
}

/**
 * stop the motors
 */
void makeCompleteStop()
{
    setLeftMotorSpeed(0);
    setRightMotorSpeed(0);
}

/**
 * check the line sensors and set the data in the lineSensorState array
 */
void checkLineSensor()
{
    for (int i = 0; i < lineSensorCount; i++)
    {
        lineSensorState[i] = digitalRead(lineSensorPins[i]);
    }
}

/**
 * correct the line position if the robot is too far to the left or right
 */
void correctLinePosition()
{
    checkLineSensor();
    int tooFarRight = lineSensorState[0] + lineSensorState[1];
    int tooFarLeft = lineSensorState[3] + lineSensorState[4];
    if (tooFarRight == 1)
    {
        setLeftMotorSpeed(0);
        return;
    }

    if (tooFarLeft == 1)
    {
        setRightMotorSpeed(0);
        return;
    }
}

/**
 *   set the direction of the right motor
 *   @param direction true = forward, false = backward
 */
void setRightMotorDirection(bool direction)
{
    if (direction)
    {
        digitalWrite(rightMotorDirection, HIGH);
        return;
    }
    digitalWrite(rightMotorDirection, LOW);
}

/**
 *   set the direction of the left motor
 *   @param direction true = forward, false = backward
 */
void setLeftMotorDirection(bool direction)
{
    if (direction)
    {
        digitalWrite(leftMotorDirection, LOW);
        return;
    }
    digitalWrite(leftMotorDirection, HIGH);
}

/**
 *  set the speed of the left motor
 * @param speed 0 - 255
 */
void setLeftMotorSpeed(int speed)
{
    analogWrite(leftMotorSpeed, speed);
}

/**
 *  set the speed of the right motor
 * @param speed 0 - 255
 */
void setRightMotorSpeed(int speed)
{
    analogWrite(rightMotorSpeed, speed);
}
