""" ***********************************************************
*   Beispiel für eine Zugriffsklasse
*
*   Übersicht über die Funktionen
*
*
*   ***********************************************************
"""

""" ***********************************************************
*   IMPORT um Modul bekannt zu machen
*   ***********************************************************
"""
import serial    #Serielle Schnittstelle ansprechbar machen
import time      #Zeitfunktionen nutzbar machen
import random
import sys       #fuer sys.exit

""" ***********************************************************
*   Klasse US_WERTE für den Ultraschall-Sensor an USB-Serial
*   ***********************************************************
"""
class us_werte:
    # Variablen der Klasse
    werte = [0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5]
    ser_usb = 'string'
    last_time = time.time()    # Zeit letztes Werte-Update
    timeout = 1                # Sekunden für Info
    timeerror = 5              # Sekunden bis Warnung
    timefatal = 20             # Sekunden bis Fehler
    
    
    # ********************************************************
    # Alle Funktionen der Klasse
    # ********************************************************

    # setze die Schnittstelle über die Daten erwartet werden
    def setup(self, us_interface, ctrl_debug):
        self.debug = ctrl_debug;

        # oeffne serial USB Device 
        try:
            zeichen = us_interface.in_waiting
        except:
            print("ERROR: us_werte: couldn't open serial interface!")
            sys.exit(0)

        self.ser_usb = us_interface
        self.last_time = time.time()
        return



    # lesen alle Zeichen aus der Schnittstelle und verarbeite sie,
    # wenn es eine ganze Zeile ist
    def update(self):
        for x in range(0,16):
            self.werte[x] = random.randint(0, 500)
            
        #self.last_time = time.time()
        return


    
    # gib die aktuellen Messwerte
    def read(self):
        if ((time.time() - self.last_time) > self.timefatal):
            print("ERROR: us_werte: values timed out fatal " + str(self.timefatal) + " s")
            sys.exit(0)
        elif ((time.time() - self.last_time) > self.timeerror):
            print("WARNING: us_werte: values timed out " + str(self.timeerror) + " s")
        elif ((time.time() - self.last_time) > self.timeout):
            print("INFO: us_werte: values close to timeout " + str(self.timeout) + " s")
            
        return self.werte
    


