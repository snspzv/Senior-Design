#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio settings
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
  // Radio Set Up
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  // Send time value
  int time = 5;
  radio.write(&time, sizeof(time));
  delay(1000);
}
