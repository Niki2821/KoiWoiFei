""" ***********************************************************
*   Beispiel für einen Zugriff auf ein eigenes externes Modul
*   ***********************************************************
"""

""" ***********************************************************
*   IMPORT um Modul bekannt zu machen
*   ***********************************************************
"""
import serial    #Serielle Schnittstelle ansprechbar machen
import sys       #fuer sys.exit
import time      #Zeitfunktionen nutzbar machen
import us_werte_1

""" ***********************************************************
*   GLOBALE VARIABLEN
*   ***********************************************************
"""


""" ***********************************************************
*   LOKALE FUNKTIONEN
*   ***********************************************************
"""
# sende TRUE wenn einer der US-Werte kleiner Schwellwert ist, sonst
# sende FALSE
def is_us_alert(us_values):
    result = False
    return result

# bestimme die geringeste Distanz bei einem US-Sensor-Wert
def get_us_dist(us_values):
    result = 500
    return result

# in welcher Richtung befindet sich die geringste Entfernung?
def get_us_direction(us_values):
    n = 6;
    return (n * 22.5) 

""" ***********************************************************
*   HAUPTFUNKTION
*   ***********************************************************
"""

#Variablen
us_sensor = us_werte.us_werte()
us_values = [1, 2, 3]

print("Beispiel Ultraschalle-Werte auslesen")

# oeffne serial USB Device 
try:
   ser_usb = serial.Serial('/dev/ttyUSB0',115200, timeout=30)
except:
   print("ERROR: Konnte Serielle an \\def\\ttyUSB0 nicht öffnen!")
   sys.exit(0)
   
print("INFO: Serieller Port " + ser_usb.name + " wurde geöffnet.")

us_sensor.setup(ser_usb)

# mach es für immer
while True:
    # aktualisiere alle Sensor-Auswertungen
    us_sensor.update()
    #line_sensor.update()
    #nav_sensor.update()
    
    # lese die aktuellen Messwerte
    us_values = us_sensor.read()
    print(us_values)
    #line_values = line_sensor.read()
    #...

    # mache die Auswertung
    # für Ultraschall Sensoren
    if is_us_alert(us_values):
        us_dist = get_us_dist(us_values)
        us_direction = get_us_direction(us_values)
        print("Richtung: " + str(us_direction) + " Entfernung: " + str(us_dist) + " cm")
    # Line Sensor auswerten
    # ...
    
    # setzte die neue Richtung / Motorleistung
    # ... hier fehlt noch alles
    print("next loop")
    time.sleep(1)

