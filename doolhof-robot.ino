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
const bool onBattery = true;
const int defaultSpeed = (onBattery) ? batteryDefaultSpeed : adapterDefaultSpeed;

// 7 segment display
const int SEVEN_SEGMENT_LEDS[] = {4, 5, 6, 7, 8, 9, 10};
const int SEGMENT_ARRAY_SIZE = sizeof(SEVEN_SEGMENT_LEDS) / sizeof(int);
const int LETTER_COUNT = 4;
const int NUMBER_COUNT = 10;
#define u1 1
#define u2 2
// letters for the 7 segment display
const int LETTERS[LETTER_COUNT][SEGMENT_ARRAY_SIZE] = {
    {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH}, // S
    {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH},  // T
    {HIGH, LOW, LOW, LOW, HIGH, HIGH, HIGH},  // F
    {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW},    // I
};
// numbers for the 7 segment display
const int NUMBERS[NUMBER_COUNT][SEGMENT_ARRAY_SIZE] = {
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},  // 0
    {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW},      // 1
    {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH},   // 2
    {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH},   // 3
    {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH},    // 4
    {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH},   // 5
    {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH},  // 6
    {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW},     // 7
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH}, // 8
    {HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH},  // 9
};
// the current display state
bool targetLeftDisplay = true;
unsigned long lastSwitchTime = 0;
// the runtime in seconds
int runTime = 0;
unsigned long lastRunTimeUpdate = millis();

// pingsensor
const int trigPin = 0;
const int echoPin = A5;
// linesensor
const int lineSensorPins[] = {A0, A1, A2, A3, A4};
const int lineSensorCount = sizeof lineSensorPins / sizeof lineSensorPins[0];
bool lineSensorState[] = {false, false, false, false, false};

// value to determine if the robot should run
bool initialized = false;
bool isFinnished = false;
unsigned long pingSensorMillis = millis();
void setup()
{
    // setup the line sensors
    for (int i = 0; i < lineSensorCount; i++)
    {
        pinMode(lineSensorPins[i], INPUT);
    }

    // setup the 7 segment display
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++)
    {
        pinMode(SEVEN_SEGMENT_LEDS[i], OUTPUT);
    }
    pinMode(u1, OUTPUT);
    pinMode(u2, OUTPUT);
    digitalWrite(u1, HIGH);
    digitalWrite(u2, LOW);

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
    switchDisplay();
    // check if the robot should run
    if (isFinnished)
    {
        displayFi();
    }

    if (!checkIfRobotIsInitialized())
        return;

    // make the robot go forward
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);

    // update the runtime
    countRunTime();

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
        countdown();
        runTime = 0;
        isFinnished = false;
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
    if (cm < 15 && cm > 0)
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
    blinkDisplayFinish();
    // reset the initialized variable for the next run
    initialized = false;
    isFinnished = true;
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

/**
 * count the runtime in seconds and update the runTime variable
 */
void countRunTime()
{
    if (millis() - lastRunTimeUpdate >= 1000)
    {
        lastRunTimeUpdate = millis();
        runTime++;
    }
    displayDigits(runTime);
}
/**
 * countdown from 10 to 1 and show it on the 7 segment display, after 1 show st.
 */
void countdown()
{
    // count down from 10 to 1 and display it on the 7 segment display every second
    int i = 10;
    long startTime = millis();
    while (i >= 1)
    {
        switchDisplay();
        displayDigits(i);

        if (millis() - startTime >= 1000)
        {
            startTime = millis();
            i--;
        }
    }
    // display the letters "ST" on the 7 segment display for 1 second
    startTime = millis();
    while (startTime + 1000 > millis())
    {
        switchDisplay();
        displaySt();
    }
}
/**
 * Display the letters "ST" on the 7 segment display
 */
void displaySt()
{
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++)
    {
        if (targetLeftDisplay)
        {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], LETTERS[0][i]);
        }
        else
        {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], LETTERS[1][i]);
        }
    }
}
/**
 * Display the letters "FI" on the 7 segment display
 */
void displayFi()
{
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++)
    {
        if (targetLeftDisplay)
        {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], LETTERS[2][i]);
        }
        else
        {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], LETTERS[3][i]);
        }
    }
}

/**
 * Display the digits on the 7 segment display
 * @param number the number to display
 */
void displayDigits(int number)
{
    // set first digit to 0 if the number is less than 10
    int digit1 = 0;
    int digit2 = number;
    // if the number is greater than 9, split it into two digits
    if (number > 9)
    {
        String numberString = String(number);
        digit1 = String(numberString.charAt(0)).toInt();
        digit2 = String(numberString.charAt(1)).toInt();
    }
    // display the digits
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++)
    {
        if (targetLeftDisplay)
        {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], NUMBERS[digit1][i]);
        }
        else
        {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], NUMBERS[digit2][i]);
        }
    }
}

/**
 * Clear the display
 */
void clearDisplay()
{
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++)
    {
        digitalWrite(SEVEN_SEGMENT_LEDS[i], LOW);
    }
}

/**
 * Switch from left to right display and vice versa. This is done to multiplex the 7 segment display.
 */
void switchDisplay()
{
    if (millis() - lastSwitchTime > 10)
    {
        lastSwitchTime = millis();
        digitalWrite(u1, targetLeftDisplay);
        digitalWrite(u2, !targetLeftDisplay);
        targetLeftDisplay = !targetLeftDisplay;
    }
}

/**
 * Blink the display 3 times and show the finish message
 */
void blinkDisplayFinish()
{
    unsigned long lastBlinkTime = millis();
    int amountOfBlinks = 0;
    bool displayFinish = true;
    while (amountOfBlinks <= 4)
    {
        switchDisplay();
        if (millis() - lastBlinkTime > 500)
        {
            displayFinish = !displayFinish;
            amountOfBlinks++;
            lastBlinkTime = millis();
        }
        if (displayFinish)
        {
            displayDigits(runTime);
        }
        else
        {
            clearDisplay();
        }
    }
}