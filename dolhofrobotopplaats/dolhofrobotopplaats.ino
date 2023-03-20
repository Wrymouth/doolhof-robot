 // linker motor
#define rmotorLeft 13 // righting motor links
#define leftSpeed 11 // pwm linker motor
#define brakeLeft 8 // rem linker motor 
//#define rotateLeft A1 // analoog sensor rotatie links

// rechter motor 
#define rmotorRight 12 // righting motor rechts 
#define rightSpeed 3 // pwm rechter motor
#define brakeRight 9 // rem recher motor
//#define rotateRight A0 // analoog sensor rotatie rechts

//Speed
#define Speed 100 // standaart snelheid motor 


void setup() {
  // put your setup code here, to run once:

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
  // put your main code here, to run repeatedly:
}
// links
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
void straght(){
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
// recht op plaats
void rightonplace(){
  digitalWrite(rmotorRight, HIGH);
  digitalWrite(rmotorLeft, HIGH);
  digitalWrite(brakeLeft, LOW);
  digitalWrite(brakeRight, HIGH);
  analogWrite(rightSpeed, Speed);
  analogWrite(leftSpeed, Speed);
}

// links op plaats 
void leftonplace(){
  digitalWrite(rmotorRight, LOW);
  digitalWrite(rmotorLeft, LOW);
  digitalWrite(brakeLeft, LOW);
  digitalWrite(brakeRight, HIGH);
  analogWrite(rightSpeed, Speed);
  analogWrite(leftSpeed, Speed);
}