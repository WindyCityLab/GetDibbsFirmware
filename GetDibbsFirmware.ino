#include <NeoPixelBus.h>
#include "LightMatrixManager.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "ca8de794d7534659b2b4b5c995333f3a";
LightMatrixManager matrix;

void setup() {

  Blynk.begin(auth, "deck5", "pearson#dewitt1");
  matrix.begin(2);
  matrix.currentWeek = 0;
  matrix.currentDay = 0; // Sunday
  matrix.currentHour = 7; // 7am 
 // matrix.displayCursor();

  matrix.setClientColor(0x00, RgbColor(0,0,50));
  matrix.setClientColor(0x01, RgbColor(0,50,0));
  matrix.setClientColor(0x02, RgbColor(50,0,0));

  matrix.allocateResource(0, 2, 7 - INITIAL_HOUR, 0x00);
  matrix.allocateResource(0, 4, 9 - INITIAL_HOUR, 0x01);
  matrix.allocateResource(0, 4, 11 - INITIAL_HOUR, 0x02);

  matrix.refreshDisplay();
}

BLYNK_WRITE(5) // Increment Day
{
  matrix.incrementDay();
  matrix.refreshDisplay();
}

BLYNK_WRITE(6) // Increment Hour
{
  matrix.incrementHour();
  matrix.refreshDisplay();
}
BLYNK_WRITE(7) // Decrement Day
{
  matrix.decrementDay();
  matrix.refreshDisplay();
}
BLYNK_WRITE(8) // Decrement Hour
{
  matrix.decrementHour();
  matrix.refreshDisplay();
}

BLYNK_WRITE(9) // Allocate at current location
{
  matrix.toggleAllocation(0x00);
  matrix.refreshDisplay();
}
void loop() {

  Blynk.run();
} 





