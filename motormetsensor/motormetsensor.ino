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

// Left electromotor
#define motorLeftDirection 13 // righting motor links
#define leftSpeed 11  // pwm linker motor
#define brakeLeft 8   // rem linker motor

// Right electromotor
#define motorRightDirection 12 // righting motor rechts
#define rightSpeed 3   // pwm rechter motor
#define brakeRight 9   // rem recher motor

// Speed
#define Speed 100 // standaart snelheid motor

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
    pinMode(motorLeftDirection, OUTPUT);
    pinMode(leftSpeed, OUTPUT);
    pinMode(brakeLeft, OUTPUT);

    pinMode(motorRightDirection, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(brakeRight, OUTPUT);
    //run brake function to prevent robot from leaving us
    brake();

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

void left(){
  digitalWrite(rmotorRight, HIGH);
  digitalWrite(rmotorLeft, LOW);
  digitalWrite(brakeLeft, HIGH);
  digitalWrite(brakeRight, LOW);
  analogWrite(rightSpeed, 0);
  analogWrite(leftSpeed, Speed);
}
// rechts
void right(){
  digitalWrite(rmotorRight, HIGH);
  digitalWrite(rmotorLeft, LOW);
  digitalWrite(brakeLeft, LOW);
  digitalWrite(brakeRight, HIGH);
  analogWrite(rightSpeed, Speed);
  analogWrite(leftSpeed, 0);
}
//rechtdoor

void straight(){
  digitalWrite(rmotorRight, HIGH);
  digitalWrite(rmotorLeft, LOW);
  digitalWrite(brakeLeft, LOW);
  digitalWrite(brakeRight, LOW);
  analogWrite(rightSpeed, Speed);
  analogWrite(leftSpeed, Speed);
}

//brake
void brake(){
  digitalWrite(rmotorRight, HIGH);
  digitalWrite(rmotorLeft, HIGH);
  digitalWrite(brakeLeft, HIGH);
  digitalWrite(brakeRight, HIGH);
  analogWrite(rightSpeed, 0);
  analogWrite(leftSpeed, 0);
}

void correctError() {
    // if the second sensor is on the line, we are too far to the left and need to correct
    if (currentLineSensorState[0] + currentLineSensorState[1] == 1) {
        left();
    } else if (currentLineSensorState[3] + currentLineSensorState[4] == 1) {
        right();
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
    }
}