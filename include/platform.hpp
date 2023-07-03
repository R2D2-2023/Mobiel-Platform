#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <Arduino.h>
#include <AccelStepper.h>
#include <string.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include "sensors.hpp"

class Platform {
public:
  Platform(int left_step_pin, int left_dir_pin, int right_step_pin, int right_dir_pin, int base_speed);
  void setup();
  void loop();
private:
  MPU6050 mpu;
  AccelStepper left_motor;
  AccelStepper right_motor;

  enum class RobotState { DRIVE, STOP, BASE_IN, BASE_OUT, IDLE, MEASURE };
  RobotState current_state;

  int base_speed;
  float last_error;

  int period;
  unsigned long time_now;
  unsigned long previous_millis;

  int new_speed;
  int current_speed;
  float angle;
  int right_degrees;
  String current_direction;
  String last_direction;

  int sensor_1;
  int sensor_2;
  int sensor_3;
  Sensors climate_sensor = Sensors();

  void checkSerial();
  float getAngle();
  void setMotorSpeed(float left_wheel, float right_wheel);
  void sendCardinal();
  void rotateTo(int target_angle);
};

#endif