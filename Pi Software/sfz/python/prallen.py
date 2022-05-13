
import math
import time
import board
import adafruit_bno055
import random
from adafruit_motorkit import MotorKit
import serial    #Serielle Schnittstelle ansprechbar machen
import sys     

from collections import UserList

i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)
last_val = 0xFFFF



message = ""
count = 0
timeout = 30
message_cut = ""

listval = 0
us_werte = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]







try:
   ser_usb = serial.Serial('/dev/ttyUSB0',115200, timeout=30)
except:
   print("[ERROR]: Konnte Serielle an \\def\\ttyUSB0 nicht öffnen!\n")
   print("         Ende der Software")
   sys.exit(0)




while ser_usb.in_waiting > 0:
   print("[DEBUG]: Zeichen zur Verarbeitung", ser_usb.in_waiting)
   byte_zeichen = ser_usb.read(1)
   string_zeichen = byte_zeichen.decode()
   if string_zeichen == '\n':
      break
   if string_zeichen == '\r':
      break

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

def wertelesen():
    

    message = ""
    count = 0
    timeout = 30
    message_cut = ""

    listval = 0
    us_werte = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]


    while True:
        timecnt = 0
        while ser_usb.in_waiting == 0:
            print(".",sep="",end="")
      
            timecnt = timecnt + 1
            time.sleep(1)
            if timecnt > timeout:
                print("[DEBUG]: timeout")
                timecnt = 0


   
        byte_zeichen = ser_usb.read(1)
        string_zeichen = byte_zeichen.decode()
        if (string_zeichen == '\n' or string_zeichen == '\r'):    # vollstaendige message gelesen
            
      
    
      

            if ser_usb.in_waiting > 0:
                byte_zeichen = ser_usb.read(1)
                string_zeichen = byte_zeichen.decode()
                if string_zeichen == '\n':
                    message = ""
                else:
                    message = string_zeichen

        else:   # message noch nicht vollstaendig, haenge das Zeichen an
            message = message + string_zeichen

            if string_zeichen == ' ':
                message = ''

            if string_zeichen == ';':
                i = message.find(";")
                message_cut = message[0:i]
            
            
                int(message_cut)
            
                us_werte[listval] = message_cut
                print(us_werte)
                message = ''
                listval += 1
                if listval == 15:
                    print(us_werte)
                    listval = 0
                    global us_werte1
                    us_werte1 = us_werte 
                    us_werte = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
                    print(us_werte1)
                    break
                        

wertelesen()









def prallen():
    kit.motor1.throttle = Rechts2
    kit.motor2.throttle = -abs(Links2)
    kit.motor3.throttle = Hinten2

time.sleep(1)

print(Rechts2)
print(Links2)
print(Hinten2)

while True:
    wertelesen
    #try/except für "none, none, none" error vom sensor
    min_value = min(us_werte)
    
        
    
    


    if min_value < 30:
        min_index = us_werte.index(min_value)
        print(min_index)


    else:
        min_index = 17


    if 0<= min_index <= 3:
        userinput = 90

    if 4<= min_index <= 7:
        userinput = 180
    
    if 8<= min_index <= 11:
        userinput = 270

    if 12<= min_index <= 16:
        userinput = 359

    elif min_index == 17:
        userinput = userinput

    motorcalc()

    try:

        prallen()
        time.sleep(3)
        stop()
        
        

    except:

        #print("Euler angle: {}".format(sensor.euler))

        time.sleep(0.1)

        
ser_usb.close()  # den USB Seriellen Port wieder freigeben
 
print("--- Ende der Software ---")
        

        

        



    

  





