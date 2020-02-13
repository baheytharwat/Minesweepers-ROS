#include <ros.h>
#include <sensor_msgs/Joy.h>

#define DIR1 12  //cytron1
#define DIR2 8

#define DIR3 A0  //cytron2
#define DIR4 7


#define PWM1 11  //cytron1
#define PWM2 10

#define PWM3 9   //cytron2
#define PWM4 6

#define Speed 255

int buttons[12]={0};
int axes[6]={0};
int x=0;
int y=0;

ros::NodeHandle  nh;

void messageCb( const sensor_msgs::Joy& msg){
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led

  if(msg.axes[4]==1)
    Left();
  else if (msg.axes[4]==-1)
    Right();

  else if(msg.axes[5]==1)
    Forward();
  else if (msg.axes[5]==-1)
    Backward();
  else
    Stop();
}

ros::Subscriber<sensor_msgs::Joy> sub("/joy", &messageCb );

void setup()
{ 
  pinMode(13,OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(DIR3, OUTPUT);
  pinMode(DIR4, OUTPUT);
  
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);

  
}

void loop()
{  

  
  
  nh.spinOnce();
  delay(1);
}

void Forward(){
  
  digitalWrite(DIR1,HIGH);
  digitalWrite(DIR2,HIGH);
  digitalWrite(DIR3,HIGH);
  digitalWrite(DIR4,HIGH);
  
  analogWrite(PWM1,Speed);
  analogWrite(PWM2,Speed);
  analogWrite(PWM3,Speed);
  analogWrite(PWM4,Speed);
  
   delay(100);
  }

  
void Backward(){
  
  digitalWrite(DIR1,LOW);
  digitalWrite(DIR2,LOW);
  
  digitalWrite(DIR3,LOW);
  digitalWrite(DIR4,LOW);
  
  analogWrite(PWM1,Speed);
  analogWrite(PWM2,Speed);
  analogWrite(PWM3,Speed);
  analogWrite(PWM4,Speed);
  
   delay(100);
  }

  
void Left(){
  
  digitalWrite(DIR1,HIGH);
  digitalWrite(DIR2,LOW);
  digitalWrite(DIR3,LOW);
  digitalWrite(DIR4,HIGH);
  
  analogWrite(PWM1,Speed);
  analogWrite(PWM2,Speed);
  analogWrite(PWM3,Speed);
  analogWrite(PWM4,Speed);
  
   delay(100);
  }


void Right(){
  
  digitalWrite(DIR1,LOW);
  digitalWrite(DIR2,HIGH);
  digitalWrite(DIR3,HIGH);
  digitalWrite(DIR4,LOW);
  
  analogWrite(PWM1,Speed);
  analogWrite(PWM2,Speed);
  analogWrite(PWM3,Speed);
  analogWrite(PWM4,Speed);
  
   delay(100);
  }
  
  void Stop(){

  analogWrite(PWM1,0);
  analogWrite(PWM2,0);
  analogWrite(PWM3,0);
  analogWrite(PWM4,0);
  
   delay(100);

    
  }  
