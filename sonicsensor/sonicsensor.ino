//pingsensor
const int TrigPin = A4;
const int EchoPin = A5;

void setup() {
  // serial start
  Serial.begin(9600);
  // setup ping sensor
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}

void pingsensor() {
  // give 10 microseconds pulse on trig pin to send a sound wave
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  // measure duration of pulse from ECHO pin
  float duration_us = pulseIn(EchoPin, HIGH);
  // calculate the distance
  float distance_cm = 0.017 * duration_us;
  if (distance_cm < 10) {}
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
}

void loop() {
  pingsensor();
}
