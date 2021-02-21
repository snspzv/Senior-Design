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
//takes in time variable, for now is arbitrary, and needs to flash for the amount of time
//sensed from radar.c
void transmit(const uint8_t time) {

  uint8_t packet_success;
  //write() returns a bool indicating if the ack was rec'd or not, could be useful for error checking 
  // and triggering a pulsing beacon for a backup *** AUTO ACK feature needs to be enabled for this
  packet_success = radio.write(&time, sizeof(time));
  /*if (packet_success)
    {
      send a retry/backup here?    
    }
*/
#endif /* radarradio_h */
