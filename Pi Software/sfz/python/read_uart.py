""" ******************************************************************
*
*   Software um Serielle Daten aus einem Seriellen USB-Interface zu
*   lesen
*
*   ******************************************************************
*   Ersteller: Markus
*
*   ******************************************************************
*   Version 0.1  Alles lesen und in eine Datei loggen
*   2020-04-26   
*   ******************************************************************
"""
import serial    #Serielle Schnittstelle ansprechbar machen
import sys       #fuer sys.exit
import time      #Zeitfunktionen nutzbar machen
import datetime  #timestamp richtig formatieren

""" ******************************************************************
* global service function timestamp
* function return a string with day and time in the following format:
* 
* NOTE: function requires import of time and datetime module
*   ******************************************************************
"""
def timestamp():
   ctime = time.localtime()
   jahr, monat, tag = ctime[0:3]
   stunde, minute, sec = ctime[3:6]
   d = datetime.datetime(jahr,monat,tag,stunde,minute,sec)
   day_str=str('{:%Y-%m-%d %H:%M:%S}'.format(d))
   return day_str

"""
*   Beginn der Hauptfunktion
"""

print("""Seriel Logging V0.1 2021-04-24
Lese Serielle Data und schreibe mit Zeitstempel\n""")

# oeffne serial USB Device 
try:
   ser_usb = serial.Serial('/dev/ttyUSB0',115200, timeout=30)
except:
   print("[ERROR]: Konnte Serielle an \\def\\ttyUSB0 nicht öffnen!\n")
   print("         Ende der Software")
   sys.exit(0)

print("[DEBUG]: Serieller Port " + ser_usb.name + " wurde geöffnet.")

# oeffne eine log-Datei mit dem Datum im Namen
ctime = time.localtime()
jahr, monat, tag = ctime[0:3]
stunde, minute, sec = ctime[3:6]
logfile=str("%4d-%2d-%2d_%2dh%2d_log.txt" % (jahr,monat,tag,stunde,minute))
try:
   log = open(logfile,"w")
except:
   print("[ERROR]: Konnte Log-File", logfile,"nicht öffnen!\n")
   print("         Ende der Software")
   sys.exit(0)
   
print("[DEBUG]: Log-File", logfile," wurde geöffnet.")
log.write(";  Serielles Log       ")
log.write(timestamp())
log.write("\n")
log.close()

# lese daten und loesche alle Zeichen vor einem Message-Start (neue Zeile)
while ser_usb.in_waiting > 0:
   print("[DEBUG]: Zeichen zur Verarbeitung", ser_usb.in_waiting)
   byte_zeichen = ser_usb.read(1)
   string_zeichen = byte_zeichen.decode()
   if string_zeichen == '\n':
      break
   if string_zeichen == '\r':
      break

# lese Zeile Message
message = ""
count = 0
timeout = 30
#if ser_adsb.in_waiting > 0:
runADSB = 1
#else:
#   runADSB = 0

#while ser_adsb.in_waiting > 0:
while runADSB:
   timecnt = 0
   while ser_usb.in_waiting == 0:
      print(".",sep="",end="")
      # time.sleep(2)  # warte bis neue Message kommt
      timecnt = timecnt + 1
      if timecnt > timeout:
         print("[DEBUG]: Heartbeat")
         log = open(logfile,"a")
         log.write(";  Heartbeat ")
         log.write(timestamp())
         log.write("\n")
         log.close()
         timecnt = 0

      # wenn Zeichen kommen, wirf das erste weg...
      #if ser_usb.in_waiting > 0:
       #  byte_zeichen = ser_usb.read(1)
       #  print(" ")    #ein Leerzeichen für einen Zeilenumbruch
   
   byte_zeichen = ser_usb.read(1)
   string_zeichen = byte_zeichen.decode()
   if (string_zeichen == '\n' or string_zeichen == '\r'):    # vollstaendige message gelesen
      # mache einen Eintrag in das LOG-File
      log = open(logfile,"a")
      log.write(message)
      #if string_zeichen == '\n' : log.write("n ")
      #if string_zeichen == '\r' : log.write("r ") 
      log.write(timestamp())
      log.write("\n")
      log.close()

      if ser_usb.in_waiting > 0:
         byte_zeichen = ser_usb.read(1)
         string_zeichen = byte_zeichen.decode()
         if string_zeichen == '\n':
            message = ""
         else :
            message = string_zeichen

   else:   # message noch nicht vollstaendig, haenge das Zeichen an
      message = message + string_zeichen

print("[DEBUG]: Zeichenkette leer")

# Ende der Software
ser_usb.close()  # den USB Seriellen Port wieder freigeben
log.close()      # schliese das Message log-file
print("--- Ende der Software ---")



      
