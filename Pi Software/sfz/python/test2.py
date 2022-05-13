

import time
import board
import adafruit_bno055
import random
from adafruit_motorkit import MotorKit

i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)
last_val = 0xFFFF


userinput = input("Grad bitte: ")
userinput = int(userinput)




kit = MotorKit(i2c=board.I2C())



kompass = str(format(sensor.euler))

kompass1 = kompass[1:2]
int(kompass1)

def stop():
    
    kit.motor3.throttle = 0
    kit.motor2.throttle = 0
    kit.motor1.throttle = 0


def drehen():

    kit.motor3.throttle = 0.7
    kit.motor2.throttle = -0.7
    kit.motor1.throttle = 0.7



drehen()
time.sleep(0.5)
stop()

def drehenach():
    kompass = str(format(sensor.euler))   

        
    i = kompass.find(".")
    kompass1 = kompass[1:i]
    
    print(int(kompass1))

    if int(kompass1) >=userinput-20 and int(kompass1) <=userinput:
        stop()
        

    elif int(kompass1) < 360:
        drehen()


time.sleep(1)




while True:
    
    #try/except für "none, none, none" error vom sensor
    
    try:

        drehenach()
        break
        
    except:

        
        #print("Euler angle: {}".format(sensor.euler))
        time.sleep(0.1)
        
        
        
        

    
  


