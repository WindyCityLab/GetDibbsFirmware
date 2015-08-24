#include <NeoPixelBus.h>
#include "LightMatrixManager.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidTWI2.h>
#include <Wire.h>
#include <WiFiUdp.h>

LiquidTWI2 lcd(0x20);

char blynkAuthCode[] = "ca8de794d7534659b2b4b5c995333f3a";
LightMatrixManager matrix;

//unsigned int localPort = 2390;
//IPAddress timeServerIP; // time.nist.gov NTP server address
//const char* ntpServerName = "time.nist.gov";
//const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
//byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
//// A UDP instance to let us send and receive packets over UDP
//WiFiUDP udp;

//void getTime()
//{
//  WiFi.begin("deck5", "pearson#dewitt1");
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//  }
//  udp.begin(localPort);
//  //get a random server from the pool
//  WiFi.hostByName(ntpServerName, timeServerIP);
//
//  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
//  // wait to see if a reply is available
//  delay(1000);
//
//  int cb = udp.parsePacket();
//  if (!cb) {
//    Serial.println("no packet yet");
//  }
//  else {
//    Serial.print("packet received, length=");
//    Serial.println(cb);
//    // We've received a packet, read the data from it
//    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
//
//    //the timestamp starts at byte 40 of the received packet and is four bytes,
//    // or two words, long. First, esxtract the two words:
//
//    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
//    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
//    // combine the four bytes (two words) into a long integer
//    // this is NTP time (seconds since Jan 1 1900):
//    unsigned long secsSince1900 = highWord << 16 | lowWord;
//    Serial.print("Seconds since Jan 1 1900 = " );
//    Serial.println(secsSince1900);
//
//    // now convert NTP time into everyday time:
//    Serial.print("Unix time = ");
//    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
//    const unsigned long seventyYears = 2208988800UL;
//    // subtract seventy years:
//    unsigned long epoch = secsSince1900 - seventyYears;
//    // print Unix time:
//    Serial.println(epoch);
//
//
//    // print the hour, minute and second:
//    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
//    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
//    Serial.print(':');
//    if ( ((epoch % 3600) / 60) < 10 ) {
//      // In the first 10 minutes of each hour, we'll want a leading '0'
//      Serial.print('0');
//    }
//    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
//    Serial.print(':');
//    if ( (epoch % 60) < 10 ) {
//      // In the first 10 seconds of each minute, we'll want a leading '0'
//      Serial.print('0');
//    }
//    Serial.println(epoch % 60); // print the second
//  }
//}

void initializeLCD()
{
  lcd.setMCPType(LTI_TYPE_MCP23008);
  lcd.begin(16, 2);
  lcd.print("GetDibbs V0.1");
//  delay(500);
}
void initializeMatrix()
{
  matrix.currentWeek = 0;
  matrix.currentDay = 0; // Sunday
  matrix.currentHour = 7; // 7am
  // matrix.displayCursor();

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
  Blynk.begin(blynkAuthCode, "Catalyze Office", "perpetualimprovement");
  lcd.clear();
  lcd.print("Connected!");
}
void setup() {
  
  matrix.begin(2);
  matrix.clearDisplay();
  initializeLCD();
  initializeBlynk();
  //  getTime();
  initializeMatrix();

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
// send an NTP request to the time server at the given address
//unsigned long sendNTPpacket(IPAddress& address)
//{
//  Serial.println("sending NTP packet...");
//  // set all bytes in the buffer to 0
//  memset(packetBuffer, 0, NTP_PACKET_SIZE);
//  // Initialize values needed to form NTP request
//  // (see URL above for details on the packets)
//  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
//  packetBuffer[1] = 0;     // Stratum, or type of clock
//  packetBuffer[2] = 6;     // Polling Interval
//  packetBuffer[3] = 0xEC;  // Peer Clock Precision
//  // 8 bytes of zero for Root Delay & Root Dispersion
//  packetBuffer[12]  = 49;
//  packetBuffer[13]  = 0x4E;
//  packetBuffer[14]  = 49;
//  packetBuffer[15]  = 52;
//
//  // all NTP fields have been given values, now
//  // you can send a packet requesting a timestamp:
////  udp.beginPacket(address, 123); //NTP requests are to port 123
////  udp.write(packetBuffer, NTP_PACKET_SIZE);
////  udp.endPacket();
//}




