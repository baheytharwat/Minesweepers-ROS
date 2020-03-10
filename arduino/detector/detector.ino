#include <ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Joy.h>
#include <Timer.h>
#define Proximity1 5
#define Proximity2 6
#define Proximity3 7
#define Metal1 2
#define Metal2 3
#define PERIOD 50
std_msgs::String metalPub;
ros::Publisher metalDetection("detected", &metalPub);
ros::NodeHandle nh;
String metalString="";
char detection[2];
bool flag=false;
Timer myTimer;


void setup() {
   myTimer.every(PERIOD, publish);
  // put your setup code here, to run once:
  pinMode(Proximity1,INPUT);
  pinMode(Proximity2,INPUT);
  pinMode(Proximity3,INPUT);
  pinMode(Metal1, INPUT);
  pinMode(Metal2, INPUT);
  attachInterrupt(digitalPinToInterrupt(Metal1), metalDetected, RISING);
  attachInterrupt(digitalPinToInterrupt(Metal2), metalDetected, RISING);

  nh.initNode();
  nh.advertise(metalDetection);
  
}

void loop() {
  myTimer.update();
  // put your main code here, to run repeatedly:


}


void metalDetected(){
  
 

//bool metal1=digitalRead(Metal1);
//bool metal2=digitalRead(Metal2);

bool prox1 = digitalRead(Proximity1);
bool prox2 = digitalRead(Proximity2);
bool prox3 = digitalRead(Proximity3);




if ( prox1 || prox2 || prox3)

metalString="U";  

else
 
metalString="D";

flag=true;

}


void publish(){
  if(flag){
    metalString.toCharArray(detection,2);
    metalPub.data=detection;
    metalDetection.publish(&metalPub);
    flag=false;
  }
  
}












//void readSoftwareSerial() {
//
//  String text;
//  if (softSerial.available()) {
//    text = softSerial.readString();
//    for (int i = 1 ; i < 10 ; i++)
//    {
//      sentCommand[i] = text[i-1];
//    }
//  }
//}
//
//void writeSoftwareSerial(){
//
//softSerial.println(recievedCommand);
//delay(10);
//}
