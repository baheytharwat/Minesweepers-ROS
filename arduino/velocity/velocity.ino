#include <ros.h>
#include <std_msgs/String.h>
#include <Timer.h>

#define DIR1 22  //cytron1
#define DIR2 24
//
//#define DIR3 A0  //cytron2
//#define DIR4 7
#define PERIOD 50
Timer myTimer;


#define PWM1 12  //cytron1
#define PWM2 11

//#define PWM3 9   //cytron2
//#define PWM4 6

#define Speed 150

#define encoder_r_a 19
#define encoder_r_b 18

#define encoder_l_a 3
#define encoder_l_b 2
#define PULSES_PER_ROTATION 2400.0
#define WHEEL_RADIUS 0.075
#define WHEELS_SEPARATION 0.35
std_msgs::String odom_msg;

long counterR = 0;
long counterL = 0;
long lastReadingL = 0;
long lastReadingR = 0;
double Wr = 0;
double Wl = 0;
double rpmR = 0;
double rpmL = 0;
double Vr = 0.0;
double Vl = 0.0;
double Vx = 0.0;
double Vth = 0.0;

ros::NodeHandle  nh;

void messageCb( const std_msgs::String& msg) {

  if (msg.data[0] == 'L')
    Left();
  else if (msg.data[0] == 'R')
    Right();
  else if (msg.data[0] == 'F')
    Forward();
  else if (msg.data[0] == 'B')
    Backward();
  else
    Stop();
}

ros::Subscriber<std_msgs::String> sub("motors", &messageCb );
ros::Publisher odom_publisher("sendOdom", &odom_msg);


void setup()
{

  myTimer.every(PERIOD, publishOdom);

  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  //pinMode(DIR3, OUTPUT);
  //pinMode(DIR4, OUTPUT);

  pinMode(encoder_r_a, INPUT_PULLUP);
  pinMode(encoder_r_b, INPUT_PULLUP);

  pinMode(encoder_l_a, INPUT_PULLUP);
  pinMode(encoder_l_b, INPUT_PULLUP);

  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  //  pinMode(PWM3, OUTPUT);
  //  pinMode(PWM4, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(odom_publisher);

  attachInterrupt(digitalPinToInterrupt(encoder_r_a), encoderRPinChangeA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_r_b), encoderRPinChangeB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_l_a), encoderLPinChangeA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_l_b), encoderLPinChangeB, CHANGE);


 // Forward();
//  delay(25000);
 // Stop();
}

void loop()
{
  myTimer.update();

  //  String data = "R" + String(counterR) + "L" + String(counterL);
  //  char data_final[15];
  //  data.toCharArray(data_final, 15);
  //  odom_msg.data = data_final;
  //  odom_publisher.publish(&odom_msg);
  delay(10);
  nh.spinOnce();
}

void Forward() {

  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  //  digitalWrite(DIR3,HIGH);
  //  digitalWrite(DIR4,HIGH);

  analogWrite(PWM1, Speed);
  analogWrite(PWM2, Speed);
  //  analogWrite(PWM3,Speed);
  //  analogWrite(PWM4,Speed);

  delay(10);
}


void Backward() {

  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  //  digitalWrite(DIR3,LOW);
  //  digitalWrite(DIR4,LOW);

  analogWrite(PWM1, Speed);
  analogWrite(PWM2, Speed);
  //  analogWrite(PWM3,Speed);
  //  analogWrite(PWM4,Speed);

  delay(10);
}


void Left() {

  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, LOW);
  //  digitalWrite(DIR3,LOW);
  //  digitalWrite(DIR4,HIGH);

  analogWrite(PWM1, Speed);
  analogWrite(PWM2, Speed);
  //  analogWrite(PWM3,Speed);
  //  analogWrite(PWM4,Speed);
  //
  delay(10);
}


void Right() {

  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, HIGH);
  //  digitalWrite(DIR3,HIGH);
  //  digitalWrite(DIR4,LOW);

  analogWrite(PWM1, Speed);
  analogWrite(PWM2, Speed);
  //  analogWrite(PWM3,Speed);
  //  analogWrite(PWM4,Speed);
  //
  delay(10);
}

void Stop() {

  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
  //  analogWrite(PWM3,0);
  //  analogWrite(PWM4,0);

  delay(10);

}
void encoderRPinChangeA()
{
  counterR += digitalRead(encoder_r_a) == digitalRead(encoder_r_b) ? -1 : 1;
}

void encoderRPinChangeB()
{
  counterR += digitalRead(encoder_r_a) != digitalRead(encoder_r_b) ? -1 : 1;
}



void encoderLPinChangeA()
{
  counterL += digitalRead(encoder_l_a) == digitalRead(encoder_l_b) ? -1 : 1;
}

void encoderLPinChangeB()
{
  counterL += digitalRead(encoder_l_a) != digitalRead(encoder_l_b) ? -1 : 1;
}


void publishOdom() {
  double rotationsR = (counterR - lastReadingR) / PULSES_PER_ROTATION;
  rpmR = (rotationsR * 60.0) /  (   PERIOD / 1000.0 );
  Wr = (rotationsR * 2*PI) * (1000 / PERIOD); 
  Vr = Wr * WHEEL_RADIUS;   //velocity in m/s
  lastReadingR = counterR;
  
  double rotationsL = (counterL - lastReadingL) / PULSES_PER_ROTATION;
  Wl = (rotationsL * 2*PI) * (1000 / PERIOD);
  rpmL = (rotationsL * 60.0) / ( PERIOD / 1000.0 );
  Vl = Wl * WHEEL_RADIUS;   //velocity in m/s
  lastReadingL = counterL;

  Vx = (Vr + Vl) / 2;
  Vth = (Vr - Vl) / WHEELS_SEPARATION;
  
  String data = "R" + String(Vr) + "L" + String(Vl)+"A" +String(Vx);
  char data_final[15];
  data.toCharArray(data_final, 15);
  odom_msg.data = data_final;
  odom_publisher.publish(&odom_msg);


}
