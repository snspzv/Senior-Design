# Senior-Design

  - Radar
    - Code for Arduino at the bottom of the hill that will detect cars. Basic functions include reading data from radar module, processing data to determine whether a car is
    approaching, and transmitting status to the Arduino at the top of the hill.
    - Setup
      1. Add arduinoFFT with Arduino library manager 
         -Delete arduinoFFT/src/arduinoFFT.h and replace it with [this file](libReplacements/arduinoFFT.h)
         -Delete arduinoFFT/src/arduinoFFT.cpp and replace it with [this file](libReplacements/arduinoFFT.cpp)
         -[Documentation](https://github.com/kosme/arduinoFFT)
      2. Add RF24 with Arduino library manager
         -[Documentation](https://nrf24.github.io/RF24/classRF24.html)
      3. Download [Teensyduino](https://www.pjrc.com/teensy/td_download.html)
         -Set clock speed to 144 MHz 
  
  - Signal
    - Code for Arduino at the top of the hill. Basic functions include receiving status updates for Arduino at bottom of the hill and using that information to determine whether 
    it should signal that a car is approaching.
    - Setup
      1. Add RF24 with Arduino library manager
         -[Documentation](https://nrf24.github.io/RF24/classRF24.html)
  
  - [Signal State Diagram](https://drive.google.com/file/d/1SXXWFQub_RAO9IWIOhbIvrC0gkTXHe1I/view?usp=sharing)
