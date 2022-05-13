import time
import board
import random
from adafruit_motorkit import MotorKit
userinput = input("What: ")


randomspeed = random

kit = MotorKit(i2c=board.I2C())

def drehen():

    kit.motor3.throttle = 0.7
    kit.motor2.throttle = -0.7
    kit.motor1.throttle = 0.7

    time.sleep(2)

    kit.motor3.throttle = 0
    kit.motor2.throttle = 0
    kit.motor1.throttle = 0

def nacheindander():
    kit.motor3.throttle = 0.5
    time.sleep(2)
    kit.motor3.throttle = 0
    kit.motor1.throttle = 0.5
    time.sleep(2)
    kit.motor1.throttle = 0
    kit.motor2.throttle = -0.5
    time.sleep(2)
    kit.motor2.throttle = 0

def wegfahren():
    kit.motor3.throttle = 0.8
    kit.motor1.throttle = -0.8
    time.sleep(3)
    kit.motor1.throttle = 0
    kit.motor3.throttle = 0

def hinher():
    kit.motor1.throttle = 0.8
    kit.motor2.throttle = 0.8
    time.sleep(2)
    kit.motor1.throttle = -0.8
    kit.motor2.throttle = -0.8
    time.sleep(2)




