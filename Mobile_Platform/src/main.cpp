#include <Arduino.h>
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

int base_speed = 1000;
float last_error = 0;
int new_speed = 0;
int current_speed = 0;
int scaled_degrees = 0;
bool go = true;
int Sensor1 = 0;
int Sensor2 = 0;
int Sensor3 = 0;
int num = 0;
float mpuAngle;

enum class robotState {DRIVE, STOP, BASE_IN, BASE_OUT, IDLE, MEASURE};
robotState currentState = robotState::DRIVE;

String currentDirection = "P";
String lastAngle = "P";

MPU6050 mpu(Wire);
AccelStepper leftMotor(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightMotor(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

void checkSerial(){
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    if (data == "1"){
      currentState = robotState::STOP;      
    }
    else if(data == "0"){
      currentState = robotState::DRIVE;  
    }
  }
}

float getAngle(){
  return mpu.getAngleZ();
}

void setMotorSpeed(float leftWheel, float rightWheel){
  leftMotor.setSpeed(-leftWheel);
  rightMotor.setSpeed(rightWheel);
}

void sendCompass() { 
  if(mpuAngle < 0 ){
    scaled_degrees = 360 - (int(mpuAngle) % 360);
  } else {
    scaled_degrees = int(mpuAngle) % 360;
  }

  if(scaled_degrees <= 45 || scaled_degrees >= 315) {
    currentDirection = "N";
    if (lastAngle != currentDirection){
      Serial.println("N");
      lastAngle = currentDirection;
    }
  } else if(scaled_degrees > 45 && scaled_degrees < 135 ) {
      currentDirection = "W";
      if(lastAngle != currentDirection){
        Serial.println("W");
        lastAngle = currentDirection;
      }
  } else if(scaled_degrees > 135 && scaled_degrees < 225) {
    currentDirection = "S";
    if(lastAngle != currentDirection){
      Serial.println("S");
      lastAngle = currentDirection;
    }
  } else if(scaled_degrees > 225 && scaled_degrees < 315 ) {
    currentDirection = "E";
    if(lastAngle != currentDirection){
      Serial.println("E");  
      lastAngle = currentDirection;  
    }
  }
}

void rotateTo(int target_angle){
  if (abs(target_angle - mpuAngle) > 1 || abs(last_error - (target_angle - mpuAngle)) > 0.5) {
    last_error = target_angle - mpuAngle;
    if (mpuAngle < target_angle) {
      setMotorSpeed(base_speed, 0);
    } 
    else {
      setMotorSpeed(0, base_speed);
    }
  } 
  else {
   setMotorSpeed(base_speed, base_speed);
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

  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
  pinMode(Sensor3, INPUT);
}

void loop () {
  mpu.update();
  leftMotor.runSpeed();
  rightMotor.runSpeed();
  checkSerial();
  mpuAngle = getAngle();

  if (current_speed != new_speed){
    leftMotor.setMaxSpeed(new_speed);
    rightMotor.setMaxSpeed(new_speed);
    current_speed = new_speed;
  }

  switch (currentState){
    case robotState::DRIVE:
      base_speed = 3000;
      rotateTo(0);
      sendCompass();
      break;

    case robotState::STOP:
      base_speed = 0;
      leftMotor.stop();
      rightMotor.stop();
      sendCompass();
      break;
    

    case robotState::BASE_IN:
      while (true){
        Sensor1 = digitalRead(8);
        Sensor2 = digitalRead(9);
        Sensor3 = digitalRead(10);

        if(Sensor3 == HIGH && Sensor2 == LOW && Sensor1 == LOW){
          setMotorSpeed(500, 750 );
        }

        else if (Sensor3 == LOW && Sensor2 == LOW && Sensor1 == HIGH){
          setMotorSpeed(750, 500 );
        }

        else if (Sensor3 == LOW && Sensor2 == HIGH && Sensor1 == LOW){
          setMotorSpeed(500, 500 );
        }

        else{
          currentState = robotState::IDLE;
          break;
        }
      }
      base_speed = 300;
      rotateTo(0);  
      break;


    case robotState::BASE_OUT:
      base_speed = 300;
      setMotorSpeed(-500, -500);
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