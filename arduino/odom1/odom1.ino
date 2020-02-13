#define motor2 5
#define motor1 4
#define pwm1 9
#define pwm2 10
#define Speed 160

#include <Timer.h>
#include <ros.h>
#include <std_msgs/String.h>


#define PERIOD 1000
#define PULSES_PER_ROTATION 2460

#define encoder_a 2 // Green - pin 2 - Digital
#define encoder_b 3 // White - pin 3 - Digital


char d;
long counter = 0;
long lastReading = 0;
double rpm = 0 ;
double velocity = 0;
std_msgs::String v;
char vData[50] = "";

Timer myTimer;
ros::Publisher vel("Velocity", & v);
ros::NodeHandle nh;

void setup()
{
  nh.initNode();
  nh.advertise(vel);
  myTimer.every(PERIOD, calculateRPM);
  //Serial.begin(9600);
  pinMode(encoder_a, INPUT_PULLUP);
  pinMode(encoder_b, INPUT_PULLUP);
  attachInterrupt(0, encoderPinChangeA, CHANGE);
  attachInterrupt(1, encoderPinChangeB, CHANGE);
}

void loop()
{
  myTimer.update();
  String d=String(velocity);
  d.toCharArray( vData, 50);
  v.data=vData;
  vel.publish(&v);
  nh.spinOnce();
  //Serial.println(vData);
  delay(1000);

}

void calculateRPM()
{
  float rotationPerSample;
  noInterrupts();
  rpm = (counter - lastReading) * 1.0 / PERIOD;
  velocity = rpm * (2 * 3.14 * 0.07);
  lastReading = counter;
  interrupts();
  //Serial.println(counter);
  //Serial.println(rpm);
  
}

void encoderPinChangeA()
{
  counter += digitalRead(encoder_a) ==digitalRead(encoder_b) ? -1 : 1;
}

void encoderPinChangeB()
{
  counter += digitalRead(encoder_a) != digitalRead(encoder_b) ? -1 : 1;
}
void moveForward() {
  digitalWrite(motor1 , HIGH);
  digitalWrite(motor2 , HIGH);
  analogWrite(pwm1, Speed);
  analogWrite(pwm2, Speed);
  d='F';
}

void moveBackward() {
   digitalWrite(motor1 , LOW);
  digitalWrite(motor2 , LOW);
  analogWrite(pwm1, Speed);
  analogWrite(pwm2, Speed);
  d='B';
}

void moveLeft() {
  digitalWrite(motor1 , LOW);
  digitalWrite(motor2 , HIGH);
  analogWrite(pwm1, Speed);
  analogWrite(pwm2, Speed);
  d='L';
}

void moveRight() {
  digitalWrite(motor1 , HIGH);
  digitalWrite(motor2 , LOW);
  analogWrite(pwm1, Speed);
  analogWrite(pwm2, Speed);
  d='R';
}
void stopMotor() {
  //digitalWrite(motor1 , LOW);
  //digitalWrite(motor2 , LOW);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  d='S';
}
