# Senior-Design

  - Radar
    - Code for Arduino at the bottom of the hill that will detect cars. Basic functions include reading data from radar module, processing data to determine whether a car is
    approaching, and transmitting status to the Arduino at the top of the hill.
    - Setup
      - Add arduinoFFT with Arduino library manager 
        1. Delete arduinoFFT/src/arduinoFFT.h and replace with with https://github.com/snspzv/Senior-Design/blob/main/libReplacements/arduinoFFT.h
        2. Delete arduinoFFT/src/arduinoFFT.cpp and replace with https://github.com/snspzv/Senior-Design/blob/main/libReplacements/arduinoFFT.cpp
      - Add RF24 with Arduino library manager
    
  
  - Signal
    - Code for Arduino at the top of the hill. Basic functions include receiving status updates for Arduino at bottom of the hill and using that information to determine whether 
    it should signal that a car is approaching.
    - Setup
      1. Add RF24 with Arduino library manager
      
  
  - Git
    - To commit changes locally
      - `git add "file1" "file2"`
      - `git commit -m "description of changes"`
    - To push to remote repo
      - `git push`
      
  - RF24 Library Documentation: https://maniacbug.github.io/RF24/classRF24.html
  
  
