#!/usr/bin/env python
import rospy
from std_msgs.msg import Int32
from std_msgs.msg import String

def callback(odom):

	rightTicks = Int32()
	leftTicks = Int32()

	rightTicks.data = int(odom.data[odom.data.index("R")+1:odom.data.index("L")])
	leftTicks.data =int(odom.data[odom.data.index("L")+1:])


	#leftTicks.data = int(odom.data[odom.data.index("L")+1:odom.data.index("Z")])
	pubL = rospy.Publisher('rwheel_ticks', Int32, queue_size=10)
	pubR = rospy.Publisher('lwheel_ticks', Int32, queue_size=10)
	print ( rightTicks,leftTicks)
	rate = rospy.Rate(10)

	pubL.publish(rightTicks)
	pubR.publish(leftTicks)


def mainFn():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.

	rospy.init_node('sendOdom', anonymous=True)
	rospy.Subscriber("odomm", String, callback)
	#pub = rospy.Publisher('chatter', String, queue_size=10)
	#rate = rospy.Rate(10)
	while not rospy.is_shutdown():
    	# spin() simply keeps python from exiting until this node is stopped
		rospy.spin()


if __name__ == '__main__':
    mainFn()
