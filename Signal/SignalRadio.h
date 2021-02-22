#ifndef signalradio_h
#define signalradio_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio Settings
RF24 radio(7, 8); // CE, CSN
uint64_t rx_address = 0xAABBCCDDEE;
uint32_t dataIn[2];

void radioInit() {
  // Radio Setup
  radio.begin();
  radio.openReadingPipe(1, rx_address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.setChannel(108);
  radio.startListening();
}

/*
 * Function to poll radio channel for new data arriving
 * Should store data parameter passed by reference if received and return true
 */
bool dataReceived(uint32_t &timeOn)
{
  if(radio.available())
  {
     radio.read(&dataIn, sizeof(dataIn));
     //Serial.println(dataIn[0]);
     timeOn = dataIn[0];
     return true;
  }
  
  return false;
}
#endif /* signalradio_h */
