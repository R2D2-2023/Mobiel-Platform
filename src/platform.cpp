#include "platform.hpp"


Platform::Platform(int left_step_pin, int left_dir_pin, int right_step_pin, int right_dir_pin, int base_speed) :
  mpu(Wire),
  left_motor(AccelStepper::DRIVER, left_step_pin, left_dir_pin),
  right_motor(AccelStepper::DRIVER, right_step_pin, right_dir_pin),
  current_state(RobotState::DRIVE),
  base_speed(base_speed),
  last_error(0),
  period(2000),
  time_now(0),
  previous_millis(0),
  new_speed(0),
  current_speed(0),
  angle(0),
  right_degrees(0),
  current_direction("P"),
  last_direction("P"),
  sensor_1(0),
  sensor_2(0),
  sensor_3(0)
{}


void Platform::setup(){
  Serial.begin(115200);
  Serial2.begin(57600);
  Serial3.begin(9600);

// Setup mpu/steppermotors //
  mpu.begin();
  delay(1000);
  mpu.calcOffsets();
 
  pinMode(sensor_1, INPUT);
  pinMode(sensor_2, INPUT);
  pinMode(sensor_3, INPUT);
  left_motor.setAcceleration(2000);
  right_motor.setAcceleration(2000);
  left_motor.setMaxSpeed(base_speed);
  right_motor.setMaxSpeed(base_speed);

}

void Platform::loop(){
  mpu.update();
  left_motor.runSpeed();
  right_motor.runSpeed();
  climate_sensor.sendLiveLocationValues();

  
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
      // checkSerial();
      rotateTo(0);
      // sendCardinal();
      break;

    case RobotState::STOP:
      new_speed = 0;
      left_motor.stop();
      right_motor.stop();
      sendCardinal();
      checkSerial();
      break;
    

    case RobotState::BASE_IN:
        sensor_1 = digitalRead(8);
        sensor_2 = digitalRead(9);
        sensor_3 = digitalRead(10);
        

        if(sensor_3 == LOW && sensor_2 == HIGH && sensor_1 == LOW){
          left_motor.setSpeed(0);
          right_motor.setSpeed(2000);
          break;
        }

        if (sensor_3 == HIGH && sensor_2 == LOW && sensor_1 == LOW){
          left_motor.setSpeed(-2000);
          right_motor.setSpeed(0);
          break;
        }

        if (sensor_3 == HIGH && sensor_2 == HIGH && sensor_1 == LOW){
          left_motor.setSpeed(-2000);
          right_motor.setSpeed(2000);
          break;
        }

        else{
          current_state = RobotState::IDLE;
          break;
        }



    case RobotState::BASE_OUT:
      new_speed = 300;
      setMotorSpeed(2000, 2000);
      break;
    
    case RobotState::IDLE:
      left_motor.stop();
      right_motor.stop();
      // need signal for when the robots needs to go to BASE_OUT
      bool is_done = climate_sensor.isDoneCharging();
      if (is_done){
        Serial.println("hey");
        current_state = RobotState::BASE_OUT;
        delay(30000);
      }
      break;

      case RobotState::MEASURE:
      left_motor.stop();
      right_motor.stop();
      climate_sensor.setUpSensors();
      climate_sensor.doMeasurements();
      current_state = RobotState::DRIVE;
      break;
  }
}

void Platform::checkSerial() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    if (data == "1") {
      current_state = RobotState::STOP;
    }
    else if (data == "0") {
      current_state = RobotState::DRIVE;
    }
  }
}

float Platform::getAngle() {
  return mpu.getAngleZ();
}

void Platform::setMotorSpeed(float left_wheel, float right_wheel) {
  left_motor.setSpeed(1000 - left_wheel);
  right_motor.setSpeed(1000 - right_wheel);
}

void Platform::sendCardinal() {
  

  right_degrees = int(angle) % 360;
    if (angle < 0) {
       right_degrees += 360;
     }
    //  Serial.println(right_degrees);
    if (right_degrees <= 10 || right_degrees >= 350) {
      current_direction = "N";
      if (last_direction != current_direction) {
        Serial.println("N");
        
        last_direction = current_direction;
      }
    }
    else if (right_degrees > 80 && right_degrees < 100) {
      current_direction = "W";
      if (last_direction != current_direction) {
        Serial.println("W");
        last_direction = current_direction;
      }
    }
    else if (right_degrees > 170 && right_degrees < 190) {
      current_direction = "S";
      if (last_direction != current_direction) {
        Serial.println("S");
        last_direction = current_direction;
      }
    }
    else if (right_degrees > 260 && right_degrees < 280) {
      current_direction = "E";
      if (last_direction != current_direction) {
        Serial.println("E");  
        last_direction = current_direction;  
      }
    }
}

void Platform::rotateTo(int target_angle){
  angle = getAngle();
  if (abs(target_angle - angle) > 1 || abs(last_error - (target_angle - angle)) > 0.5) {
    last_error = target_angle - angle;
    if (angle < target_angle) {
      setMotorSpeed(2000, 2000);
    } 
    else {
      setMotorSpeed(-2000, -2000);
    }
  } 
  else {
   setMotorSpeed(2000, 0);
  }
}