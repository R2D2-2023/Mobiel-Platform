#include <AccelStepper.h>

#define LEFT_STEP_PIN 2
#define LEFT_DIR_PIN  3
#define RIGHT_STEP_PIN 4
#define RIGHT_DIR_PIN  5

AccelStepper leftMotor(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightMotor(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

const int base_speed = 1000;

void setMotorSpeeds(int left_speed, int right_speed) {
  leftMotor.setSpeed(left_speed);
  rightMotor.setSpeed(right_speed);
}

void moveForward(int speed) {
  setMotorSpeeds(speed, speed);
}

void moveBackward(int speed) {
  setMotorSpeeds(-speed, -speed);
}

void turnLeft(int left_speed, int right_speed) {
  setMotorSpeeds(left_speed, right_speed);
}

void turnRight(int left_speed, int right_speed) {
  setMotorSpeeds(left_speed, right_speed);
}

void setup() {
  leftMotor.setAcceleration(500);
  rightMotor.setAcceleration(500);

  leftMotor.setMaxSpeed(base_speed);
  rightMotor.setMaxSpeed(base_speed);

  moveForward(base_speed);
}

void loop() {
  leftMotor.runSpeed();
  rightMotor.runSpeed();
}
