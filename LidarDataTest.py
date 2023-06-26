from lidarsetup import LidarX2
import time
import numpy
import math
import statistics
import serial
lidar = LidarX2("/dev/ttyUSB0")  # Name of the serial port, can be /dev/tty*, COM*, etc.


if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.reset_input_buffer()

blocked = 0
ser.write("BOOTED\n")
while True:
    # print("hai")
    if not lidar.open():
        print("Cannot open lidar")
        exit(1)
    t = time.time()
    lijst=[]
    for i in range(22):
        measures = lidar.getMeasures()  # Get latest lidar measures
        if len(measures)>=270:
            lijst.extend(measures)
        time.sleep(0.015)
    lidar.close()
    distance=[]
    degrees=[]
    img = numpy.ones((480, 640))
    for i in lijst:
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
            # if distance[j] < 400:
            if len(median) != 0:
                if count == 15:
                    minimum.append(statistics.median(median))
                    count2 = count2 + 1
                    count = 0
                    median = []
                    if count2 == 5:
                        if blocked == 1 and min(minimum) > 400:
                            blocked = 0
                            print(degrees)
                            ser.write(b"0\n")
                            count2 = 0
                        elif min(minimum) < 400 and blocked == 0:
                            blocked = 1
                            ser.write(b"1\n")
                            count2 = 0
