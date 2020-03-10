#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Imu
import tf

seq = 1
def callback(imu):
	global seq
	AX = float(imu.data[imu.data.index("A")+1:imu.data.index("B")])
	AY = float(imu.data[imu.data.index("B")+1:imu.data.index("C")])
	AZ = float(imu.data[imu.data.index("C")+1:imu.data.index("D")])
	GX = float(imu.data[imu.data.index("D")+1:imu.data.index("E")])
	GY = float(imu.data[imu.data.index("E")+1:imu.data.index("F")])
	GZ = float(imu.data[imu.data.index("F")+1:imu.data.index("G")])
	Y = float(imu.data[imu.data.index("G")+1:imu.data.index("H")])
	
	g = 9.81
	pi = 3.14
	AX = AX * (2*g / 32767.0 )
	AY = AY * (2*g / 32767.0 )
	AZ = AZ * (2*g / 32767.0 )
	GX = GX * ( 250 / 32767.0)
	GY = GY * ( 250 / 32767.0)
	GZ = GZ * ( 250 / 32767.0)
	Y = Y * pi / 180.0
	#print(Y,AX,GZ)
	#print(AX)

	msg = Imu()
	current_time = rospy.Time.now()
	msg.header.stamp = current_time
	msg.header.frame_id = "base_link"
	msg.header.seq = seq

	quaternion = tf.transformations.quaternion_from_euler(0, 0, Y)
	msg.orientation.x = quaternion[0]
	msg.orientation.y = quaternion[1]
	msg.orientation.z = quaternion[2]
	msg.orientation.w = quaternion[3]
	orientation_variance = (3 / 100) * 250
	#msg.orientation_covariance = [orientation_covariance,0,0,0,orientation_covariance,0,0,0,orientation_covariance]
	msg.orientation_covariance[0] = 0.01
	msg.orientation_covariance[4] =  0.01
	msg.orientation_covariance[8] = 0.01

	msg.angular_velocity.x = GX
	msg.angular_velocity.y= GY
	msg.angular_velocity.z = GZ
	angular_velocity_variance = (3 / 100) * 250
	#msg.angular_velocity_covariance = [angular_velocity_covariance,0,0,0,angular_velocity_covariance,0,0,0,angular_velocity_covariance]
	msg.angular_velocity_covariance[0] = 0.01
	msg.angular_velocity_covariance[4] = 0.01
	msg.angular_velocity_covariance[8] = 0.01

	msg.linear_acceleration.x = AX
	msg.linear_acceleration.y = AY
	msg.linear_acceleration.z = AZ
	linear_acceleration_variance = (3 / 100) * 2*9.81
	#msg.linear_acceleration_covariance =[linear_acceleration_covariance,0,0,0,linear_acceleration_covariance,0,0,0,linear_acceleration_covariance]
	msg.linear_acceleration_covariance[0] = 0.1
	msg.linear_acceleration_covariance[4] = 0.1
	msg.linear_acceleration_covariance[8] = 0.1

	pub = rospy.Publisher('example/imu', Imu, queue_size=10)
	rate = rospy.Rate(10)
	pub.publish(msg)

	seq +=1

def mainFn():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.

	rospy.init_node('listener', anonymous=True)
	rospy.Subscriber("sendImu", String, callback)
	#pub = rospy.Publisher('chatter', String, queue_size=10)
	#rate = rospy.Rate(10)
	while not rospy.is_shutdown():

    	# spin() simply keeps python from exiting until this node is stopped
		rospy.spin()


if __name__ == '__main__':
    mainFn()
