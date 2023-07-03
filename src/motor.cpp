#include "motor.hpp"



void Motor::setMotorSpeed(float left_wheel, float right_wheel){
  left_motor.setSpeed(1000 - left_wheel);
  right_motor.setSpeed((1000 - right_wheel));
  
}


void Motor::rotateTo(int target_angle, float angle){

  if (abs(target_angle - angle) > 1 || abs(last_error - (target_angle - angle)) > 0.5) {
    last_error = target_angle - angle;
        
    if (angle < target_angle) {
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



