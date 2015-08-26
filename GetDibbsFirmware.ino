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

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

char blynkAuthCode[] = "ca8de794d7534659b2b4b5c995333f3a";
LightMatrixManager matrix;

unsigned int localPort = 2390;
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void getTime()
{
  udp.begin(localPort);
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  //lcd.print(timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available

  delay(1000);
  
  int cb = udp.parsePacket();
  while (!cb) {
    lcd.print("no packet yet");
    Blynk.run();
    delay(1000);
    cb = udp.parsePacket();
  }
//  lcd.print("packet received, length=");
//  lcd.println(cb);
  // We've received a packet, read the data from it
  udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

  //the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:

  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  //Serial.print("Seconds since Jan 1 1900 = " );

  
  // now convert NTP time into everyday time:
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSince1900 - seventyYears;
  setTime(epoch);
}

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
void display(int day)
{
  switch (day)
  {
    case 1 :
    {
      alpha4.writeDigitAscii(0,'S');
      alpha4.writeDigitAscii(1,'u');
      alpha4.writeDigitAscii(2,'n');
      alpha4.writeDigitAscii(3,' ');
      break;
    }
    case 2 :
    {
      alpha4.writeDigitAscii(0,'M');
      alpha4.writeDigitAscii(1,'o');
      alpha4.writeDigitAscii(2,'n');
      alpha4.writeDigitAscii(3,' ');
      break;
    }
    case 3 :
    {
      alpha4.writeDigitAscii(0,'T');
      alpha4.writeDigitAscii(1,'u');
      alpha4.writeDigitAscii(2,'e');
      alpha4.writeDigitAscii(3,'s');
      break;
    }
    case 4 :
    {
      alpha4.writeDigitAscii(0,'W');
      alpha4.writeDigitAscii(1,'e');
      alpha4.writeDigitAscii(2,'d');
      alpha4.writeDigitAscii(3,'s');
      break;
    }
    case 5 :
    {
      alpha4.writeDigitAscii(0,'T');
      alpha4.writeDigitAscii(1,'h');
      alpha4.writeDigitAscii(2,'u');
      alpha4.writeDigitAscii(3,'r');
      break;
    }
    case 6 :
    {
      alpha4.writeDigitAscii(0,'F');
      alpha4.writeDigitAscii(1,'r');
      alpha4.writeDigitAscii(2,'i');
      alpha4.writeDigitAscii(3,' ');
      break;
    }
    case 7 :
    {
      alpha4.writeDigitAscii(0,'S');
      alpha4.writeDigitAscii(1,'a');
      alpha4.writeDigitAscii(2,'t');
      alpha4.writeDigitAscii(3,' ');
      break;
    }

  }
  alpha4.writeDisplay();
}

void setup() {
  Serial.begin(9600);
  matrix.begin(2);
  alpha4.begin(0x70);  // pass in the address
  alpha4.clear();

  matrix.clearDisplay();
  initializeLCD();
  initializeBlynk();
  getTime();
  initializeMatrix();
  refreshDisplayTimer.setInterval(10000,updateCursorBasedOnTime);
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
  display(matrix.getDay());
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
  display(matrix.getDay());
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
  refreshDisplayTimer.run();

  if (Serial.available())
  {
    lcd.print(Serial.read());
  }
}

unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}




