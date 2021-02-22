#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio Settings
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// LED Alias
const int WarningLight = 4;

// Data Pack Struct
struct DataPack{
  int time;
  bool con;
};

DataPack Data;

void setup() {
  // Radio Setup
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.startListening();

  //LED Setup
  pinMode(WarningLight, OUTPUT);

  // Turn LEDs off on start
  digitalWrite(WarningLight, LOW);
}

void loop() {
  // Set intial values for time and con
  Data.time = 0;
  Data.con = 0;
  
  if (radio.available()) {    
    // Read from Radio
    radio.read(&Data, sizeof(Data));
    
    // Print for debugging
    Serial.println(Data.time);
    Serial.println(Data.con);
  }
  
  // Turn on signal light for length "time"
  if(Data.time > 0){
      digitalWrite(WarningLight, HIGH);
      for (int i = 0; i < Data.time; ++i) { delay(1000); }
      digitalWrite(WarningLight, LOW);
  }
}
