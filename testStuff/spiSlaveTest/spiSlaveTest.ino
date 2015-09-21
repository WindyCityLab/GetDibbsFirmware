volatile boolean process_it;
volatile byte incoming;
volatile boolean led6On;

void SPI_SlaveInit()
{
  SPCR |= (1 << SPE); // Turn on SPI Circuitry
  SPCR &= ~(1 << MSTR); // Clear this bits sets SPI to Slave mode
  SPCR |= (1 << SPIE); // Turn on the SPI Interrupt handling
  SPCR &= ~((1 << CPOL) | (1 << CPHA)); // Sample on Rising Edge and setup on Falling
  
}
void setup() {

  SPI_SlaveInit();
  
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);

  process_it = false;

//  SPI.attachInterrupt();
  led6On = false;
  digitalWrite(6,led6On);
  delay(100);
  digitalWrite(6,HIGH);
  delay(100);
  digitalWrite(6,LOW);
}

ISR(SPI_STC_vect)
{
  process_it = true;
}
void loop() {
  if (process_it)
  {
    SPDR = 'L';
    incoming = SPDR;
    if (incoming == 'H')
    {
      led6On = !led6On;
      digitalWrite(6, led6On);
      process_it = false;
    }
  }

}
