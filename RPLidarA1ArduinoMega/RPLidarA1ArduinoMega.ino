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

int period = 2000;
unsigned long time_now = 0;

unsigned long previous_millis = 0;

enum class robotState {DRIVE, STOP, BASE_IN, BASE_OUT, IDLE, MEASURE};
robotState currentState = robotState::STOP;

int counter = 0;
int new_speed = 0;
int speed = 0;
float angle =0;
String currentDirection = "N";
String lastAngle = "N";

MPU6050 mpu(Wire);
AccelStepper leftMotor(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightMotor(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

bool go = true;
bool booted = false;

bool checkLidar(){
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
      if (data == "1"){
        currentState = robotState::STOP;      }
      else if(data == "0"){
        currentState = robotState::DRIVE;  
      }
  }

}

float getAngle(){
  return mpu.getAngleZ();
}

void setMotorSpeed(float leftWheel, float rightWheel){
  leftMotor.setSpeed(1000 - leftWheel);
  rightMotor.setSpeed((1000 - rightWheel)*-1);
}

void sendCompass() {
    if( angle < 45 || angle > 315 ) {
      currentDirection = "N";
      Serial.println("N");
    }
    else if(angle > 45 && angle < 135 ) {
      currentDirection = "E";
      Serial.println("E");
    }
    else if(angle > 135 && angle < 225 ) {
      currentDirection = "S";
      Serial.println("S");
    }
    else if(angle > 225 && angle < 315 ) {
      currentDirection = "W";
      Serial.println("W");
    }
    lastAngle = currentDirection;
}

void rotateTo(int target_angle){
  angle = getAngle();
  if (abs(target_angle - angle) > 1 || abs(last_error - (target_angle - angle)) > 0.5) {
    last_error = target_angle - angle;
        
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

void Gyro(){
  if (go){    
    if (getAngle() != 0){
      rotateTo(0);
    }
  }
}

void setup () {
  Serial.begin(9600);

// Setup climate sensors//

//  Serial1.begin(115200);
//  int result = climate_sensors.setUpSensors();
//  if (result)
//  {
//    Serial1.print(result);
//    Serial1.print(" ");
//    Serial1.println("unlucky bro");
//    while (1){};
//  }


// Setup mpu/steppermotors //
  mpu.begin();
  delay(1000);
  mpu.calcOffsets();
  leftMotor.setAcceleration(2000);
  rightMotor.setAcceleration(2000);
  leftMotor.setMaxSpeed(base_speed);
  rightMotor.setMaxSpeed(base_speed);
}

void loop () {
  mpu.update();
  // Gyro();
  leftMotor.runSpeed();
  rightMotor.runSpeed();
  if (booted == false){
    if (Serial.available() > 0){
      String data_2 = Serial.readStringUntil('\n');
      if (data_2 == "BOOTED"){
         Serial.println("HEARD");
        currentState = robotState::DRIVE;
        booted = true;
      }
    }
  }
  if (speed != new_speed){
    leftMotor.setMaxSpeed(new_speed);
    rightMotor.setMaxSpeed(new_speed);
    speed = new_speed;
  }

  switch (currentState){
    case robotState::DRIVE:
      new_speed = 1000;
      checkLidar();
      if(lastAngle != currentDirection){
         sendCompass();
       }
     
      rotateTo(0);
      break;

    case robotState::STOP:
      new_speed = 0;
      leftMotor.stop();
      rightMotor.stop();
      checkLidar();
      break;
    

    case robotState::BASE_IN:
      new_speed = 300;
      rotateTo(0);  

      break;


    case robotState::BASE_OUT:
      new_speed = 300;
      setMotorSpeed(2000, 2000);
      break;
    
    case robotState::IDLE:
      leftMotor.stop();
      rightMotor.stop();
      // need signal for when the robots needs to go to BASE_OUT
      currentState = robotState::BASE_OUT;
      break;

      case robotState::MEASURE:
      leftMotor.stop();
      rightMotor.stop();
      // CODE for measuring
      // climate_sensors.doMeasurements();
      currentState = robotState::DRIVE;
      break;
   
  }




}
