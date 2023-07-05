from Library import LidarX2
import time
import math
import statistics
import serial
lidar = LidarX2("/dev/ttyUSB1")
import cv2
import os
import numpy as np
from Library import LidarX2
import statistics  # Name of the serial port, can be /dev/tty*, COM*, etc.


if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
    ser.reset_input_buffer()

blocked = 0

arduino_react = False 
last_dir = None
if not lidar.open():
        print("Cannot open lidar")
        exit(1)

while True:
    t = time.time()
    direction = ser.readline().decode('utf-8').rstrip()
    
    if(last_dir != direction and direction != ""):
        print(direction)
    
    count = 0
    
    list=[]
    t = time.time()
    for i in range(13):
        measures = lidar.getMeasures()  # Get latest lidar measures
        if len(measures)>=100:
            list.extend(measures)
        time.sleep(0.01)
    distance=[]
    degrees=[]
    
    img = np.ones((480, 640))
    for i in list:
        x=str(i).split(":")    
        degrees.append(float(x[0]))
        distance.append(float(x[1].split("mm")[0]))

    count = 0
    count2 = 0 
    median = []
    minimum = []
     
    for j in range(len(degrees)):
        if degrees[j] < 30 or degrees[j] > 330:
            count = count + 1
            if distance[j] != 0:
                median.append(distance[j])
            if len(median) != 0:
                if count == 5:
                    minimum.append(statistics.median(median))
                    count2 = count2 + 1
                    count = 0
                    median = []
                    if count2 == 5:
                        if blocked == 1 and min(minimum) > 400:
                            blocked = 0

                            print("rijden")
                            ser.write(b"0\n")
                            count2 = 0
                        elif min(minimum) < 400 and blocked == 0:
                            blocked = 1
                            print("plz stop :(")
                            ser.write(b"1\n")
                            count2 = 0

