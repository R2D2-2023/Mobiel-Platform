#include "platform.hpp"

// Define pin numbers for motor control
const int LEFT_STEP_PIN = 2;
const int LEFT_DIR_PIN = 3;
const int RIGHT_STEP_PIN = 4;
const int RIGHT_DIR_PIN = 5;

// Define base speed for the robot
const int BASE_SPEED = 3000;

// Create a Robot object
Platform platform(LEFT_STEP_PIN, LEFT_DIR_PIN, RIGHT_STEP_PIN, RIGHT_DIR_PIN, BASE_SPEED);

void setup() {
  // Call the setup function of the Robot
  platform.setup();
}

void loop() {
  // Call the loop function of the Robot
  platform.loop();
}

