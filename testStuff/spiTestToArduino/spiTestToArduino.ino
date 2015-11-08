#include <SPI.h>

#define CS 16
void setup() {
  pinMode(CS,OUTPUT);
  digitalWrite(CS, HIGH);
  SPI.begin();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(CS,LOW);
  SPI.beginTransaction(SPISettings(4000000,MSBFIRST,SPI_MODE0));
  char value = SPI.transfer('H');
  Serial.print(value);
  SPI.endTransaction();
  digitalWrite(CS,HIGH);
  delay(500);
}
