#include <RPLidar.h>
#include <L298N.h>

#define RPLIDAR_MOTOR 10 


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

// variables for sending information frequency configuration
long previousMillis = 0; 
long interval = 200;  
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  lidar.begin(Serial);
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  
}

void loop() {
  if (IS_OK(lidar.waitPoint(1000))) {
    //perform data processing here... 
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;  // 0-360 deg

    if (lidar.getCurrentPoint().startBit) {
       // a new scan, display the previous data...
       if (angle < 60 || angle > 300){
         if (distance < 100){
          motor1.setSpeed(0); // turn the LED on (HIGH is the voltage level)
          motor2.setSpeed(0);
          motor1.forward();
          motor2.forward();
         }else {
          motor1.setSpeed(255); // turn the LED on (HIGH is the voltage level)
          motor2.setSpeed(255);
          motor1.forward();
          motor2.forward();
       }
       } 
       minDistance = 100000;
       angleAtMinDist = 0;
    } else {
       if ( distance > 0 &&  distance < minDistance) {
          minDistance = distance;
          angleAtMinDist = angle;
       }
    }
  }
  else {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
    // Try to detect RPLIDAR
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 1000))) {
       // Detected
       lidar.startScan();
       analogWrite(RPLIDAR_MOTOR, 255);
    }
  }
}

