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

#define Speed 220

#define encoder_r_a 19
#define encoder_r_b 18

#define encoder_l_a 3
#define encoder_l_b 2


std_msgs::String odom_msg;

long counterR = 0;
long counterL = 0;


int buttons[12] = {0};
int axes[6] = {0};
int x = 0;
int y = 0;

ros::NodeHandle  nh;

void messageCb( const std_msgs::String& msg) {
  digitalWrite(13, HIGH - digitalRead(13)); // blink the led

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
ros::Publisher odom_publisher("odomm", &odom_msg);


void setup()
{

  myTimer.every(PERIOD, publishOdom);

  pinMode(13, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  //pinMode(DIR3, OUTPUT);
  //pinMode(DIR4, OUTPUT);

  pinMode(34, OUTPUT);
  digitalWrite(34, LOW);

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



  String data = "R" + String(counterR) + "L" + String(counterL);
  char data_final[15];
  data.toCharArray(data_final, 15);
  odom_msg.data = data_final;
  odom_publisher.publish(&odom_msg);


}
