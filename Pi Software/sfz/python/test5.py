# MOTORTESTPROGRAMM

"""Simple test for using adafruit_motorkit with a DC motor"""
import time
import board
from adafruit_motorkit import MotorKit

kit = MotorKit(i2c=board.I2C())

kit.motor1.throttle = 1.0
kit.motor2.throttle = 0
kit.motor3.throttle = 0
print("Motor1")
time.sleep(4)
kit.motor1.throttle = 0
kit.motor2.throttle = 1.0
kit.motor3.throttle = 0
print("Motor2")
time.sleep(4)
kit.motor1.throttle = 0
kit.motor2.throttle = 0
kit.motor3.throttle = 1.0
print("Motor3")
time.sleep(4)
kit.motor1.throttle = 0
kit.motor2.throttle = 0
kit.motor3.throttle = 0