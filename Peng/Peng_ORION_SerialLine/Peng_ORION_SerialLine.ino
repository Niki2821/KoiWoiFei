#include "MeOrion.h"
MeDCMotor ml(M1);
MeDCMotor mr(M2);

MeLineFollower lineFinderl(PORT_3);
MeLineFollower lineFinderr(PORT_4);

uint8_t motorSpeed = 100;
int rex = 0;
int stopl = 0;
int stopr = 0;
int back = 200;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int sensorStatel = lineFinderl.readSensors();
  switch (sensorStatel)
  {
    case S1_IN_S2_IN: stopl = 1; break;
    case S1_OUT_S2_OUT: stopl = 0; break;
    default: break;
  }
  int sensorStater = lineFinderr.readSensors();
  switch (sensorStater)
  {
    case S1_IN_S2_IN: stopr = 1; break;
    case S1_OUT_S2_OUT: stopr = 0; break;
    default: break;
  }
  if (stopr == 1) {
    ml.run(motorSpeed);
    mr.run(-motorSpeed);
    delay(back);
    ml.stop();
    mr.stop();
  }
  if (stopl == 1) {
    ml.run(-motorSpeed);
    mr.run(motorSpeed);
    delay(back);
    ml.stop();
    mr.stop();
  }
  rex = Serial.parseInt();
  Serial.print(rex);
  if (rex == 1) {
    ml.run(-motorSpeed);
    mr.run(motorSpeed);
    Serial.println("LI");
  }
  if (rex == 2) {
    ml.run(motorSpeed);
    mr.run(-motorSpeed);
    Serial.println("RE");
  }
  if (rex == 3) {
    ml.run(0);
    mr.run(0);
    Serial.println("ST");
  }
  if ( rex == 0) {
    Serial.println("KEINE WERTE");
  }
  delay(50);
}
