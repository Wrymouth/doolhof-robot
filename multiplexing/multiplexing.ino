

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
    // countRunTime();

    blinkDisplayFinish();
    while (true)
    {
        switchDisplay();
        displayFi();
    }
    // countdown();
    // displayDigits(13);
    // displayFi();
    // displaySt();
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