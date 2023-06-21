// #include <LSM303.h>
#include <Wire.h>
#include <RPLidar.h>
#include <MPU6050_light.h>
#include <AccelStepper.h>
#include <string.h>
#define LEFT_STEP_PIN 2
#define LEFT_DIR_PIN  3
#define RIGHT_STEP_PIN 4
#define RIGHT_DIR_PIN  5
#define RPLIDAR_MOTOR 10

const int base_speed = 1000;

// Callback methods prototypes
// void Lidar();
// void Motor();
// void Gyro();
// void setInput();

MPU6050 mpu(Wire);
RPLidar lidar;
AccelStepper leftMotor(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightMotor(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

float total_angle = 0;
float input= 0;
bool is_rotating = false;
float left;
float right;
bool go = true;




// String datastring;
//     if (data != '&'){
//       datastring += data;
//     }
//     else(){
//       data_array.append(datastring);
//       datastring = "";
//     }

float getAngle(){
  mpu.update();
  return mpu.getAngleZ();
}

float checkSpeed(int target_angle){
  mpu.update();
  float error = target_angle - getAngle();
  float speed = abs(error * 0.64);
  speed = constrain(speed*speed, 0, 255);
  return speed;
}

void setMotorSpeed(float leftWheel, float rightWheel){
  left = leftWheel;
  right = rightWheel;
  Motor();
}

float last_error = 0;
void rotateTo(int target_angle){
  float angle = getAngle();
  if (abs(target_angle - angle) > 1 || abs(last_error - (target_angle - angle)) > 0.5) {
    input = 0;
    last_error = target_angle - angle;
    // float speed = checkSpeed(target_angle);
    if (angle < target_angle) {
      setMotorSpeed(2000, 0);
    } 
    else {
      setMotorSpeed(0, 2000);
    }
  } 
  else {
   setMotorSpeed(0, 0);
  }
}

void Motor() {
  // leftMotor.setSpeed(1000 - left*3.9);
  // rightMotor.setSpeed((1000 - right*3.9)*-1);
  leftMotor.setSpeed((1000 - left));
  rightMotor.setSpeed((1000 - right)*-1);
}


void Gyro(){
  //rotateTo(0);
  if (go){    
    if (getAngle() != 0){
      rotateTo(0);
    }
  }
}

void setInput(){
  // Serial.write(90);
  input = 90;
  Serial.print("done");
}
void setup () {
  // Wire.begin(8);
  // Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  mpu.begin();
  delay(1000);
  mpu.calcOffsets();
  // lidar.begin(Serial1);
  // pinMode(RPLIDAR_MOTOR, OUTPUT); 
  leftMotor.setAcceleration(2000);
  rightMotor.setAcceleration(2000);
  leftMotor.setMaxSpeed(base_speed);
  rightMotor.setMaxSpeed(base_speed);
}

unsigned long previous_millis = 0;


// void receiveEvent(int howMany) {
//   while (Wire.available()) {
//     char c = Wire.read();
//     if(c == '1') {
//       Serial.println("Object Detected");
//       leftMotor.setSpeed(0);
//       rightMotor.setSpeed(0);
//       delay(2000);
//     } else {
//       leftMotor.setSpeed(255);
//       rightMotor.setSpeed(255);
//     }
//   }
// }


void loop () {
  // unsigned long current_millis = millis();
  
  // if (current_millis - previous_millis > 3000)
  // {
  //   input = 90;
  //   previous_millis = current_millis;
  // }
  // setMotorSpeed(0,0);
  // Motor(); 
// rotateTo(90);   
  Gyro();
  // Wire.onReceive(receiveEvent);
  leftMotor.runSpeed();
  rightMotor.runSpeed();
}