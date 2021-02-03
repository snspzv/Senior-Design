#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio Settings
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// LED Alias
const int StatusLED = LED_BUILTIN;
const int WarningLight = 4;


void setup() {
  // Radio Setup
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //LED Setup
  pinMode(StatusLED, OUTPUT);
  pinMode(WarningLight, OUTPUT);

  // Turn LEDs off on start
  digitalWrite(StatusLED, LOW);
  digitalWrite(WarningLight, LOW);

}
void loop() {
  if (radio.available()) {
    int time = 0;
    bool con = 0;
    
    // Read from Radio
    radio.read(&text, sizeof(text));
    
    // Print for debugging
    Serial.println(time);

    // Turn on Status LED to show connection
    digitalWrite(StatusLED, HIGH);
  }
  else{
    // Turn off Status LED
    digitalWrite(StatusLED, LOW);;

    while(!radio.available()){
        // Flash Warning Light
      digitalWrite(WarningLight, HIGH);
      delay(1000);
      digitalWrite(WarningLight, LOW);
      delay(1000);
    }

  }

  if(time > 0){
    // Turn on signal light for length "time"
    digitalWrite(WarningLight, HIGH);
    for (int i = 0; i < time; ++i) { delay(1000); }
    digitalWrite(WarningLight, LOW);
  } 
  else { digitalWrite(WarningLight, LOW); }
  
}
