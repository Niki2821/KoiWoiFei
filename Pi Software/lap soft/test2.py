
import time
import board
import adafruit_bno055
import random
from adafruit_motorkit import MotorKit

i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)
last_val = 0xFFFF


#userinput = input("Grad bitte: ")
#userinput = int(userinput)


randomspeed = random

kit = MotorKit(i2c=board.I2C())



kompass = str(format(sensor.euler))

kompass1 = kompass[1:2]
int(kompass1)

def stop():
    
    kit.motor3.throttle = 0
    kit.motor2.throttle = 0
    kit.motor1.throttle = 0


def drehen():

    kit.motor3.throttle = 0.6
    kit.motor2.throttle = -0.6
    kit.motor1.throttle = 0.6



drehen()
time.sleep(0.5)
stop()



time.sleep(1)



while True:
    
    print(int(kompass1))

    kompass = str(format(sensor.euler))
    i = kompass.find(".")
    kompass1 = kompass[1:i]
    int(kompass1)


    if int(kompass1) 355 and 356 and 357 and 358 and 359 and 360 and 0 and 1 and 2 and 3 and 4 and 5:
        stop()

    elif int(kompass1) < 360:
        drehen()








    #print("Accelerometer (m/s^2): {}".format(sensor.acceleration))
    print("Euler angle: {}".format(sensor.euler))
    #print("Quaternion: {}".format(sensor.quaternion))
    #print("Linear acceleration (m/s^2): {}".format(sensor.linear_acceleration))
   
  


