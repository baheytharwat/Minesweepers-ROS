#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from nav_msgs.msg import Odometry

def localize(msg):
	global x
	global y
	
	x = msg.pose.pose.position.x
	y = msg.pose.pose.position.y

def mineDetected(msg):
	
	global x
	global y

	pub = rospy.Publisher('setMines', String, queue_size=10)

	isAbove = msg.data

	


	msg_gui = String()	
	msg_gui.data = str(isAbove) + "X" + str(x) + "Y" + str(y)

	

	pub.publish(msg_gui)
	
    
def mainFn():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The																																																																						
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('metal_gui', anonymous=True)

    rospy.Subscriber("detected", String, mineDetected)

    rospy.Subscriber("example/odom", Odometry, localize)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

x = 3
y = 5

if __name__ == '__main__':
    mainFn()
