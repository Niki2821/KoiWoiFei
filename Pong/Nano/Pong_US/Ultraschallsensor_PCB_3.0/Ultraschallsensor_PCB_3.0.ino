//US-Nano v.3.0
#include <HCSR04.h>

UltraSonicDistanceSensor diSen2 (3, 2);
UltraSonicDistanceSensor diSen1 (5, 6);
UltraSonicDistanceSensor diSen3 (8, 7);
UltraSonicDistanceSensor diSen4 (10, 9);

#include <Adafruit_NeoPixel.h>

#define BOARD_PIN    4
#define BOARD_COUNT  9

Adafruit_NeoPixel board = Adafruit_NeoPixel(BOARD_COUNT, BOARD_PIN, NEO_GRB + NEO_KHZ800);

int del = 50;
int mode = 2;
int  sensor1 = 500 ;
int  sensor2 = 400 ;
int  sensor3 = 300 ;
int  sensor4 = 150 ;
uint32_t color[] = {0x00000000, 0x00000050, 0x00101050, 0x00300040,
                    0x00005000, 0x00105010, 0x00502000, 0x00500000,
                    0x00304000, 0x00404000, 0x00500035, 0x00800000
                   };

void setup() {

  delay (500);  // warte nach PowerOn, damit die Serielle zum Programmieren frei ist.
  int blinken = 100;

  Serial.begin(115200);
  Serial.print("init ...");

  board.begin();
  board.show();

  Serial.println(" ...done v.3.0");

  //abwechselnd blinken (r,b)
  int j;
  for (j = 0; j < 25; j++) {
    int i;
    for (i = 0; i < 8; i = i + 2) {
      board.setPixelColor(i, color[7]);
      board.setPixelColor(i + 1, color[1]);
    }
    board.show();
    delay(blinken);
    for (i = 0; i < 8; i = i + 2) {
      board.setPixelColor(i, color[1]);
      board.setPixelColor(i + 1, color[7]);
    }
    board.show();
    delay(blinken);
  }
}
int zeichen1 = 0;

void loop() {
  //MASTER MODE, gibt festen Ausgabetakt vor
  if (mode == 2) {
    delay(70);
  }

  //SLAVE MODE, wartet bis Zeichen da sind
  if (mode == 1) {
    int count = 0;
    while (( Serial.available() == false) && count < 150) {
      delay(1);
      count ++;
      if(count == 150){ mode == 2;}
    }
  }

  messen();

  board.show();
  if (Serial.available()) {
    mode = 1; //Nicht der erste, Slave Mode
  }

  while ((Serial.available() > 0)) {

    zeichen1 = Serial.read();
    if (zeichen1 == '\n') {
      //Serial.print("<NL>; ");
    }
    else if (zeichen1 == '\r') {
      //Serial.print("<CR>");
    }

    else {
      Serial.write(zeichen1);
    }
  }

  //ausgabe sensorwerte
  if (mode == 2) {
    Serial.print("usw> ");
  }
  Serial.print (sensor4);
  Serial.print ("; ");
  Serial.print (sensor3);
  Serial.print ("; ");
  Serial.print (sensor2);
  Serial.print ("; ");
  Serial.print (sensor1);
  Serial.println ("; ");
}


#define MAX_COLOR 8

// Farbe abhängig von Entfernung, Entfernung in cm, Rückgabewert ist Farbindex (0-7)
int getFarbe(int abstand) {
  if (abstand > 200) return 1;  //blau
  if (abstand > 150) return 2;  //fastblau
  if (abstand > 100) return 3;  //violett
  if (abstand > 70)  return 4;  //grün
  if (abstand > 40)  return 5;  //grün weiß
  if (abstand > 20)  return 6;  //orange
  return 7;                     //rot
}

void messen() {
  int i;
  int farbe1;
  int farbe2;
  int farbe3;
  int farbe4;

  sensor1 = diSen1.measureDistanceCm();
  if (sensor1 == -1) {
    sensor1 = 500;
  }
  farbe1 = getFarbe(sensor1);

  sensor2 = diSen2.measureDistanceCm();
  if (sensor2 == -1) {
    sensor2 = 500;
  }
  farbe2 = getFarbe(sensor2);

  sensor3 = diSen3.measureDistanceCm();
  if (sensor3 == -1) {
    sensor3 = 500;
  }
  farbe3 = getFarbe(sensor3);

  sensor4 = diSen4.measureDistanceCm();
  if (sensor4 == -1) {
    sensor4 = 500;
  }
  farbe4 = getFarbe(sensor4);


  for (i = 0; i < 2; i++) {
    board.setPixelColor(i, color[farbe1]);
  }

  for (i = 2; i < 4; i++) {
    board.setPixelColor(i, color[farbe2]);
  }

  for (i = 4; i < 6; i++) {
    board.setPixelColor(i, color[farbe3]);
  }

  for (i = 6; i < 8; i++) {
    board.setPixelColor(i, color[farbe4]);
  }
}
