#ifndef radio_h
#define radio_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

static uint8_t i = 0;
// Radio settings
RF24 radio(5, 4); // CE, CSN
uint64_t tx_address = 0xAABBCCDDEE;

void radioInit() {
  // Radio Setu1p
  radio.begin();
  radio.openWritingPipe(tx_address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.setRetries(0, 0); //1000 us wait, 2 retry max
  radio.setChannel(108);
  radio.stopListening();
}
//takes in time variable, for now is arbitrary, and needs to flash for the amount of time
//sensed from radar.c
void transmit(uint32_t t) {
  uint32_t data[2];
  data[0] = t;
  data[1] = i++;
  //write() returns a bool indicating if the ack was rec'd or not, could be useful for error checking
  // and triggering a pulsing beacon for a backup *** AUTO ACK feature needs to be enabled for this
//  int te = micros();
  bool packet_success = radio.write(&data, sizeof(data));
//  Serial.println(micros() - te);
  //Already automatically retries with radio.SetRetries
  //May add logic for custom acks
}
#endif /* radio_h */
