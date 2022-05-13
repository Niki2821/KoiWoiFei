


import math
import time
import board
import adafruit_bno055
import random
from adafruit_motorkit import MotorKit
#from matplotlib.style import use
from collections import UserList

i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)
last_val = 0xFFFF





userinput = input("Grad bitte: ")
userinput = int(userinput)



kit = MotorKit(i2c=board.I2C())


ML = 300
MR = 60
MH = 180
xml = userinput + ML
xmr = userinput + MR
xmh = userinput + MH
xbmr = math.radians(xmr)
xbml = math.radians(xml)
xbmh = math.radians(xmh)
Rechts = math.sin(xbmr)
Links = math.sin(xbml)
Hinten = math.sin(xbmh)

Rechts2 = round(Rechts, 2)
Links2 = round(Links, 2)
Hinten2 = round(Hinten, 2)

if  0.60<= Hinten2 <= 0.90:
   Hinten2 = 1.00

if  0.60<= Links2 <= 0.90:
   Links2 = 1.00

if  0.60<= Rechts2 <= 0.90:
    Rechts2 = 1.00


kompass = str(format(sensor.euler))



kompass1 = kompass[1:2]

int(kompass1)


def motorcalc():
    ML = 300
    MR = 60
    MH = 180

    xml = userinput + ML
    xmr = userinput + MR
    xmh = userinput + MH

    xbmr = math.radians(xmr)
    xbml = math.radians(xml)
    xbmh = math.radians(xmh)

    Rechts = math.sin(xbmr)
    Links = math.sin(xbml)
    Hinten = math.sin(xbmh)




def stop():
    kit.motor3.throttle = 0
    kit.motor2.throttle = 0
    kit.motor1.throttle = 0


def drehen():

    kit.motor3.throttle = 0.7
    kit.motor2.throttle = -0.7
    kit.motor1.throttle = 0.7

def prallen():
    kit.motor1.throttle = Rechts2
    kit.motor2.throttle = -abs(Links2)
    kit.motor3.throttle = Hinten2





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

print(Rechts2)
print(Links2)
print(Hinten2)

while True:

    #try/except für "none, none, none" error vom sensor

    try:

        prallen()
        time.sleep(7)
        stop()
        break
        

    except:

        #print("Euler angle: {}".format(sensor.euler))

        time.sleep(0.1)

        

        

        

        



    

  





