# Mobile Platform 
Welcome to the Mobile Platform with C++ and Python Integration! This platform combines the power of the C++ and Python programming languages to drive a mobile robot capable of obstacle detection using a Lidar sensor and determining its orientation using a gyroscope.

<div style="text-align: center">
<img src= "Assets/Images/Mobileplatform.jpg" width="200"/>
</div>

## Overview
The mobile platform serves as a foundation for building autonomous robotic systems that require advanced capabilities like obstacle detection by using a Lidar sensor. By integrating C++ and Python, developers can leverage the strengths of both languages to create robust and efficient applications. This platform utilizes the Arduino Mega 2560 for controlling stepper motors and the MPU6050 sensor for orientation sensing, and it communicates with a Raspberry Pi using the UART protocol.

## Features
1. **Obstacle Detection**: The platform is equipped with a Lidar (Light Detection and Ranging) sensor, which uses laser beams to measure distances and create a 2D representation of the environment. This data is processed in C++ to detect obstacles and enable collision avoidance.
    
2. **Gyroscope for Orientation**: The platform incorporates the MPU6050 sensor to determine its orientation and motion. The MPU6050 sensor, combined with the appropriate libraries, provides accurate measurement of angular velocity and acceleration, enabling precise control of the robot's movements.
   
3. **Stepper Motor Control**: The platform utilizes the AccelStepper library to control the stepper motors. This library allows for precise control of motor movements, including speed, acceleration, and position control.
  
4. **Sensor Libraries**: The platform utilizes the MPU6050 library to interface with the MPU6050 sensor. This library provides functions for retrieving sensor data such as accelerometer and gyroscope readings.
 
5. **Arduino Mega 2560 and Raspberry Pi**: The mobile platform is built around the Arduino Mega 2560 microcontroller board for motor control and sensor interfacing. Additionally, it communicates with a Raspberry Pi using the I2C protocol, enabling seamless integration with the Pi for higher-level processing tasks.

6.  **Wire Library**: The Wire library is used to establish I2C communication between the Arduino Mega 2560 and the Raspberry Pi. This library provides functions for transmitting and receiving data over the I2C bus, allowing for reliable communication between the two devices.
    

## Getting Started

To get started with the Mobile Platform with C++ and Python Integration, follow these steps:

1. **Hardware Setup**: Set up the mobile platform by connecting the stepper motors, Lidar sensor, MPU6050 sensor, Arduino Mega 2560, and Raspberry Pi. Ensure all connections are correctly made and properly powered.

2. **Software Installation**: Install the required software development tools, libraries, and drivers for both C++ and Python. This may include compilers, build systems, sensor libraries, and communication frameworks. Install the AccelStepper library for motor control, the MPU6050 library for interfacing with the MPU6050 sensor and the YDLidarX2_python library for controlling the Lidar.

3. **Code Development**: Develop your application using either C++ or Python, or a combination of both. Utilize the provided libraries, including AccelStepper, MPU6050, arduino and YDLidarX2_python to control the robot's movements, retrieve sensor data, and implement obstacle detection algorithms.

4. **Testing and Deployment**: Test your application on the mobile platform to ensure proper functionality. Once satisfied, deploy the code to the Arduino Mega 2560 and Raspberry Pi and observe the robot's performance in real-world scenarios.


## Authors
- [@RobinKret](https://github.com/RobinKret)
- [@jurgen1507](https://www.github.com/jurgen1507)
- [@Jordydjong](https://github.com/Jordydjong)
- [@DvnGBulletZz](https://github.com/DvnGBulletZz)
- [@ANV152](https://github.com/ANV152)

## License
[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)



## Acknowledgements
- [MPU6050_Light](https://github.com/rfetick/MPU6050_light) - Open Source MPU6050 Library made by rfetick
- [AccelStepper](https://github.com/waspinator/AccelStepper)  - Open Source Stepper motor Library made by waspinator
- [YDLidarX2_python](https://github.com/nesnes/YDLidarX2_pythons)  - Open Source Lidar Library made by nesnes
- [Arduino](https://github.com/arduino/ArduinoCore-avr/tree/master)  - Open Source Arduino Library by Arduino
