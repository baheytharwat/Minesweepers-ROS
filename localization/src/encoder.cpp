#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>
#include<algorithm>
#include <string.h> 
#include<math.h>
#include<cmath>
using namespace std; 

double x = 0.0;
double y = 0.0;
double th = 0.0;
double counter2=0.0;
double vx = 0.0;
double vy = 0;
double vx2 = 0.0;
double vy2 = 0;
double vth = 0;
long counter1=0.0;
short i=1;
short j=0;
char v[10];
char c[10];
char c2[10];
char v2[10];

void VelocityCallback2(const std_msgs::String::ConstPtr& msg){
std::string msgData=msg->data;
  std::string velData(msgData.begin(), std::find(msgData.begin(), msgData.end(), 'c'));
  strcpy(v2, velData.c_str());
  vx2=atof(v2);
  int index=msgData.find('c');
  std::string counterData=msgData.substr(index+1,msgData.size()-1);
  strcpy(c2,counterData.c_str());
  counter2=atol(c2);
  ROS_INFO_STREAM("Velocity 2 :");
  ROS_INFO_STREAM(vx2);
  ROS_INFO_STREAM(counter2);
}

 void VelocityCallback(const std_msgs::String::ConstPtr& msg)
   {
std::string msgData=msg->data;
  std::string velData(msgData.begin(), std::find(msgData.begin(), msgData.end(), 'c'));
  strcpy(v, velData.c_str());
  vx=atof(v);
  int index=msgData.find('c');
  std::string counterData=msgData.substr(index+1,msgData.size()-1);
  strcpy(c,counterData.c_str());
  counter1=atol(c);
  ROS_INFO_STREAM("Velocity 1 :");
  ROS_INFO_STREAM(vx);
  ROS_INFO_STREAM(counter1);
   }

int main(int argc, char** argv){
  ros::init(argc, argv, "odometry_publisher");

  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 10);
  tf::TransformBroadcaster odom_broadcaster;
   
  

  ros::Time current_time, last_time,start_time;
    start_time = ros::Time::now();

  current_time = ros::Time::now();
  last_time = ros::Time::now();
ros::Subscriber sub = n.subscribe("Velocity", 1000, VelocityCallback); 
ros::Subscriber sub2 = n.subscribe("Velocity2", 1000, VelocityCallback2); 
  ros::Rate r(5.0);
  while(n.ok()){
    ros::spinOnce();               // check for incoming messages
    current_time = ros::Time::now();

    //compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;

    float variance =  (0.0001 * (current_time-start_time).toSec())+0.001;
    th=atan((counter1-counter2)/0.4);
	//	float variance =  0.00001;

    x += delta_x;
    y += delta_y;
    th += delta_th;

    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;
int i=0;
while(i<=35){
    odom.pose.covariance[i] =variance;
    odom.twist.covariance[i]=variance; 
    i+=7;
}
    //publish the message
    odom_pub.publish(odom);
    //ROS_INFO_STREAM((current_time-start_time).toSec());
    last_time = current_time;
    r.sleep();
  }
}
