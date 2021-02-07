#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio Settings
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// LED Alias
const int WarningLight = 4;


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
  int time = 0;
  bool con = 0;
  
  if (radio.available()) {    
    // Read from Radio
    radio.read(&time, sizeof(time));
    radio.read(&con, sizeof(con));
    
    // Print for debugging
    Serial.println(time);
    Serial.println(con);

    // Turn on signal light for length "time"
    if(time > 0){
    digitalWrite(WarningLight, HIGH);
    for (int i = 0; i < time; ++i) { delay(1000); }
    digitalWrite(WarningLight, LOW);
  } 
  else { digitalWrite(WarningLight, LOW); }
  }
  /* else{
    while(!con){
        // Flash Warning Light
      digitalWrite(WarningLight, HIGH);
      delay(1000);
      digitalWrite(WarningLight, LOW);
      delay(1000);
    }
  }
*/  
}
