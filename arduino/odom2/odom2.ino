#include <Timer.h>
#include <ros.h>
#include <std_msgs/String.h>


#define PERIOD 1000
#define PULSES_PER_ROTATION 2460

#define encoder_a 2 // Green - pin 2 - Digital
#define encoder_b 3 // White - pin 3 - Digital

long counter = 0;
long lastReading = 0;
double rpm = 0 ;
double velocity = 0;
std_msgs::String v;
char vData[20] = "";

Timer myTimer;
ros::Publisher vel("Velocity2", & v);
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
  String d =String(velocity)+"c"+String(counter);
  d.toCharArray( vData, 20);
    //Serial.println(d);
  v.data = vData;
  vel.publish(&v);
  nh.spinOnce();
  //Serial.println(vData);
  delay(150);

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
  counter += digitalRead(encoder_a) == digitalRead(encoder_b) ? -1 : 1;
}

void encoderPinChangeB()
{
  counter += digitalRead(encoder_a) != digitalRead(encoder_b) ? -1 : 1;
}
