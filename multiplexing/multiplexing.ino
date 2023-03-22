#define LEFT 2
#define RIGHT 3
const int SEVEN_SEGMENT_LEDS[] = {4, 5, 6, 7, 8, 9, 10, 11};
const int SEGMENT_ARRAY_SIZE = sizeof(SEVEN_SEGMENT_LEDS) / sizeof(int);
const int LETTER_COUNT = 4;
const int NUMBER_COUNT = 10;
/**
 *   a
 * f   b
 *   g
 * e   c
 *   d
 */

const int LETTERS[VALUES][SEGMENT_ARRAY_SIZE] = {
    {HIGH, LOW, HIGH, HIGH, HIGH, LOW, HIGH},  // dit is de S
    {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH},   // dit is de T
    {HIGH, LOW, LOW, LOW, HIGH, HIGH, HIGH},   // dit is de F
    {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW},     // dit is de I
};

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

int currentNumber = 10;
bool leftDisplay = true;

void setup() {
    // put your setup code here, to run once:
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++) {
        pinMode(SEVEN_SEGMENT_LEDS[i], OUTPUT);
    }
}

void loop() {
    displaySt();
    switchDisplay();
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

void displayNumber(int number) {
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++) {
        if (leftDisplay) {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], NUMBERS[number][i]);
        } else {
            digitalWrite(SEVEN_SEGMENT_LEDS[i], NUMBERS[number][i]);
        }
    }

void switchDisplay() {
    if (millis() - lastSwitchTime > 10) {
        lastSwitchTime = millis();
    }
    leftDisplay = !leftDisplay;
}
