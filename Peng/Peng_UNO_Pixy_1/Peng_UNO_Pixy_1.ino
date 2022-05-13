#include <SPI.h>
#include <Pixy.h>

Pixy pixy;

int signature = 0;
int x = 0;
int Xmin = 100;
int Xmax = 200;
static int i = 0;

void setup()
{
  Serial.begin(9600);
  pixy.init();
}

void loop() {
  while (millis() < 5000) {
    scan();
  }
  scan();
  if (signature == 1) {

    if (x < Xmin) {
      Serial.println("L");
    }
    if (x > Xmax) {
      Serial.println("R");
    }
    if (x < Xmax && x > Xmin) {
      Serial.println("S");
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
