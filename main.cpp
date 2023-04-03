#include <RPLidar.h>
#include <L298N.h>

#define RPLIDAR_MOTOR 10 
#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);

// motor pin definition
#define M_EN 9
#define M_IN1 8
#define M_IN2 7
#define M_EN1 12
#define M_IN3 11
#define M_IN4 2
RPLidar lidar;
L298N motor1(M_EN, M_IN1, M_IN2);
L298N motor2(M_EN1, M_IN3, M_IN4);
float minDistance = 100000;
float angleAtMinDist = 0;
unsigned short driverPwm = 255;
const int numReadings = 10;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average


// variables for sending information frequency configuration
long previousMillis = 0; 
long interval = 200;  
float robotAngle = 0;
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

   Wire.begin();
 byte status = mpu.begin();
   delay(1000);
   mpu.calcOffsets(); // gyro  
   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

}

float getAngle(){
  mpu.update();
  return mpu.getAngleZ() - robotAngle;
}


float pow(float nmbr){
  return nmbr * nmbr;
}

float checkSpeed(int target_angle){
  mpu.update();

  return pow((target_angle - mpu.getAngleZ())*0.177);
}
void rotate(int target_angle, L298N motor1, L298N motor2){
  int current_angle = getAngle();
  // int b = 0;
  
  if (target_angle > 0){
    while (current_angle < target_angle)
    {
      float speed = checkSpeed(target_angle);     
      Serial.println(getAngle());
       Serial.println(255 - speed);
      current_angle = getAngle();
    }

    
  } else {
    while (current_angle > target_angle)
    {
      float speed = checkSpeed(target_angle); 
            Serial.println(getAngle());
     
      Serial.println(-255 + speed);
      current_angle = getAngle();
    }
  }
  // else{
  //   while (current_angle > target_angle){
  //     b++;
  //     float speed = checkSpeed(target_angle);
      
  //       Serial.println(-255 + speed);
      
  //     mpu.update();
  //     current_angle = mpu.getAngleZ();
  //   }

      
  // }
  

 
}

void loop() {
  
    //perform data processing here... 
     // 0-360 deg
  total = total - readings[readIndex];
  readings[readIndex] = mpu.getAngleZ();

  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  
  average = total / numReadings;
  int target = 90;
  // Serial.println(average);
  Serial.print(getAngle());
  if (getAngle() < target ){
    rotate(target, motor1, motor2);
  } else {
    Serial.println("offset calculated");
    robotAngle += target;
 // gyro  

  }

}

