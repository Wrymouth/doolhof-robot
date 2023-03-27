const int SEVEN_SEGMENT_LEDS[] = {4, 5, 6, 7, 8, 9, 10};
const int SEGMENT_ARRAY_SIZE = sizeof(SEVEN_SEGMENT_LEDS) / sizeof(int);
const int LETTER_COUNT = 4;
const int NUMBER_COUNT = 10;

const int LETTERS[LETTER_COUNT][SEGMENT_ARRAY_SIZE] = {
    {HIGH, LOW, HIGH, HIGH, HIGH, LOW, HIGH},  // dit is de S
    {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH},   // dit is de T
    {HIGH, LOW, LOW, LOW, HIGH, HIGH, HIGH},   // dit is de F
    {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW},     // dit is de I
};

const int NUMBERS[NUMBER_COUNT][SEGMENT_ARRAY_SIZE] = {
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},   // 0
    {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW},       // 1
    {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH},    // 2
    {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH},    // 3
    {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH},     // 4
    {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH},    // 5
    {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH},   // 6
    {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW},      // 7
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH},  // 8
    {HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH},   // 9
};

int currentNumber = 10;
bool leftDisplay = true;
unsigned long lastSwitchTime = 0;

// left motor
#define leftMotorDirection 13
#define leftMotorSpeed 11
// right motor
#define rightMotorDirection 12
#define rightMotorSpeed 3
// default values for motor
#define adapterDefaultSpeed 65
#define batteryDefaultSpeed 80
const bool onBattery = true;
const int defaultSpeed = (onBattery) ? batteryDefaultSpeed : adapterDefaultSpeed;

// 7 segment display
#define u1 1
#define u2 2

// pingsensor
const int trigPin = 0;
const int echoPin = A5;
// linesensor
const int lineSensorPins[] = {A0, A1, A2, A3, A4};
const int lineSensorCount = sizeof lineSensorPins / sizeof lineSensorPins[0];
bool lineSensorState[] = {false, false, false, false, false};

// value to determine if the robot should run
bool initialized = false;

void setup() {
    // setup the line sensors
    for (int i = 0; i < lineSensorCount; i++) {
        pinMode(lineSensorPins[i], INPUT);
    }
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++) {
        pinMode(SEVEN_SEGMENT_LEDS[i], OUTPUT);
    }
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

void loop() {
    digitalWrite(4,HIGH);
    delay(100);
    // switchDisplay();
    // displaySt();
    // if (!checkIfRobotIsInitialized())
    //     return;
    // setLeftMotorSpeed(defaultSpeed);
    // setRightMotorSpeed(defaultSpeed);
    // // turnAroundIfObjectDetected();

    // checkLineSensor();
    // correctLinePosition();

    // finishOrTurnRight();
    // correctLinePosition();

    // turnRightIfPossible();
    // correctLinePosition();

    // turnLeftWhenNothingDetected();
    // correctLinePosition();
}

/**
 * This function checks if the robot is already initialized, if not it checks if the robot is on the start line and initializes the robot
 * @return true if the robot is initialized, false if the robot is not initialized
 */
bool checkIfRobotIsInitialized() {
    if (initialized)
        return true;
    checkLineSensor();
    if (lineSensorState[0] && lineSensorState[1] && !lineSensorState[2] && lineSensorState[3] && lineSensorState[4]) {
        // add countdown on display here using millis and a while loop(preferably in a function to keep the code clean)
        initialized = true;
    }

    return initialized;
}

/**
 * Check the ping sensor and turn around if an object is detected
 * Todo: This function is not working for some weird reason, even though the sensor seems to be working fine
 */
void turnAroundIfObjectDetected() {
    // give 10 microseconds pulse on trig pin to send a sound wave
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // measure duration of pulse from ECHO pin
    float duration_us = pulseIn(echoPin, HIGH);
    // calculate the distance
    float distance_cm = 0.017 * duration_us;
    // if the distance is less than 10 cm, turn around
    if (distance_cm < 10 && distance_cm > 0) {
        turnTillLineFound(true);
    }
}
/**
 * if the robot is on the finish line, stop the robot and set the initialized variable to false. this will allow the robot to run again when it gets placed back on the line
 */
void finish() {
    makeCompleteStop();
    // wait for 10 seconds using millis
    unsigned long startMillis = millis();
    // reset the initialized variable for the next run
    initialized = false;
}

