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
#define defaultSpeed 60

// Timers
unsigned long currentMillis;

// linesensor
int lineSensorPins[] = {4, 5, 6, 7, 10};
int lineSensorCount = sizeof lineSensorPins / sizeof lineSensorPins[0];
bool LineSensorState[] = {false, false, false, false, false};
// bool previousLineSensorState[] = {false, false, false, false, false};

// list of all actions
enum Action
{
    MOVE_STRAIGHT,
    MOVE_LEFT,
    MOVE_RIGHT,
    TURN_AROUND,
    FINISH,
    ERROR
};
// set current action to first action as default value
// list of all actions as strings for debugging

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
    setRightmoterDirection(true);
    setLeftmoterDirection(true);
    setLeftBrake(false);
    setRightBrake(false);
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
}

void loop()
{
    // currentMillis = millis();
    CheckLineSensor();
    correctLinePosition();
}

void CheckLineSensor()
{
    // for (int i = 0; i < lineSensorCount; i++)
    // {
    //     previousLineSensorState[i] = currentLineSensorState[i];
    // }
    for (int i = 0; i < lineSensorCount; i++)
    {
        LineSensorState[i] = digitalRead(lineSensorPins[i]);
    }
}

void correctLinePosition()
{
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
    int tooFarRight = LineSensorState[0] + LineSensorState[1];
    int tooFarLeft = LineSensorState[3] + LineSensorState[4];
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
void setRightmoterDirection(bool direction)
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
void setLeftmoterDirection(bool direction)
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
