/* 
 * In dieser Software werden verschiedene Sensorwerte erfunden und genau wie von der 
 * Ultraschall Software über den UART ausgegeben.
 */

#define BOARD_PIN    4       // Digital IO pin connected to the NeoPixels.

// Anzahl der Sensoren ...
int  sensorNr    = 16 ;
int  sensorVal[] = {500,500,500,500,
                    500,500,500,500,
                    500,500,500,500,
                    500,500,500,500};  

// Init the Hardware of the Board
void setup() {

  delay (500);  // warte nach PowerOn, damit die Serielle zum Programmieren frei ist.
  int blinken = 100;

  // init debug serial
  Serial.begin(115200);
  Serial.println("Ultasschall Ausgabe Simulation initialisiert ...");

  randomSeed(analogRead(0));
}


// What to do continously
void loop() {
  int i;
  int fehler;

  // Erzeuge Sensorwerte
  SensorWerte();

  if (random(100) > 2) {
    fehler = 0;
  } else {
    fehler = 1;
  }

  // Ausgabe
  for (i=0;i<sensorNr-fehler;i++) {
    Serial.print( sensorVal[i]);
    Serial.print ("; ");
  }
  Serial.println (" ");

  // delay Ausgabe
  delay(100);
  // Zusatzverzögerung für verschieden langsame Messwerte
  delay(random(200));
  
}



/* *************************************************************************************** *
 * Randomwerte für die Sensorausgabe
 * *************************************************************************************** */

void SensorWerte() {
  int i;
  int sensorHit;

  // alle Werte haben einen Default-Wert - kein Messwert 
  for (i=0;i<sensorNr;i++) {
    sensorVal[i] = 500;
  }

  // Random, ob ein Zufallswert erzeugt werden soll
  if (random(100) > 5) return;   // nur bei den Werten 1 ..5 wird ein anderer Messwert erzeugt

  // Random Zufallswert
  i = random(1,sensorNr-1);
  sensorHit = random(15);
  sensorVal[i-1] = 123 + (10 * sensorHit); 
  sensorVal[i]   = 10 * sensorHit;
  sensorVal[i+1] = 99 + (5 * sensorHit);
 
}
