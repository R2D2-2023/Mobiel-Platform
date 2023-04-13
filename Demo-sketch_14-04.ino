#include <TaskScheduler.h>
// #include <LSM303.h>
#include <Wire.h>
#include <RPLidar.h>
#include <MPU6050_light.h>
#include <AccelStepper.h>

#define LEFT_STEP_PIN 2
#define LEFT_DIR_PIN  3
#define RIGHT_STEP_PIN 4
#define RIGHT_DIR_PIN  5
#define RPLIDAR_MOTOR 10

const int base_speed = 1000;

// Callback methods prototypes
void Lidar();
void Motor();
void Gyro();
void setInput();

//Tasks

Task t1(20, TASK_FOREVER, &Lidar);
Task t2(100, TASK_FOREVER, &Motor);
Task t3(50, TASK_FOREVER, &Gyro);
Task t4(5000, TASK_FOREVER, &setInput);

Scheduler runner;
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
}

float last_error = 0;
void rotateTo(int target_angle){
  if (abs(target_angle - getAngle()) > 1 || abs(last_error - (target_angle - getAngle())) > 0.5) {
    input = 0;
    last_error = target_angle - getAngle();
    float speed = checkSpeed(target_angle);
    if (getAngle() < target_angle) {
      setMotorSpeed(speed, 0);
    } 
    else {
      setMotorSpeed(0, speed);
    }
  } 
  else {
   setMotorSpeed(0, 0);



  }
}

void Motor() {
  // Serial.print("Current Angle: ");
  // Serial.print(getAngle());
  // Serial.print(" || Motor Left: ");
  // Serial.print(255 - left);
  // Serial.print(" || Motor Right: ");
  // Serial.println(255 - right);
  // Serial.println("\n");

  leftMotor.setSpeed(1000 - left*3.9);
  rightMotor.setSpeed(1000 - right*3.9);
  // leftMotor.setSpeed(255 - left);
  // rightMotor.setSpeed(255 - left);

  yield();
  
}


void Gyro(){
  if (go){
    // if (Serial.available() > 0) {
    if(input != 0){
      float angle = input;
      // float angle = Serial.parseFloat();
      if (angle >= -90 && angle <= 90) {
      // if (!is_rotating) {
        total_angle = getAngle() + angle;
        // is_rotating = true;
        input = 0;
        // rotateTo(total_angle);
      // } else {

      // total_angle = getAngle();
      // rotateTo(total_angle);
      // }
      }
    } 
      

    else {
      // is_rotating = false;
      rotateTo(total_angle);
    }
  
  }
  yield();
}


void Lidar() {
  if (IS_OK(lidar.waitPoint())) {
  float distance = lidar.getCurrentPoint().distance; 
  float angle    = lidar.getCurrentPoint().angle;
  byte  quality  = lidar.getCurrentPoint().quality; 
  
    if (angle < 60 || angle > 300){
      if (quality > 21){
        if (distance < 40){
          setMotorSpeed(255,255);
          go = false;
        } else { 
          go = true;
        }
      }
      
    }
  } else {
    analogWrite(RPLIDAR_MOTOR, 0); 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info))) {
       lidar.startScan();
       analogWrite(RPLIDAR_MOTOR, 255);
      //  delay(1000);
    }
  }
  yield();
}

void setInput(){
  // Serial.write(90);
  input = 90;
  Serial.print("done");
}
void setup () {
  Serial.begin(9600);
  Wire.begin();
  byte status = mpu.begin();
  delay(1000);
  mpu.calcOffsets();
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT); 
  leftMotor.setAcceleration(2000);
  rightMotor.setAcceleration(2000);

  leftMotor.setMaxSpeed(base_speed);
  rightMotor.setMaxSpeed(base_speed);
  runner.init();
  // runner.addTask(t1);
  runner.addTask(t2);
  runner.addTask(t3);
  runner.addTask(t4);
  // Scheduler.start();

  // Create task to run every 500 microseconds
  // Scheduler.startLoop(Lidar);
  // Scheduler.startLoop(Gyro);
  // Scheduler.startLoop(Motor);
  // t1.enable();
  t2.enable();
  t3.enable();
  t4.enable();


  // Scheduler.every(1, Lidar);
  // Scheduler.every(1, Gyro);
  // Scheduler.every(1, Motor);
}

unsigned long previous_millis = 0;

void loop () {
  runner.execute();
  // unsigned long current_millis = millis();
  // // Scheduler.execute();
  
  // if (current_millis - previous_millis > 3000)
  // {
  //   input = 90;
  //   previous_millis = current_millis;
  // }

  
  leftMotor.runSpeed();
  rightMotor.runSpeed();
}
