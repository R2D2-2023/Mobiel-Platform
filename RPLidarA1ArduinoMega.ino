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
float last_error = 0;

MPU6050 mpu(Wire);
AccelStepper leftMotor(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightMotor(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

bool go = true;


float getAngle(){
  return mpu.getAngleZ();
}

void setMotorSpeed(float leftWheel, float rightWheel){
  leftMotor.setSpeed(1000 - leftWheel);
  rightMotor.setSpeed((1000 - rightWheel)*-1);
}

void rotateTo(int target_angle){
  float angle = getAngle();
  if (abs(target_angle - angle) > 1 || abs(last_error - (target_angle - angle)) > 0.5) {
    last_error = target_angle - angle;
    if (angle < target_angle) {
      setMotorSpeed(1000, 0);
    } 
    else {
      setMotorSpeed(0, 1000);
    }
  } 
  else {
   setMotorSpeed(0, 0);
  }
}

void Gyro(){
  if (go){    
    if (getAngle() != 0){
      rotateTo(0);
    }
  }
}

void setup () {
  Serial.begin(9600);
  mpu.begin();
  delay(1000);
  mpu.calcOffsets();
  leftMotor.setAcceleration(2000);
  rightMotor.setAcceleration(2000);
  leftMotor.setMaxSpeed(base_speed);
  rightMotor.setMaxSpeed(base_speed);
}

unsigned long previous_millis = 0;
enum class robotState {DRIVE, STOP, BASE_IN, BASE_OUT};
robotState currentState = robotState::DRIVE;

int counter = 0;
int new_speed = 0;
int speed = 0;

void loop () {
  mpu.update();
  // Gyro();
  leftMotor.runSpeed();
  rightMotor.runSpeed();
  
  if (speed != new_speed){
    leftMotor.setMaxSpeed(new_speed);
    rightMotor.setMaxSpeed(new_speed);
    speed = new_speed;
  }

  // robotState();
  // Serial.println(counter);
  switch (currentState){
    case robotState::DRIVE:
      new_speed = 1000;
      rotateTo(0);
      counter++;
      if (counter > 1000){
        currentState = robotState::STOP;
        counter = 0;
      }
      break;

    case robotState::STOP:
      leftMotor.stop();
      rightMotor.stop();
      setMotorSpeed(1000,1000);
      counter++;
      if (counter > 1000){
        currentState = robotState::BASE_IN;
        counter = 0;
      }
      break;
    

    case robotState::BASE_IN:
      new_speed = 300;
      rotateTo(0);  
      
      counter++;
      if (counter > 1000){
        currentState = robotState::BASE_OUT;
        counter = 0;
      }
      break;


    case robotState::BASE_OUT:
      new_speed = 300;
      setMotorSpeed(2000, 2000);
      counter++;
      if (counter > 1000){
        currentState = robotState::DRIVE;
        counter = 0;
      }
      break;
    // case BASE_OUT:

    
  }
  //case1 Rijden
  

  //case2 Stoppen

  // //case3 Base station
  // leftMotor.setMaxSpeed(100);
  // rightMotor.setMaxSpeed(100);

  // //case4 Base station uitrijden
  // setMotorSpeed(-1000, -1000);



}