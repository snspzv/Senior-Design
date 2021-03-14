
//#include <KickFFT.h> //Go to line 201 and cast l as 32 bit before shifting to avoid error

#include "analogConversion.h"
//#include "timer.h"
#include <arduinoFFT.h>
#include "processing.h"
#include "RadarRadio.h"

uint8_t const SAMPLE_MAX = 128; //Be careful that combined size of data_in and fft_result will not be greater than RAM
volatile double data_real[2][SAMPLE_MAX];
volatile double data_imag[2][SAMPLE_MAX];
volatile uint8_t currently_filling = 0;
uint8_t filled = 2;
volatile uint8_t samples_real = 0;
volatile uint8_t samples_imag = 0;



void setup() {
  //__disable_irq();
  ADCInit();
  //timerInit();
  radioInit();
  Serial.begin(9600);
  //__enable_irq();
  Serial.println("test");
}

void loop() {
  //one data_in buffer is done being filled
  if((samples_real >= SAMPLE_MAX) && (samples_imag >= SAMPLE_MAX))
  {
    filled = currently_filling; //current filling buffer full and is marked as filled
    currently_filling ^= 0x01; //toggle fill to other buffer   
    

    double freq = dopplerFreq(filled);
//    if(freq != 0)
//    {
//      Serial.print(freqToLightTime(ms));
//      Serial.print(" s\n");
//    }
    //uint32_t t = freqToLightTime(freq);
    //Serial.println(freqToLightTime(freq));
    //transmit(freqToLightTime(freq));
    samples_real = 0;
    samples_imag = 0;
  }
}
