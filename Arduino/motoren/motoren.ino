/*
* Simple code for the L298N motor controller
* and Metal Gearmotor 37Dx52L mm with 64 CPR Encoder 
*/
#include <L298N.h>

// motor pin definition
#define M_EN 9
#define M_IN1 8
#define M_IN2 7
#define M_EN1 12
#define M_IN3 13
#define M_IN4 2

// create a motor instance
L298N motor1(M_EN, M_IN1, M_IN2);
L298N motor2(M_EN1, M_IN3, M_IN4);
// PWM signal for the motor driver
unsigned short driverPwm = 255;

// variables for sending information frequency configuration
long previousMillis = 0; 
long interval = 200;  

void setup() {
  Serial.begin(57600);
  Serial.println("Basic Motor Test:");
}

void loop() {

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval){
    Serial.print("PWM value: ");
    Serial.println(driverPwm);
    previousMillis = currentMillis;   
  }

  // check if the data has been sent from the computer:
  // to make this code work properly, select "No line ending" in monitor
  if (Serial.available()) {
    // Narrowing convertion without check! Don't do this in production:)
    driverPwm = Serial.parseInt();
  }

  motor1.setSpeed(driverPwm);
  motor1.forward(); 
  motor2.setSpeed(driverPwm);
  motor2.forward();
}