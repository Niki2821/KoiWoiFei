import serial    #Serielle Schnittstelle ansprechbar machen
import sys       #fuer sys.exit
import time      #Zeit

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
      print(message)
      
    
      

      if ser_usb.in_waiting > 0:
         byte_zeichen = ser_usb.read(1)
         string_zeichen = byte_zeichen.decode()
         if string_zeichen == '\n':
            message = ""
         else :
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
               us_werte1 = us_werte 
               us_werte = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
               print(us_werte1)
               break
               
               
            
        
    


    

print("[DEBUG]: Zeichenkette leer")

# Ende der Software
ser_usb.close()  # den USB Seriellen Port wieder freigeben
 
print("--- Ende der Software ---")