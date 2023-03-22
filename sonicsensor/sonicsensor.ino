//pingsensor
const int TRIG_PIN = A4;
const int ECHO_PIN = A5;

void setup() {
  // serial start
  Serial.begin(9600);
  // setup ping sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void pingsensor() {
  // give 10 microseconds pulse on trig pin to send a sound wave
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // measure duration of pulse from ECHO pin
  float duration_us = pulseIn(ECHO_PIN, HIGH);
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
