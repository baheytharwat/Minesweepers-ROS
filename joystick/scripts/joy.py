#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import String

def callback(joy):


	msg = String()

	if joy.axes[0] == 1:
		msg.data = "L"
	elif joy.axes[0] == -1:
		msg.data = "R"

	elif joy.axes[1] == 1:
		msg.data = "F"

	elif joy.axes[1] == -1:
		msg.data = "B"

	else:
		msg.data = "S"

	pub = rospy.Publisher('motors', String, queue_size=10)
	
	rate = rospy.Rate(10)


	pub.publish(msg)


def mainFn():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.

	rospy.init_node('sendJoy', anonymous=True)
	rospy.Subscriber("joy", Joy, callback)
	#pub = rospy.Publisher('chatter', String, queue_size=10)
	#rate = rospy.Rate(10)
	while not rospy.is_shutdown():
    	# spin() simply keeps python from exiting until this node is stopped
		rospy.spin()


if __name__ == '__main__':
    mainFn()
