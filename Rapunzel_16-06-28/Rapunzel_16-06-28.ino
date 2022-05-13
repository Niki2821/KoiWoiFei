/* RoboCup Junior, Dance
 *  Jasmin Thanner, Salomé Abrantes
 *  erstellt am: 07.06.2016
 *  aktuelle Version vom: 21.06.2016
 */

 
#include <MeBaseBoard.h>
#include <SoftwareSerial.h>

MeInfraredReceiver infraredReceiverDecode(PORT_3);
MeDCMotor motorR(M1);                 //rechter Motor an Buchse M1
MeDCMotor motorL(M2);                 //linker Motor an Buchse M2
MeDCMotor motorA(PORT_1);             //arm Motor 
MeLineFollower lichtSensor(PORT_6);   //Lichtsensor Me-Line Finder

#define NICHTS 0
#define FAHREN 1
#define HAARE  2
#define HOCH   3
#define RUNTER 4

void setup()
{
  infraredReceiverDecode.begin();
  Serial.begin(9600);
  Serial.println("InfraredReceiverDecode Start!");
}

void loop()
{
  uint8_t ReceiverCode;
  uint8_t buttonState;
  static uint8_t PrebuttonState = 0;
  static uint8_t Rapunzelstatus = NICHTS;
  static uint8_t Rapunzelvorher = NICHTS;

  /* aktionsblock
   * abhängig von Rapunselstatus werden verschiedene funktionen ausgeführt
  */
  if (Rapunzelvorher != Rapunzelstatus)
  {
    Rapunzelvorher = Rapunzelstatus;
    switch(Rapunzelstatus)
    {
      case NICHTS: Serial.println("Rapunzel:NICHTS.");break;
      case HOCH  : Serial.println("Rapunzel:HOCH.");
         motorA.run(100);
         delay(1000);
         motorA.stop();
      break;
      case RUNTER: Serial.println("Rapunzel:RUNTER.");
         motorA.run(-100);
         delay(1000);
         motorA.stop();
      break;
      case FAHREN: Serial.println("Rapunzel:FAHREN.");
         motorR.run(-100);                      //geradeaus
         motorL.run(-100);
         break; 
      case HAARE:  Serial.println("Rapunzel:HAARE.");
          motorR.stop();      //Motor stopp
          motorL.stop();
          break;
      default:     Serial.println("Rapunzel:fehler"); break;
    }
  }

/*ereignissblock
 * IR eingaben
 */
  
  
  /*buttonState = infraredReceiverDecode.buttonState();
  if(PrebuttonState != buttonState)
  {
    PrebuttonState = buttonState;
    Serial.print("buttonState 0x");
    Serial.println(buttonState);
  }
  */
  if(infraredReceiverDecode.available())
  {
    ReceiverCode = infraredReceiverDecode.read();
    switch(ReceiverCode)
    {
       case IR_BUTTON_0: Serial.println("Press 0."); Rapunzelstatus = FAHREN; break;
       case IR_BUTTON_1: Serial.println("Press 1."); Rapunzelstatus = HAARE;  break;
       case IR_BUTTON_4: Serial.println("Press 4."); Rapunzelstatus = HOCH;  break;
       case IR_BUTTON_7: Serial.println("Press 7."); Rapunzelstatus = RUNTER;  break;
       default: break;
    }
  }
}

