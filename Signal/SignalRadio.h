#ifndef signalradio_h
#define signalradio_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

extern volatile bool g_packetArrived;

RF24 radio(7, 8); // CE, CSN
uint64_t rx_address = 0xAABBCCDDEE;
uint32_t dataIn[2];

void radioInit() {
  //Interrupt on pin 3 (INT1)
  EICRA = 0; //Low level of INT1 generates interrupt
  EIMSK = (1 << INT1); //Enable interrupts on INT1
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
uint32_t getTime()
{
   radio.read(&dataIn, sizeof(dataIn));
   //Serial.println(dataIn[0]);
   return dataIn[0];
}

ISR(INT1_vect)
{
  g_packetArrived = true;
}
#endif /* signalradio_h */
