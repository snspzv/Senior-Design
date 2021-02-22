#ifndef signalradio_h
#define signalradio_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio Settings
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void RadioSetup() {
  // Radio Setup
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(108);
  radio.startListening();
}

/*
 * Function to poll radio channel for new data arriving
 * Should store data parameter passed by reference if received and return true
 */
bool dataReceived(&timeOn)
{
 
}
#endif /* signalradio_h */
