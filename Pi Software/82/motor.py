#hi
import time
import us_werte
#unterfunktion



#hauptfunktion

us_sensor = us_werte.us_werte()

print ("start")

us = [1, 2, 3]



while True:
    
    us = us_sensor.lesen()
    print(us)
    us.sort()
    print(us)
    time.sleep(1)







print(us)