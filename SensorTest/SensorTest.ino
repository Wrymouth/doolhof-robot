#define LS1 4
#define LS2 5
#define LS3 6
#define LS4 7
#define LS5 10

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(LS1, INPUT);
  pinMode(LS2, INPUT);
  pinMode(LS3, INPUT);
  pinMode(LS4, INPUT);
  pinMode(LS5, INPUT);
}

void loop() {
  Serial.print(digitalRead(LS1));
  Serial.print(digitalRead(LS2));
  Serial.print(digitalRead(LS3));
  Serial.print(digitalRead(LS4));
  Serial.print(digitalRead(LS5));
  Serial.println();
  delay(1000);
  // put your main code here, to run repeatedly:

}
