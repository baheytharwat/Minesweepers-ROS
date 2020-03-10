#define DIR1 4
#define DIR2 7
#define DIR3 A0
#define DIR4 11

#define PWM1 5
#define PWM2 9
#define PWM3 6
#define PWM4 10

int Speed = 200;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(DIR3, OUTPUT);
  pinMode(DIR4, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

   // moveRight(Speed);
    moveForward(Speed);
}

void loop() {
  // put your main code here, to run repeatedly:


}




void moveForward(int Speed) {

  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  digitalWrite(DIR3, HIGH);
  digitalWrite(DIR4, HIGH);

  analogWrite(PWM1, Speed);
  analogWrite(PWM2, Speed);
  analogWrite(PWM3, Speed);
  analogWrite(PWM4, Speed);


  delay(10);
}

void moveBackward(int Speed) {

  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  digitalWrite(DIR3, LOW);
  digitalWrite(DIR4, LOW);

  analogWrite(PWM1, Speed);
  analogWrite(PWM2, Speed);
  analogWrite(PWM3, Speed);
  analogWrite(PWM4, Speed);


  delay(10);
}



void moveLeft(int Speed) {

  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, LOW);
  digitalWrite(DIR3, LOW);
  digitalWrite(DIR4, HIGH);

  analogWrite(PWM1, Speed);
  analogWrite(PWM2, Speed);
  analogWrite(PWM3, Speed);
  analogWrite(PWM4, Speed);


  delay(10);
}



void moveRight(int Speed) {

  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, HIGH);
  digitalWrite(DIR3, HIGH);
  digitalWrite(DIR4, LOW);

  analogWrite(PWM1, Speed);
  analogWrite(PWM2, Speed);
  analogWrite(PWM3, Speed);
  analogWrite(PWM4, Speed);


  delay(10);
}


void Stop() {


  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
  analogWrite(PWM3, 0);
  analogWrite(PWM4, 0);

  delay(10);


}
