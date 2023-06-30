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

enum class RobotState {DRIVE, STOP, BASE_IN, BASE_OUT, IDLE, MEASURE};
RobotState current_state = RobotState::DRIVE;

int new_speed = 0;
int current_speed = 0;
float angle =0;
int right_degrees = 0;
String current_direction = "P";
String last_direction = "P";

MPU6050 mpu(Wire);
AccelStepper left_motor(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper right_motor(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

bool go = true;

int Sensor1 = 0;
int Sensor2 = 0;
int Sensor3 = 0;


bool checkLidar(){
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
//    Serial.println(data);
      if (data == "1"){
        current_state = RobotState::STOP;      }
      else if(data == "0"){
        current_state = RobotState::DRIVE;  
      }
  }

}


/**
         * @brief gets the Yaw of the MPU6050
         */
float getAngle(){
  return mpu.getAngleZ();
}


/**
         * @brief sets the motor speed for both motors
         * 
         * @param left_wheel
         * @param right_wheel 
         */
void setMotorSpeed(float left_wheel, float right_wheel){
  left_motor.setSpeed(1000 - left_wheel);
  right_motor.setSpeed((1000 - right_wheel));
}

//void returnAngle(int given_angle){
//  if given_angle 
//}
int num = 0;



/**
         * @brief This function sends cardinal directions to PI
         * 
         */
void sendCardinal() {
  
//    Serial.println(angle);
 
    if(angle < 0 ){
       right_degrees = 360 - (int(angle) %360);
     }
     else{
      right_degrees = int(angle)%360;
     }
     

    

    Serial.println(right_degrees);
    if( right_degrees <= 45 || right_degrees >= 315) {
      current_direction = "N";
      if (last_direction != current_direction){
        Serial.println("N");
        Serial.println(right_degrees);
        last_direction = current_direction;
        
        
      }
    }
     else if(right_degrees > 45 && right_degrees < 135 ) {
          current_direction = "W";
       if(last_direction != current_direction){
          Serial.println("W");
          Serial.println(right_degrees);
          last_direction = current_direction;
           
       }
    }
     else if(right_degrees > 135 && right_degrees < 225) {
      current_direction = "S";
      if(last_direction != current_direction){
        Serial.println("S");
        Serial.println(right_degrees);
        last_direction = current_direction;
        
      }
    }
     else if(right_degrees > 225 && right_degrees < 315 ) {
      current_direction = "E";
      if(last_direction != current_direction){
        Serial.println("E");  
        Serial.println(right_degrees);  
        last_direction = current_direction;  
        
      }
    }
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

  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
  pinMode(Sensor3, INPUT);
  left_motor.setAcceleration(2000);
  right_motor.setAcceleration(2000);
  left_motor.setMaxSpeed(base_speed);
  right_motor.setMaxSpeed(base_speed);
}

void loop () {
  mpu.update();
  // Gyro();
  left_motor.runSpeed();
  right_motor.runSpeed();
  
  if (current_speed != new_speed){
    left_motor.setMaxSpeed(new_speed);
    right_motor.setMaxSpeed(new_speed);
    current_speed = new_speed;
  }

  switch (current_state){
    case RobotState::DRIVE:
      new_speed = 3000;
      left_motor.setSpeed(3000);
      right_motor.setSpeed(3000);
      checkLidar();
      rotateTo(0);
      sendCardinal();
      break;

    case RobotState::STOP:
      new_speed = 0;
      left_motor.stop();
      right_motor.stop();
      sendCardinal();
      checkLidar();
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


    case RobotState::BASE_OUT:
      new_speed = 300;
      setMotorSpeed(2000, 2000);
      break;
    
    case RobotState::IDLE:
      left_motor.stop();
      right_motor.stop();
      // need signal for when the robots needs to go to BASE_OUT
      current_state = RobotState::BASE_OUT;
      break;

      case RobotState::MEASURE:
      left_motor.stop();
      right_motor.stop();
      // CODE for measuring
      // climate_sensors.doMeasurements();
      current_state = RobotState::DRIVE;
      break;
   
  }
}