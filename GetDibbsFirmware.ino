#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <LiquidTWI2.h>
#include <Wire.h>
#include <Time.h>
#include <SimpleTimer.h>
#include "LightMatrixManager.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#define SSID "Catalyze Office 2"

SimpleTimer refreshDisplayTimer;

Adafruit_AlphaNum4 dayDisplay = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 hourDisplay = Adafruit_AlphaNum4();

LightMatrixManager matrix;

const char* ssid     = "Catalyze Office 2";
const char* password = "perpetualimprovement";
const char* host = "getdibbs.parseapp.com";
const int httpPort = 80;

//void initializeLCD()
//{
//  lcd.setMCPType(LTI_TYPE_MCP23008);
//  lcd.begin(16, 2);
//  lcd.print("GetDibbs V0.1");
//  delay(1000);
//}

void connectToTheInternet()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void getTime()
{
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
  }
  String url = "/epochTime";

  Serial.print("Syncing Time...");
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  while(!client.available()) {
    delay(10);
  }
  Serial.println("completed.");
  
  String header = client.readStringUntil('"');
  String epochString = client.readStringUntil('"');
  uint64_t epoch = atof(epochString.c_str())/1000 - (5 * 3600);
  setTime(epoch);
  Serial.print("Hour: "); Serial.println(hour());
  Serial.print("Minute: "); Serial.println(minute());
  Serial.print("Day: "); Serial.println(weekday());
}

void initialize14SegmentDisplay()
{
  Serial.println(F("Clearing 14 Segment Displays"));
  dayDisplay.begin(0x70);  // pass in the address
  dayDisplay.clear();
  dayDisplay.writeDisplay();
  
  hourDisplay.begin(0x71);  // pass in the address
  hourDisplay.clear();
  hourDisplay.writeDisplay();  
}

void getReservations()
{
  matrix.clearAllReservations();
  Serial.print(F("Retrieving reservations..."));
  WiFiClient client;
    if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
  }
  String url = "/reservations?resourceID=001";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" + 
             "Connection: close\r\n\r\n");

  delay(10);
  while(!client.available()) {
    delay(10);
  }
  Serial.println("Completed!");
  
  String skip = client.readStringUntil('[');
  uint8_t count = client.readStringUntil(',').toInt();
  for (int i=0; i<count; i++)
  {
    uint8_t red = client.readStringUntil(',').toInt();
    uint8_t green = client.readStringUntil(',').toInt();
    uint8_t blue = client.readStringUntil(',').toInt();
    RgbColor color = RgbColor(red,green,blue);
    uint8_t day = client.readStringUntil(',').toInt()-1;
    uint8_t hour = client.readStringUntil(',').toInt();

    matrix.allocateResource(0, day, hour, color);
  }
  matrix.refreshDisplay();
}

void setup() {
  matrix.begin(2);
  matrix.clearDisplay();
  matrix.update();

  initialize14SegmentDisplay();

  Serial.begin(115200);
  Serial.println("Hello GetDibbs!");
  delay(100);
  connectToTheInternet();
   
 // initializeLCD();

  getTime();
  matrix.currentHour = hour();
  matrix.setDay(weekday());
  getReservations();
  
  refreshDisplayTimer.setInterval(15000,updateCursorBasedOnTime);
  displayDay(matrix.getDay());
  displayHour(matrix.currentHour);
}

void updateCursorBasedOnTime()
{
  getReservations();
  displayDay(matrix.getDay());
  matrix.currentHour = hour();
  matrix.refreshDisplay();
//  displayCursorOnMatrix();
  displayHour(matrix.currentHour);
}

//BLYNK_WRITE(5) // Increment Day
//{
//  matrix.incrementDay();
//  matrix.refreshDisplay();
//  displayDay(matrix.getDay());
//  displayHour(matrix.currentHour);
//}
//
//BLYNK_WRITE(6) // Increment Hour
//{
//  matrix.incrementHour();
//  matrix.refreshDisplay();
//  displayDay(matrix.getDay());
//  displayHour(matrix.currentHour);
//}
//BLYNK_WRITE(7) // Decrement Day
//{
//  matrix.decrementDay();
//  matrix.refreshDisplay();
//  displayDay(matrix.getDay());
//  displayHour(matrix.currentHour);
//}
//BLYNK_WRITE(8) // Decrement Hour
//{
//  matrix.decrementHour();
//  matrix.refreshDisplay();
//  displayDay(matrix.getDay());
//  displayHour(matrix.currentHour);
//}
//
//BLYNK_WRITE(9) // Allocate at current location
//{
//  matrix.toggleAllocation(0x00);
//  matrix.refreshDisplay();
//}
void loop() {

  refreshDisplayTimer.run();

//  if (Serial.available())
//  {
//    lcd.print(Serial.read());
//  }
}




