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
  EICRA &= ~((1 << ISC11) | (1 << ISC10)); //Low level of INT1 generates interrupt
  EIMSK |= (1 << INT1); //Enable interrupts on INT1
  radio.begin();
  radio.openReadingPipe(1, rx_address);
  radio.setPALevel(RF24_PA_HIGH); //High power
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.setChannel(108);
  radio.startListening();
}

//Reads received data from transceiver
//Returns the time for the light to turn on in seconds
double getTime()
{
   radio.read(&dataIn, sizeof(dataIn));
   return dataIn[0];
}

//Interrupt generated everytime the transceiver receives data
ISR(INT1_vect)
{
  g_packetArrived = true;
}
#endif /* signalradio_h */
