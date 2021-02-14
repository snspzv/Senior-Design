#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio settings
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// Data Pack Struct
struct DataPack{
  int time;
  bool con;
};

DataPack Data;

void setup() {
  // Radio Set Up
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.stopListening();
}
void loop() {
  // Send time value
  Data.time = 5;
  Data.con = 1;
  radio.write(&Data, sizeof(Data));
  
  delay(1000);  
}
