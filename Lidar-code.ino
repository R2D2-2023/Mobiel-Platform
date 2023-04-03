#include <RPLidar.h>

RPLidar lidar;

#define RPLIDAR_MOTOR 10

void setup() {
  // put your setup code here, to run once:
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);
}

void loop() {
  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance; 
    float angle    = lidar.getCurrentPoint().angle;
    byte  quality  = lidar.getCurrentPoint().quality; 
    
    if (angle < 60 || angle > 300){
      if (distance < 400 && quality == 21){
        analogWrite(RPLIDAR_MOTOR, 0);
      }
    }
  } else {
    analogWrite(RPLIDAR_MOTOR, 0); 
 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       lidar.startScan();
       analogWrite(RPLIDAR_MOTOR, 255);
       delay(1000);
    }
  }
}
