#include <SPI.h>

void setup() {
  pinMode(SS,OUTPUT);
  digitalWrite(SS, HIGH);
  SPI.begin();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(SS,LOW);
  SPI.beginTransaction(SPISettings(4000000,MSBFIRST,SPI_MODE0));
  char value = SPI.transfer('H');
  Serial.print(value);
  SPI.endTransaction();
  digitalWrite(SS,HIGH);
  delay(500);
}
