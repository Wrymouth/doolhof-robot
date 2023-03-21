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

enum Action {
    MOVE_STRAIGHT,
    MOVE_LEFT,
    MOVE_RIGHT,
    TURN_AROUND,
    FINISH,
    ERROR
};

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
    int previous = previousLineSensorState[1] * 1 + previousLineSensorState[3] * 2;
    int current = currentLineSensorState[1] * 1 + currentLineSensorState[3] * 2;
    Action action = ERROR;

    switch (previous * 10 + current) {
        case 22: // prev straight, cur straight
            action = MOVE_STRAIGHT;
            break;
        case 20: // prev straight, cur none
            action = TURN_AROUND;
            break;
        case 39: // prev all, cur straight
        case 37: // prev left, cur straight
            action = MOVE_RIGHT;
            break;
        case 31: // prev right, cur straight
            action = MOVE_LEFT;
            break;
        case 33: // prev all, cur none
            action = MOVE_RIGHT;
            break;
        case 0: // prev all, cur all
            action = FINISH;
            break;
        default:
            action = ERROR;
            break;
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