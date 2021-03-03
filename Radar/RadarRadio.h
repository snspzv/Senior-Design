#ifndef radarradio_h
#define radarradio_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio settings
RF24 radio(7, 8); // CE, CSN
uint64_t tx_address = 0xAABBCCDDEE;

void radioInit() {
  // Radio Setu1p
  radio.begin();
  radio.openWritingPipe(tx_address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.setRetries(5, 2); //1000 us wait, 2 retry max
  radio.setChannel(108);
  radio.stopListening();
}
//takes in time variable, for now is arbitrary, and needs to flash for the amount of time
//sensed from radar.c
void transmit(uint32_t t) {
  double data[2];
  data[0] = t;
  data[1] = 0;
  //write() returns a bool indicating if the ack was rec'd or not, could be useful for error checking 
  // and triggering a pulsing beacon for a backup *** AUTO ACK feature needs to be enabled for this
  bool packet_success = radio.write(&data, sizeof(data));
  
  //Already automatically retries with radio.SetRetries
  //May add logic for custom acks
}
#endif /* radarradio_h */
