#ifndef radarradio_h
#define radarradio_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio settings
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void RadioSetup() {
  // Radio Setu1p
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(108);
  radio.stopListening();
}

#endif /* radarradio_h */
