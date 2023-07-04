#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <Arduino.h>
#include <AccelStepper.h>
#include <string.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include "sensors.hpp"

     /**
     * @brief 
     * Main class for running the Platform
     */
class Platform {
public:
/**
   * @brief Construct a new Platform object
   * 
   * @param left_step_pin 
   * @param left_dir_pin 
   * @param right_step_pin 
   * @param right_dir_pin 
   * @param base_speed 
   */
  Platform(int left_step_pin, int left_dir_pin, int right_step_pin, int right_dir_pin, int base_speed);
  /**
   * @brief setup for arduino 
   * 
   */
  void setup();
  /**
   * @brief main loop for the arduino
   * 
   */
  void loop();
  /**
   * @brief check the serial of the LiDAR and look if the lidar detects an object
   * 
   */
  void checkSerial();
  /**
   * @brief gets the Yaw of the MPU6050
   */
  float getAngle();
  /**
     * @brief sets the motor speed for both motors
     * 
     * @param left_wheel
     * @param right_wheel 
     */
  void setMotorSpeed(float left_wheel, float right_wheel);
  /**
   * @brief This function sends cardinal directions to PI
   * 
   */

  void sendCardinal();
  /**
   * @brief rotate the motors so the platform  corrects its self/turn the car to the designated angle
   * 
   * @param target_angle 
   */
  void rotateTo(int target_angle);
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

};

#endif