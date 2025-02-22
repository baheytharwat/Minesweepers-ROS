#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>

#define WHEEL_SEPARATION 0.35

double Vr = 0;
double Vl = 0;


double vx = 0;
double vy = 0;
double vth = 0;

void callBackFn(const std_msgs::String::ConstPtr &msg){

std::string odom_str,r_str,l_str;
int rIndex,lIndex,len;
odom_str = msg->data;

len = odom_str.size();

rIndex = odom_str.find('R');
lIndex = odom_str.find('L');

r_str = odom_str.substr(rIndex+1,lIndex);
l_str = odom_str.substr(lIndex+1,len);

Vr = (double)atof(r_str.c_str());
Vl = (double)atof(l_str.c_str());

vx = (Vr + Vl) / 2;
vth = (Vr - Vl) / WHEEL_SEPARATION;

ROS_INFO_STREAM("Vx: "<<vx<<", Vth: "<<vth);



}

int main(int argc, char** argv){
  ros::init(argc, argv, "odometry_publisher");

  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("example/odom", 50);
  ros::Subscriber sub = n.subscribe("sendOdom",1000,callBackFn);
  tf::TransformBroadcaster odom_broadcaster;

  double x = 0.0;
  double y = 0.0;
  double th = 0.0;


  ros::Time current_time, last_time,start_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();
  start_time = ros::Time::now();

  ros::Rate r(10.0);
  while(n.ok()){

    ros::spinOnce();               // check for incoming messages
    current_time = ros::Time::now();

    //compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;
    double variance =  (0.0001 * (current_time-start_time).toSec()) + 0.001;

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
    //odom_broadcaster.sendTransform(odom_trans);

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
    
    //set the covariance
    int i=0;
    while(i<=35){
    odom.pose.covariance[i] =variance;
    odom.twist.covariance[i]=variance; 
    i+=7;
	}
    //publish the message
    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();
  }
}
