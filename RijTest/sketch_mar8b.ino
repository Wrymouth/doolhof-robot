int directionPinRight = 12;
int pwmPinRight = 3;
int brakePinRight = 9;
//uncomment if using channel B, and remove above definitions
int directionPinLeft = 13;
int pwmPinLeft = 11;
int brakePinLeft = 8;
//boolean to switch direction
bool directionState;
void setup() {
  //define pins
  pinMode(directionPinRight, OUTPUT);
  pinMode(pwmPinRight, OUTPUT);
  pinMode(brakePinRight, OUTPUT);
  pinMode(directionPinLeft, OUTPUT);
  pinMode(pwmPinLeft, OUTPUT);
  pinMode(brakePinLeft, OUTPUT);
}

void brake() {
  digitalWrite(brakePinRight, HIGH);
  digitalWrite(brakePinLeft, HIGH);
}

void releaseBrakes() {
  digitalWrite(brakePinRight, LOW);
  digitalWrite(brakePinLeft, LOW);
}

void loop() {
  //change direction every loop()
  directionState = !directionState;
  //write a low state to the direction pin (13)
  if (directionState == false) {
    digitalWrite(directionPinRight, LOW);
    digitalWrite(directionPinLeft, HIGH);
  }
  //write a high state to the direction pin (13)
  else {
    digitalWrite(directionPinRight, HIGH);
    digitalWrite(directionPinLeft, LOW);
  }
  //release breaks
  releaseBreaks();
  //set work duty for the motor
  analogWrite(pwmPinRight, 90);
  analogWrite(pwmPinLeft, 90);
  delay(500);
  //activate breaks
  brake();
  //set work duty for the motor to 0 (off)
  analogWrite(pwmPinRight, 0);
  analogWrite(pwmPinLeft, 0);
  delay(500);
}