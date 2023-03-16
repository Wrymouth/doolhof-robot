// wit == true zwart == false
// bij verandering van meting rijd door en meet nogmaals
// algorithm priority: right > straight > left > turn around

// cur straight, prev straight = go straight (rechte weg)
// prev straight, cur none = turn around (doodlopende weg)
// prev all, cur straight = go right (drie weg kruispunt)
// prev all, cur none = go right (T kruispunt)
// prev all, cur all = finish (eindpunt)
// prev left, cur straight = go straight (kruispunt rechtdoor, linksaf)
// prev right, cur straight = go right (kruispunt rechtdoor, rechtsaf)
// prev left, cur none = go left (bocht linksaf)
// prev right, cur none = go right (bocht rechtsaf)
// prev none, cur none = error
// prev none, cur straight = go straight (beginpunt)

int lineSensorPins[5] = {4, 5, 6, 7, 10};
int lineSensorCount = sizeof lineSensorPins / sizeof lineSensorPins[0];
bool currentLineSensorState[5] = {false, false, false, false, false};
bool previousLineSensorState[5] = {false, false, false, false, false};

enum Action
{
    moveStraight,
    moveRight,
    moveLeft,
    turnAround,
    finish,
    error
};
Action action = moveStraight;

void setup()
{
    Serial.begin(9600);
    // put your setup code here, to run once:
    for (int i = 0; i < lineSensorCount; i++)
    {
        pinMode(lineSensorPins[i], INPUT);
    }
    Action action = measureAgain;
}

void loop()
{
    // decide action based on current and previous line
    CheckLineSensor();
    printLineSensorData();

    // rijden
    drive();
    // actie bepalen
    solveMaze();
    Serial.println(action);
    delay(5000);
}

void CheckLineSensor()
{
    for (int i = 0; i < lineSensorCount; i++)
    {
        previousLineSensorState[i] = currentLineSensorState[i];
    }
    for (int i = 0; i < lineSensorCount; i++)
    {
        currentLineSensorState[i] = digitalRead(lineSensorPins[i]);
    }
}

void printLineSensorData()
{
    Serial.println("Previous measured data:");
    for (int i = 0; i < 5; i++)
    {
        Serial.print(previousLineSensorState[i]);
    }
    Serial.println();
    Serial.println("Current measured data:");
    for (int i = 0; i < 5; i++)
    {
        Serial.print(currentLineSensorState[i]);
    }
    Serial.println();
    Serial.println();
}

void solveMaze()
{
    // false means road, true means wall
    if (!previousLineSensorState[2])
    { // last measurement had road in the center (2)
        // prev straight, cur straight
        if (previousLineSensorState[0] && previousLineSensorState[1] && previousLineSensorState[3] && previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && !currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            action = moveStraight;
        }
        else if (previousLineSensorState[0] && previousLineSensorState[1] && previousLineSensorState[3] && previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            // prev straight, cur none
            action = turnAround;
        }
        else if (!previousLineSensorState[0] && !previousLineSensorState[1] && !previousLineSensorState[3] && !previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && !currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            // prev all, cur straight
            action = moveRight;
        }
        else if (!previousLineSensorState[0] && !previousLineSensorState[1] && !previousLineSensorState[3] && !previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            // prev all, cur none
            action = moveRight;
        }
        else if (!previousLineSensorState[0] && !previousLineSensorState[1] && !previousLineSensorState[3] && !previousLineSensorState[4] && !currentLineSensorState[0] && !currentLineSensorState[1] && !currentLineSensorState[2] && !currentLineSensorState[3] && !currentLineSensorState[4])
        {
            // prev all, cur all
            action = finish;
        }
        else if (!previousLineSensorState[0] && !previousLineSensorState[1] && previousLineSensorState[3] && previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && !currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            // prev left, cur straight
            action = moveStraight;
        }
        else if (previousLineSensorState[0] && previousLineSensorState[1] && !previousLineSensorState[3] && !previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && !currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            // prev right, cur straight
            action = moveRight;
        }
        else if (!previousLineSensorState[0] && !previousLineSensorState[1] && previousLineSensorState[3] && previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            // prev left, cur none
            action = moveLeft;
        }
        else if (previousLineSensorState[0] && previousLineSensorState[1] && !previousLineSensorState[3] && !previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            // prev right, cur none
            action = moveRight;
        }
        else if (previousLineSensorState[0] && previousLineSensorState[1] && previousLineSensorState[3] && previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            // prev none, cur none
            action = error;
        }
        else if (previousLineSensorState[0] && previousLineSensorState[1] && previousLineSensorState[3] && previousLineSensorState[4] && currentLineSensorState[0] && currentLineSensorState[1] && !currentLineSensorState[2] && currentLineSensorState[3] && currentLineSensorState[4])
        {
            // prev none, cur straight
            action = moveStraight;
        }
        else
        {
            action = error;
        }
    } else {
        action = error;
    }
}

void doMoveStraight()
{
    // ga rechtdoor
}

void doMoveRight()
{
    // ga rechtsaf
}

void doMoveLeft()
{
    // ga linksaf
}

void drive()
{
}