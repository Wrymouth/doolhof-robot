// 7 segment display
const int SEVEN_SEGMENT_LEDS[] = {4, 5, 6, 7, 8, 9, 10};
const int SEGMENT_ARRAY_SIZE = sizeof(SEVEN_SEGMENT_LEDS) / sizeof(int);
const int LETTER_COUNT = 4;
const int NUMBER_COUNT = 10;
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

bool targetLeftDisplay = true;
unsigned long lastSwitchTime = 0;

// 7 segment display
#define u1 1
#define u2 2

void setup()
{
    // setup the 7 segment display
    for (int i = 0; i < SEGMENT_ARRAY_SIZE; i++)
    {
        pinMode(SEVEN_SEGMENT_LEDS[i], OUTPUT);
    }
    pinMode(u1, OUTPUT);
    pinMode(u2, OUTPUT);
    digitalWrite(u1, HIGH);
    digitalWrite(u2, LOW);
}

void loop()
{
    switchDisplay();
    // countdown();
    // displayDigits(13);
    // displayFi();
    // displaySt();
}

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
