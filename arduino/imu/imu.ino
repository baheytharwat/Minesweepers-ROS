#include <I2Cdev.h>
#include <MPU6050.h>
#include <ros.h>
#include <std_msgs/String.h>



#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#define g 9.81

MPU6050 imm;

std_msgs::String imu_msg;
ros::Publisher imu("imu", &imu_msg);
ros::NodeHandle nh;


int16_t AcX, AcY, AcZ;
int16_t GyX, GyY, GyZ;
double real_value = 0;
unsigned long current_time =0;
unsigned long last_time=0;
float angleYaw=0;

#define LED_PIN 13
bool blinkState = false;

void setup() {
  Wire.begin();
  //Serial.begin(38400);
  
  nh.initNode();
  nh.advertise(imu);

  // initialize device
  //Serial.println("Initializing I2C devices...");
  imm.setClockSource(MPU6050_CLOCK_PLL_XGYRO);
  imm.setFullScaleGyroRange(0);
  imm.setFullScaleAccelRange(0);
  imm.setSleepEnabled(false);

  // verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(imm.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // setup imu
  imm.CalibrateGyro(15);
  imm.CalibrateAccel(15);

  // configure Arduino LED pin for output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  current_time = micros();
  imm.getMotion6(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ);
  float dt = current_time - last_time;
  angleYaw +=   GyZ * (250 / 32767.0) * (dt/1000000.0);
  int yaw = (((int)angleYaw + 180) % 360 ) - 180;
  delay(1);
  
  //mapAcc();
  //  mapGyro();

  String AX = String(AcX);
  String AY = String(AcY);
  String AZ = String(AcZ);
  String GX = String(GyX);
  String GY = String(GyY);
  String GZ = String(GyZ);
  String Y = String(yaw); // Yaw
  String data = "A" + AX + "B" + AY + "C" + AZ + "D" + GX + "E" + GY + "F" + GZ + "G" + Y + "H";
  int length = data.indexOf("H") + 2;
  char data_final[length + 1];
  data.toCharArray(data_final, length + 1);
  imu_msg.data = data_final;
  imu.publish(&imu_msg);
  nh.spinOnce();
  last_time = current_time;
   
  // read raw accel/gyro measurements from device
  // Serial.print("a/g:\t");
  // Serial.print(map(ax, -32768, 32767, -19.6, 19.6)); Serial.print("\t");
  // Serial.print(map(ay, -32768, 32767, -19.6, 19.6)); Serial.print("\t");
  // Serial.print(map(az, -32768, 32767, -19.6, 19.6)); Serial.print("\t");
  // Serial.print(map(gx, -32768, 32767, -500, 500)); Serial.print("\t");
  // Serial.print(map(gy, -32768, 32767, -500, 500)); Serial.print("\t");
  // Serial.println(map(gz, -32768, 32767, -500, 500));

}


void mapAcc(){

AcX = AcX * (2*g / 32767.0 ); 
AcY = AcY * (2*g / 32767.0 );
AcZ = AcZ * (2*g / 32767.0 );

}


void mapGyro(){

GyX = GyX * ( 250 / 32767.0);
GyY = GyY * ( 250 / 32767.0);
GyZ = GyZ * ( 250 / 32767.0);
  
}
