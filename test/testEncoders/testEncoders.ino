#include <ros.h>
#include <mia_msgs/Imu.h>
#include <Timer.h>

#define PERIOD 1
#define PULSES_PER_ROTATION 2460

#define encoder_a 18 // Green - pin 2 - Digital
#define encoder_b 19 // White - pin 3 - Digital

long counter = 0;
long lastReading = 0;
double rpm = 0 ;


Timer myTimer;


void setup()
{
  //myTimer.every(PERIOD, calculateRPM);
  Serial.begin(9600);
  
  pinMode(encoder_a, INPUT_PULLUP);
  pinMode(encoder_b, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder_a), encoderPinChangeA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_b), encoderPinChangeB, CHANGE);

  pinMode(22,OUTPUT);
  pinMode(24,OUTPUT);

  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);

  digitalWrite(22,HIGH);
  digitalWrite(24,LOW);
  analogWrite(12,80);
  analogWrite(11,80);

}

void loop()
{
  myTimer.update();
  Serial.println(counter);

  
}

void calculateRPM()
{
  float rotationPerSample;
  noInterrupts();
  rpm = (counter - lastReading)* 1.0 /PERIOD;
  lastReading = counter;
  interrupts();
}

void encoderPinChangeA()
{
  counter += digitalRead(encoder_a) == digitalRead(encoder_b) ? -1 : 1;
}

void encoderPinChangeB()
{
  counter += digitalRead(encoder_a) != digitalRead(encoder_b) ? -1 : 1;
}
