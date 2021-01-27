# Senior-Design

  - Radar
    - Code for Arduino at the bottom of the hill that will detect cars. Basic functions include reading data from radar module, processing data to determine whether a car is
    approaching, and transmitting status to the Arduino at the top of the hill.
    - Setup
      1. Add KickFFT with Arduino library manager
      2. In KickFFT.h, change line 201 to `uint32_t bit = (uint32_t(1) << 30);`
    
  
  - Signal
    - Code for Arduino at the top of the hill. Basic functions include receiving status updates for Arduino at bottom of the hill and using that information to determine whether 
    it should signal that a car is approaching.
  