/**
 * this function checks if the robot is on the finish line. if it is not, it turns right
 * if the robot is on the finish line, it runs the finish function
 */
void finishOrTurnRight() {
    if (!lineSensorState[0] && !lineSensorState[1] && !lineSensorState[2] && !lineSensorState[3] && !lineSensorState[4]) {
        unsigned long startMillis = millis();
        // let the robot drive for 280 ms
        while (millis() - startMillis < 280) {
            checkLineSensor();
            // if the robot detects it is not on the finish line, turn right
            if (lineSensorState[0] && lineSensorState[4]) {
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
void turnRightIfPossible() {
    if (!lineSensorState[4]) {
        setRightMotorSpeed(0);
        while (!lineSensorState[3] && !lineSensorState[4]) {
            checkLineSensor();
        }
        setRightMotorSpeed(defaultSpeed);
    }
}

/**
 * turn the robot left or right till the line is found
 * @param direction true for right, false for left
 */
void turnTillLineFound(bool direction) {
    makeCompleteStop();
    (direction) ? setRightMotorDirection(false) : setLeftMotorDirection(false);
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
    while (lineSensorState[2]) {
        checkLineSensor();
    }
    makeCompleteStop();

    (direction) ? setRightMotorDirection(true) : setLeftMotorDirection(true);
    setLeftMotorSpeed(defaultSpeed);
    setRightMotorSpeed(defaultSpeed);
}

/**
 * turn left if the robots sensors detect no line
 */
void turnLeftWhenNothingDetected() {
    if (lineSensorState[0] && lineSensorState[1] && lineSensorState[2] && lineSensorState[3] && lineSensorState[4]) {
        turnTillLineFound(false);
    }
}

/**
 * stop the motors
 */
void makeCompleteStop() {
    setLeftMotorSpeed(0);
    setRightMotorSpeed(0);
}

/**
 * check the line sensors and set the data in the lineSensorState array
 */
void checkLineSensor() {
    for (int i = 0; i < lineSensorCount; i++) {
        lineSensorState[i] = digitalRead(lineSensorPins[i]);
    }
}

/**
 * correct the line position if the robot is too far to the left or right
 */
void correctLinePosition() {
    checkLineSensor();
    int tooFarRight = lineSensorState[0] + lineSensorState[1];
    int tooFarLeft = lineSensorState[3] + lineSensorState[4];
    if (tooFarRight == 1) {
        setLeftMotorSpeed(0);
        return;
    }

    if (tooFarLeft == 1) {
        setRightMotorSpeed(0);
        return;
    }
}

/**
 *   set the direction of the right motor
 *   @param direction true = forward, false = backward
 */
void setRightMotorDirection(bool direction) {
    if (direction) {
        digitalWrite(rightMotorDirection, HIGH);
        return;
    }
    digitalWrite(rightMotorDirection, LOW);
}

/**
 *   set the direction of the left motor
 *   @param direction true = forward, false = backward
 */
void setLeftMotorDirection(bool direction) {
    if (direction) {
        digitalWrite(leftMotorDirection, LOW);
        return;
    }
    digitalWrite(leftMotorDirection, HIGH);
}

/**
 *  set the speed of the left motor
 * @param speed 0 - 255
 */
void setLeftMotorSpeed(int speed) {
    analogWrite(leftMotorSpeed, speed);
}

/**
 *  set the speed of the right motor
 * @param speed 0 - 255
 */
void setRightMotorSpeed(int speed) {
    analogWrite(rightMotorSpeed, speed);
}

void displaySt() {
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++) {
        if (leftDisplay) {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], LETTERS[0][i]);
        } else {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], LETTERS[1][i]);
        }
    }
}

void displayFi() {
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++) {
        if (leftDisplay) {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], LETTERS[2][i]);
        } else {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], LETTERS[3][i]);
        }
    }
}

void displayDigits(int number) {
    // tostring
    String numberString = String(number);
    int digit1 = int(numberString.charAt(0));
    int digit2 = int(numberString.charAt(1));
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++) {
        if (leftDisplay) {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], NUMBERS[digit1][i]);
        } else {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], NUMBERS[digit2][i]);
        }
    }
}

void switchDisplay() {
    if (millis() - lastSwitchTime > 10) {
        lastSwitchTime = millis();
        digitalWrite(u1, leftDisplay);
        digitalWrite(u2, !leftDisplay);
        leftDisplay = !leftDisplay;
    }
}
