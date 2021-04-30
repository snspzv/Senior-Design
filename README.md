# Senior-Design 
## Radar 
Code for Teensy at the bottom of the hill that will detect cars. Basic functions include reading data from radar module, 
processing data to determine whether a car is approaching, and transmitting status to the Arduino at the top of the hill.
#### Hardware Setup
1. Necessary Components:
   - Teensy 3.6
   - nRF24L01 transceiver
   - IPS-937_F radar module
2. Wire following [this schematic](Schematics/radarCircuit.png) 
#### Software Setup
1. Download [Arduino IDE](https://www.arduino.cc/en/software)
2. Add arduinoFFT with Arduino library manager 
   - Delete arduinoFFT/src/arduinoFFT.h and replace it with [this file](libReplacements/arduinoFFT.h)
   - Delete arduinoFFT/src/arduinoFFT.cpp and replace it with [this file](libReplacements/arduinoFFT.cpp)
   - [Documentation](https://github.com/kosme/arduinoFFT)
3. Add RF24 with Arduino library manager
   - [Documentation](https://nrf24.github.io/RF24/classRF24.html)
4. Download [Teensyduino](https://www.pjrc.com/teensy/td_download.html)
5. Before uploading code to the Teensy in the Arduino IDE, take the following steps:
   - Under Tools > Boards > Teensyduino, select Teensy 3.6 
   - Under Tools > CPU Speed, select 144 MHz. **This option will not be available if a Teensy board is not selected!**
#### Notes
1. Change ROAD_DISTANCE in [processing.h](radar/processing.h) to whatever your setup requires. ROAD_DISTANCE should 
   be the distance between the signal circuit and the last place on the road the radar detects cars.
## Signal 
Code for Arduino at the top of the hill. Basic functions include receiving status updates for Arduino 
at bottom of the hill and using that information to determine whether it should signal that a car is approaching.
#### Hardware Setup
1. Necessary Components:
   - Arduino Nano
   - nRF24L01 transceiver
   - Relay
2. Wire follwoing [this schematic](Schematics/radarSignal.png)
#### Software Setup
1. Download [Arduino IDE](https://www.arduino.cc/en/software)
2. Add RF24 with Arduino library manager
   - [Documentation](https://nrf24.github.io/RF24/classRF24.html)
