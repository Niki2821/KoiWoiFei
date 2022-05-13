#include <SPI.h>
#include <Pixy.h>

Pixy pixy;

int signature = 0;
int x = 0;
int Xmin = 120;
int Xmax = 180;
static int i = 0;

void setup()
{
  pixy.init();
  pinMode(1, OUTPUT);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  delay(100);
  while (millis() < 5000) {
    scan();
  }
  scan();
  Serial.println(x);
  if (signature == 1) {

    if (x < Xmin) {
      Serial.println(1);
    }
    if (x > Xmax) {
      Serial.println(2);
    }
    if (x < Xmax && x > Xmin) {
      Serial.println(3);
    }
  }
  
}
void scan()
{
  uint16_t blocks;
  blocks = pixy.getBlocks();  //receive data from pixy
  signature = pixy.blocks[i].signature;    //get object's signature
  x = pixy.blocks[i].x;                    //get x position
}
