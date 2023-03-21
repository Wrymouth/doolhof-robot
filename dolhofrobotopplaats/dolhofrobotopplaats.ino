// linker motor
#define rmotorLeft 13  // righting motor links
#define leftSpeed 11   // pwm linker motor
#define brakeLeft 8    // rem linker motor
// #define rotateLeft A1 // analoog sensor rotatie links

// rechter motor
#define rmotorRight 12  // righting motor rechts
#define rightSpeed 3    // pwm rechter motor
#define brakeRight 9    // rem recher motor
// #define rotateRight A0 // analoog sensor rotatie rechts

// Speed
#define Speed 100  // standaart snelheid motor

const int driveTime = 1000;
unsigned long previousDriveTime = 0;
const int leftTime = 200;
unsigned long previousLeftTime = 0;
const int rightTime = 200;
unsigned long previousRightTime = 0;
const int turnTime = 400;
unsigned long previousTurnTime = 0;
const int brakeTime = 2000;
unsigned long previousBrakeTime = 0;

unsigned long currentMillis;

enum Action {
    MOVE_STRAIGHT,
    MOVE_LEFT,
    MOVE_RIGHT,
    TURN_AROUND,
    FINISH,
    ERROR
};

int currentAction = MOVE_STRAIGHT;
string actionStrings[] = {
    "MOVE_STRAIGHT",
    "MOVE_LEFT",
    "MOVE_RIGHT",
    "TURN_AROUND",
    "FINISH",
    "ERROR"};

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    // linker motor
    pinMode(rmotorLeft, OUTPUT);
    pinMode(leftSpeed, OUTPUT);
    pinMode(brakeLeft, OUTPUT);

    // rechter motor
    pinMode(rmotorRight, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(brakeRight, OUTPUT);
    brake();
}

void loop() {
    currentMillis = millis();
    doThing(currentAction);
    currentAction++;
    if (currentAction > ERROR) {
        currentAction = MOVE_STRAIGHT;
    }
    Serial.println(actionStrings[currentAction]);
}

void performAction(Action action) {
    switch (action) {
        case MOVE_STRAIGHT:
            straght();
            break;
        case MOVE_LEFT:
            left();
            break;
        case MOVE_RIGHT:
            right();
            break;
        case TURN_AROUND:
            rightonplace();
            break;
        case FINISH:
            leftonplace();
            break;
        case ERROR:
            brake();
            break;
    }
}

// links
void left() {
    digitalWrite(rmotorRight, HIGH);
    digitalWrite(rmotorLeft, LOW);
    digitalWrite(brakeLeft, HIGH);
    digitalWrite(brakeRight, LOW);
    analogWrite(rightSpeed, 0);
    analogWrite(leftSpeed, Speed);
}
// rechts
void right() {
    digitalWrite(rmotorRight, HIGH);
    digitalWrite(rmotorLeft, LOW);
    digitalWrite(brakeLeft, LOW);
    digitalWrite(brakeRight, HIGH);
    analogWrite(rightSpeed, Speed);
    analogWrite(leftSpeed, 0);
}

// rechtdoor
void straght() {
    digitalWrite(rmotorRight, HIGH);
    digitalWrite(rmotorLeft, LOW);
    digitalWrite(brakeLeft, LOW);
    digitalWrite(brakeRight, LOW);
    analogWrite(rightSpeed, Speed);
    analogWrite(leftSpeed, Speed);
}

// brake
void brake() {
    digitalWrite(rmotorRight, HIGH);
    digitalWrite(rmotorLeft, HIGH);
    digitalWrite(brakeLeft, HIGH);
    digitalWrite(brakeRight, HIGH);
    analogWrite(rightSpeed, 0);
    analogWrite(leftSpeed, 0);
}
// recht op plaats
void rightonplace() {
    if (currentMillis - previousRightTime < rightTime) {
        digitalWrite(rmotorRight, HIGH);
        digitalWrite(rmotorLeft, HIGH);
        digitalWrite(brakeLeft, LOW);
        digitalWrite(brakeRight, HIGH);
        analogWrite(rightSpeed, Speed);
        analogWrite(leftSpeed, Speed);
    }
}

// links op plaats
void leftonplace() {
    if (currentMillis - previousLeftTime < leftTime) {
        digitalWrite(rmotorRight, LOW);
        digitalWrite(rmotorLeft, LOW);
        digitalWrite(brakeLeft, LOW);
        digitalWrite(brakeRight, HIGH);
        analogWrite(rightSpeed, Speed);
        analogWrite(leftSpeed, Speed);
    }
}