#include <HCSR04.h>

UltraSonicDistanceSensor diSen1 (3,2);
UltraSonicDistanceSensor diSen2 (6,5);
UltraSonicDistanceSensor diSen3 (8,7);
UltraSonicDistanceSensor diSen4 (10,9);

#include <Adafruit_NeoPixel.h>

#define BOARD_PIN    4       // Digital IO pin connected to the NeoPixels.
#define BOARD_COUNT  9       // Number of Pixel in stripe

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel board = Adafruit_NeoPixel(BOARD_COUNT, BOARD_PIN, NEO_GRB + NEO_KHZ800);

int  sensor1 = 500 ;
int  sensor2 = 400 ;
int  sensor3 = 300 ;
int  sensor4 = 150 ;
uint32_t color[] = {0x00000000,0x00000050,0x00101050,0x00300040,
                    0x00005000,0x00105010,0x00502000,0x00500000,
                    0x00304000,0x00404000,0x00500035,0x00800000};  

// Init the Hardware of the Board
void setup() {

  delay (500);  // warte nach PowerOn, damit die Serielle zum Programmieren frei ist.
  int blinken = 100;

  // init debug serial
  Serial.begin(9600);
  Serial.print("9-LED Board wird initialisiert ...");
  
  // initialize the board animation LED values and the lib
  board.begin();
  board.show();   // Initialize all pixels to 'off'

  Serial.println(" fertig");

  // LED's abwechselnd blinken 
  int j;
  for(j=0; j<25; j++) {  
    int i;
    for (i=0; i<8; i=i+2) {
      board.setPixelColor(i, color[7]); 
      board.setPixelColor(i+1, color[1]); 
    } 
    board.show();   // Initialize all pixels to 'off'
    delay(blinken);
    for (i=0; i<8; i=i+2) {
      board.setPixelColor(i, color[1]); 
      board.setPixelColor(i+1, color[7]); 
    } 
    board.show();   // Initialize all pixels to 'off'
    delay(blinken);
  }
}

// What to do continously
void loop() {
  int loop_delay = 0;


 // update the colors of the strip
  boardCountDown();

  Serial.println(" ... ");
  // show the colors and wait 
  board.show();
  delay(loop_delay);  

}



/* *************************************************************************************** *
 * LED Fuktionen
 * *************************************************************************************** */

#define MAX_COLOR 8

// Farbe abhängig von Entfernung
// Entfernung in cm 
// Rückgabewert ist Farbindex (0-7)
int getFarbe(int abstand){
   if (abstand>350) return 0;
   if (abstand>270) return 1;
   if (abstand>200) return 2;
   if (abstand>140) return 3;
   if (abstand>90)  return 4;
   if (abstand>40)  return 5;
   if (abstand>20)  return 6;
   return 7;
}

void boardCountDown() {
  int i;
  int farbe1;
  int farbe2;
  int farbe3;
  int farbe4;
   
  sensor1 = diSen1.measureDistanceCm();
//  if (sensor1 <0) sensor1 = 500 ;
  farbe1 = getFarbe(sensor1);
  //Ausgabe für seriellen Monitor(nur sensor1)


    sensor2 = diSen2.measureDistanceCm();
//  if (sensor2 <0) sensor2 = 500 ;
  farbe2 = getFarbe(sensor2);

    sensor3 = diSen3.measureDistanceCm();
//  if (sensor3 <0) sensor3 = 500 ;
  farbe3 = getFarbe(sensor3);

    sensor4 = diSen4.measureDistanceCm();
//  if (sensor4 <0) sensor4 = 500 ;
  farbe4 = getFarbe(sensor4);



  // Setze das Muster und die Farbe fuer die Ausgabe
  for (i=0; i<2; i++) {
    board.setPixelColor(i, color[farbe1]);  
  }

  for (i=2; i<4; i++) {
    board.setPixelColor(i, color[farbe2]);  
  }

  for (i=4; i<6; i++) {
    board.setPixelColor(i, color[farbe3]);  
  }

  for (i=6; i<8; i++) {
    board.setPixelColor(i, color[farbe4]);  
  }


  Serial.print (sensor1);
  Serial.print ("; ");
  Serial.print (sensor2);
  Serial.print ("; ");
  Serial.print (sensor3);
  Serial.print ("; ");
  Serial.println (sensor4);
}
