
//#include <KickFFT.h> //Go to line 201 and cast l as 32 bit before shifting to avoid error

#include "adc.h"
#include "timer.h"
#include <arduinoFFT.h>
#include "processing.h"

uint8_t const SAMPLE_MAX = 128; //Be careful that combined size of data_in and fft_result will not be greater than RAM
volatile double data_in[2][SAMPLE_MAX];
volatile uint8_t currently_filling = 0;
uint8_t filled = 2;
volatile uint8_t samples = 0;



void setup() {
  ADCInit();
  timerInit();
  Serial.begin(115200);
  sei();
}

void loop() {
  //one data_in buffer is done being filled
  if(samples >= SAMPLE_MAX)
  {
    ADCSRA &= ~(1 << ADIE); //disable ADC interrupts
    filled = currently_filling; //current filling buffer full and is marked as filled
    currently_filling ^= 0x01; //toggle fill to other buffer   
    samples = 0;
    
    startTimer();//Start countdown to begin filling next buffer

    double freq = dopplerFreq(filled);
    if(dopplerFreq != 0)
    {
      Serial.print(freqToMPH(freq));
      Serial.print(" mph\n");
    }

    transmit(freqToLightTime(freq));
  }
  
}
