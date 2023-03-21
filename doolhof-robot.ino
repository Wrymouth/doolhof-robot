bool debug = true;
// left motor
#define leftMotorDirection 13
#define leftMotorSpeed 11
#define leftMotorBrake 8

// right motor
#define rightMotorDirection 12
#define rightMotorSpeed 3
#define rightMotorBrake 9
// default values for motor
#define defaultSpeed 100

// linesensor
int lineSensorPins[] = {4, 5, 6, 7, 10};
int lineSensorCount = sizeof lineSensorPins / sizeof lineSensorPins[0];
bool lineSensorState[] = {false, false, false, false, false};

void setup()
{
    if (debug)
        Serial.begin(9600);
    // setup the line sensors
    for (int i = 0; i < lineSensorCount; i++)
    {
        pinMode(lineSensorPins[i], INPUT);
    }

    // setup left motor
    pinMode(leftMotorDirection, OUTPUT);
    pinMode(leftMotorSpeed, OUTPUT);
    pinMode(leftMotorBrake, OUTPUT);

    // setup right motor
    pinMode(rightMotorDirection, OUTPUT);
    pinMode(rightMotorSpeed, OUTPUT);
    pinMode(rightMotorBrake, OUTPUT);
    // engage the brakes to prevent the robot from running away;
    setLeftBrake(true);
    setRightBrake(true);

    // start the motors and go forward
    setRightMotorDirection(true);
    setLeftMotorDirection(true);
    setLeftBrake(false);
    setRightBrake(false);
    // setLeftMotorSpeed(defaultSpeed);
    // setRightMotorSpeed(defaultSpeed);
}

void loop()
{
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
    checkLineSensor();
    turnRightIfPossible();
    turnAroundAtDeadEnd();
    checkLineSensor();
    correctLinePosition();
}

void turnRightIfPossible()
{
    if (!lineSensorState[4])
    {
        setRightMotorSpeed(0);
        setRightBrake(true);
        while (!lineSensorState[3] && !lineSensorState[4])
        {
            checkLineSensor();
        }
        setRightBrake(false);
        setRightMotorSpeed(defaultSpeed);
    }
}

void turnAroundAtDeadEnd()
{
    if (lineSensorState[0] && lineSensorState[1] && lineSensorState[2] && lineSensorState[3] && lineSensorState[4])
    {
        makeCompleteStop();
        setLeftMotorDirection(false);
        setLeftMotorSpeed(defaultSpeed);
        setRightMotorSpeed(defaultSpeed);
        while (lineSensorState[2])
        {
            checkLineSensor();
        }
        makeCompleteStop();

        setLeftMotorDirection(true);
        setLeftMotorSpeed(defaultSpeed);
        setRightMotorSpeed(defaultSpeed);
    }
}

void makeCompleteStop()
{
    // stop the motors
    setLeftMotorSpeed(0);
    setRightMotorSpeed(0);
    // engage the brakes
    setLeftBrake(true);
    setRightBrake(true);
    // wait for a while using millis
    // unsigned long startMillis = millis();
    // while (millis() - startMillis < 200)
    // {
    setLeftBrake(false);
    setRightBrake(false);
    // }
}

void checkLineSensor()
{
    for (int i = 0; i < lineSensorCount; i++)
    {
        lineSensorState[i] = digitalRead(lineSensorPins[i]);
    }
}

void correctLinePosition()
{
    // setLeftMotorSpeed(defaultSpeed);
    // setRightMotorSpeed(defaultSpeed);
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
 *  set the brake of the left motor
 * @param brake true = engage brake, false = release brake
 */
void setLeftBrake(bool brake)
{
    if (brake)
    {
        digitalWrite(leftMotorBrake, HIGH);
        return;
    }
    digitalWrite(leftMotorBrake, LOW);
}

/**
 *  set the brake of the right motor
 * @param brake true = engage brake, false = release brake
 */
void setRightBrake(bool brake)
{
    if (brake)
    {
        digitalWrite(rightMotorBrake, HIGH);
        return;
    }
    digitalWrite(rightMotorBrake, LOW);
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
