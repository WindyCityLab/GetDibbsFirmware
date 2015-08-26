#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidTWI2.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <SimpleTimer.h>
#include "LightMatrixManager.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#define TIME_ZONE_ADJUSTMENT 5
LiquidTWI2 lcd(0x20);
SimpleTimer refreshDisplayTimer;

Adafruit_AlphaNum4 dayDisplay = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 hourDisplay = Adafruit_AlphaNum4();

char blynkAuthCode[] = "ca8de794d7534659b2b4b5c995333f3a";
LightMatrixManager matrix;

unsigned int localPort = 2390;
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void initializeLCD()
{
  lcd.setMCPType(LTI_TYPE_MCP23008);
  lcd.begin(16, 2);
  lcd.print("GetDibbs V0.1");
}
void initializeMatrix()
{
  matrix.currentWeek = 0;
  matrix.setDay(weekday());
  matrix.currentHour = hour()-TIME_ZONE_ADJUSTMENT; // 7am
  matrix.displayCursor();

  matrix.setClientColor(0x00, RgbColor(0, 0, 50));
  matrix.setClientColor(0x01, RgbColor(0, 50, 0));
  matrix.setClientColor(0x02, RgbColor(50, 0, 0));

  matrix.allocateResource(0, 2, 7 - INITIAL_HOUR, 0x00);
  matrix.allocateResource(0, 4, 9 - INITIAL_HOUR, 0x01);
  matrix.allocateResource(0, 4, 11 - INITIAL_HOUR, 0x02);

  matrix.refreshDisplay();
}

void initializeBlynk()
{
  lcd.clear();
  lcd.print("Blynk Connecting...");
  Blynk.begin(blynkAuthCode, "Catalyze Office 2", "perpetualimprovement");
  lcd.clear();
  lcd.print("Connected!");
}

void setup() {
  Serial.begin(9600);
  dayDisplay.begin(0x70);  // pass in the address
  dayDisplay.clear();
  dayDisplay.writeDisplay();
  
  hourDisplay.begin(0x71);  // pass in the address
  hourDisplay.clear();
  hourDisplay.writeDisplay();
  
  matrix.begin(2);
  matrix.clearDisplay();
  matrix.update();
  
  initializeLCD();
  
  initializeBlynk();
  getTime();
  
  initializeMatrix();
  refreshDisplayTimer.setInterval(2000,updateCursorBasedOnTime);
  displayDay(matrix.getDay());
  displayHour(matrix.currentHour);
}

void updateCursorBasedOnTime()
{
  if ((matrix.getDay() != weekday()) && (matrix.currentHour != hour()-TIME_ZONE_ADJUSTMENT))
  {
    matrix.refreshDisplay();
  }
  lcd.clear();
  lcd.print(matrix.getDay());
  lcd.print(":");
  lcd.print(hour()-TIME_ZONE_ADJUSTMENT);
  lcd.print(":");
  lcd.print(minute());
  lcd.print(":");
  lcd.print(second());
}

BLYNK_WRITE(5) // Increment Day
{
  matrix.incrementDay();
  matrix.refreshDisplay();
  displayDay(matrix.getDay());
  displayHour(matrix.currentHour);
}

BLYNK_WRITE(6) // Increment Hour
{
  matrix.incrementHour();
  matrix.refreshDisplay();
  displayDay(matrix.getDay());
  displayHour(matrix.currentHour);
}
BLYNK_WRITE(7) // Decrement Day
{
  matrix.decrementDay();
  matrix.refreshDisplay();
  displayDay(matrix.getDay());
  displayHour(matrix.currentHour);
}
BLYNK_WRITE(8) // Decrement Hour
{
  matrix.decrementHour();
  matrix.refreshDisplay();
  displayDay(matrix.getDay());
  displayHour(matrix.currentHour);
}

BLYNK_WRITE(9) // Allocate at current location
{
  matrix.toggleAllocation(0x00);
  matrix.refreshDisplay();
}
void loop() {

  Blynk.run();
  refreshDisplayTimer.run();

  if (Serial.available())
  {
    lcd.print(Serial.read());
  }
}




