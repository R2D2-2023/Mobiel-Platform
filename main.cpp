#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);

float total_angle = 0;
float target_angle = 0;
bool is_rotating = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  byte status = mpu.begin();
  delay(1000);
  mpu.calcOffsets();
}

float getAngle(){
  mpu.update();
  return mpu.getAngleZ();
}

void setMotorSpeed(float left, float right){
  Serial.print("Current Angle: ");
  Serial.print(getAngle());
  Serial.print(" || Motor Left: ");
  Serial.print(255 - left);
  Serial.print(" || Motor Right: ");
  Serial.println(255 - right);
  Serial.println("\n");
}

float checkSpeed(int target_angle){
  mpu.update();
  float error = target_angle - getAngle();
  float speed = abs(error * 0.1773);
  speed = constrain(speed*speed, 0, 255);
  return speed;
}

float last_error = 0;
void rotateTo(int target_angle){
  while (abs(target_angle - getAngle()) > 2 || abs(last_error - (target_angle - getAngle())) > 1) {
    last_error = target_angle - getAngle();
    float speed = checkSpeed(target_angle);
    if (getAngle() < target_angle) {
      setMotorSpeed(speed, 0);
    } else {
      setMotorSpeed(0, speed);
    }
  }
  setMotorSpeed(0, 0);
}

void loop() {
  if (Serial.available() > 0) {
    float angle = Serial.parseFloat();
    if (angle >= -90 && angle <= 90) {
      if (!is_rotating) {
        total_angle = getAngle();
        is_rotating = true;
        rotateTo(total_angle + angle);
      } else {
        total_angle = getAngle();
        rotateTo(total_angle);
      }
    }
  } else {
    is_rotating = false;
    rotateTo(total_angle);
    setMotorSpeed(0, 0);

  }
}