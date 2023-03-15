int lineSensorPins[] = { 4, 5, 6, 7, 10 };
int lineSensorCount = sizeof lineSensorPins / sizeof lineSensorPins[0];
bool currentLineSensorState[] = { false, false, false, false, false };
bool previousLineSensorState[] = { false, false, false, false, false };

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (int i = 0; i < lineSensorCount; i++) {
    pinMode(lineSensorPins[i], INPUT);
  }
}

void CheckLineSensor() {
  for (int i = 0; i < lineSensorCount; i++) {
    previousLineSensorState[i] = currentLineSensorState[i];
  }
  for (int i = 0; i < lineSensorCount; i++) {
    currentLineSensorState[i] = digitalRead(lineSensorPins[i]);
  }
}

void printLineSensorData() {
  Serial.println("Previous measured data:");
  for (int i = 0; i < 5; i++) {
    Serial.print(previousLineSensorState[i]);
  }
  Serial.println();
  Serial.println("Current measured data:");
  for (int i = 0; i < 5; i++) {
    Serial.print(currentLineSensorState[i]);
  }
  Serial.println();
  Serial.println();
}

void loop() {
  CheckLineSensor();
  printLineSensorData();

  delay(5000);
  // put your main code here, to run repeatedly:
}
