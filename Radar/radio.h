#ifndef radio_h
#define radio_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

static uint8_t i = 0;
RF24 radio(5, 4); // CE, CSN
uint64_t tx_address = 0xAABBCCDDEE;

void radioInit() {
  radio.begin();
  radio.openWritingPipe(tx_address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.setRetries(0, 0); 
  radio.setChannel(108);
  radio.stopListening();
}

void transmit(uint32_t t) {
  uint32_t data[2];
  data[0] = t;
  data[1] = i++;

  radio.write(&data, sizeof(data));
}
#endif /* radio_h */
