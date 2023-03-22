bool debug = false;
// left motor
#define leftMotorDirection 13
#define leftMotorSpeed 11
#define leftMotorBrake 8

// right motor
#define rightMotorDirection 12
#define rightMotorSpeed 3
#define rightMotorBrake 9
// default values for motor
#define defaultSpeed 65
// pingsensor
const int trigPin = A4;
const int echoPin = A5;

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
    // setup ping sensor
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    // engage the brakes to prevent the robot from running away;
    setLeftBrake(true);
    setRightBrake(true);

    // start the motors and go forward
    setRightMotorDirection(true);
    setLeftMotorDirection(true);
    setLeftBrake(false);
    setRightBrake(false);
}
bool initialized = false;
void loop()
{
    if (!initialized)
    {
        checkLineSensor();
        if (lineSensorState[0] && lineSensorState[1] && !lineSensorState[2] && lineSensorState[3] && lineSensorState[4])
        {
            initialized = true;
        }
        return;
    }
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
    turnAroundIfObjectDetected();
    checkLineSensor();
    turnRightIfPossible();
    turnAroundAtDeadEnd();
    checkLineSensor();
    correctLinePosition();
}

void turnAroundIfObjectDetected()
{
    // give 10 microseconds pulse on trig pin to send a sound wave
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // measure duration of pulse from ECHO pin
    float duration_us = pulseIn(echoPin, HIGH);
    // calculate the distance
    float distance_cm = 0.017 * duration_us;
    if (debug)
    {
        Serial.print("distance: ");
        Serial.print(distance_cm);
        Serial.println(" cm");
    }
    if (distance_cm < 10 && distance_cm > 0)
    {
        makeCompleteStop();
    }
}

void doFinish()
{
    makeCompleteStop();
    // wait for 10 seconds using millis
    unsigned long startMillis = millis();
    // reset the initialized variable for the next run
    initialized = false;
    while (millis() - startMillis < 10000)
    {
        if (debug)
            Serial.println("finished");
    }
}

void turnRightIfPossible()
{
    if (!lineSensorState[0] && !lineSensorState[1] && !lineSensorState[2] && !lineSensorState[3] && !lineSensorState[4])
    {
        unsigned long startMillis = millis();
        while (millis() - startMillis < 250)
        {
            checkLineSensor();
            if (lineSensorState[0] && lineSensorState[1] && lineSensorState[2] && lineSensorState[3] && lineSensorState[4])
            {
                makeCompleteStop();
                setRightMotorDirection(false);
                setLeftMotorSpeed(defaultSpeed);
                setRightMotorSpeed(defaultSpeed);
                while (lineSensorState[2])
                {
                    checkLineSensor();
                }
                makeCompleteStop();

                setRightMotorDirection(true);
                setLeftMotorSpeed(defaultSpeed);
                setRightMotorSpeed(defaultSpeed);
                return;
            }
        }
        // finished i think
        doFinish();
    }
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
void turnLeftTillLineFound()
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

void turnAroundAtDeadEnd()
{
    if (lineSensorState[0] && lineSensorState[1] && lineSensorState[2] && lineSensorState[3] && lineSensorState[4])
    {
        turnLeftTillLineFound();
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
    // release the brakes
    setLeftBrake(false);
    setRightBrake(false);
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
