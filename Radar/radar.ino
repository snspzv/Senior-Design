
//#include <KickFFT.h> //Go to line 201 and cast l as 32 bit before shifting to avoid error

#include "adc.h"
#include "timer.h"
#include <arduinoFFT.h>
#include "processing.h"

//uint8_t const SAMPLE_MAX = 128; //Be careful that combined size of data_in and fft_result will not be greater than RAM

//to be ran with Teensy board
uint16_t const SAMPLE_MAX = 256;
volatile double data_in_I[2][SAMPLE_MAX];
volatile double data_in_Q[2][SAMPLE_MAX];
volatile uint8_t currently_filling = 0;
uint8_t filled = 2;
volatile uint8_t samples = 0;

//extern adc ... here?

void setup() {
 
  ADCInit();
  timerInit();
  radioInit();
  //9600 declared in adc.h
  //Serial.begin(115200);
  //sei();
}

//put the filling of the arrays here with reading from the radar, or in the 
// ISR in adc.h?

//dont change the idea, just switch over to teensy

const int readpinI = A9; //adc0
const int readpinQ = A6; // adc1

// if we want to define in ADCinit()?
//extern adc;

ADC *adc = new ADC();

pinMode(LED_BUILTIN, OUTPUT);
pinMode(readpinI, INPUT);
pinMode(readpinQ, INPUT);

// struct to keep track of readouts from pin1 and pin2
Sync_result readouts;

void loop() {
/*
  //one data_in buffer is done being filled
  if(samples >= SAMPLE_MAX)
  {

    ADCSRA &= ~(1 << ADIE); //disable ADC interrupts
    filled = currently_filling; //current filling buffer full and is marked as filled
    currently_filling ^= 0x01; //toggle fill to other buffer   
    samples = 0;
    
    startTimer();//Start countdown to begin filling next buffer

    double freq = dopplerFreq(filled);
//    if(freq != 0)
//    {
//      Serial.print(freqToLightTime(ms));
//      Serial.print(" s\n");
//    }

    transmit(freqToLightTime(freq));
  }
  */
  

  //reads in the values of pin1 and pin2 to adc0 and adc1
  readouts = adc->readSynchronizedContinuous();

  //**do we need to do this?**
  // if using 16 bits and single-ended is necessary to typecast to unsigned,
  // otherwise values larger than 3.3/2 will be interpreted as negative
  readouts.result_adc0 = (uint16_t)readouts.result_adc0;
  readouts.result_adc1 = (uint16_t)readouts.result_adc1;

  //fill the arrays here?


  //main question, can we use an ISR for filling the arrays to feed into FFT?
  Serial.print(time, DEC);
  Serial.print(" ");
  Serial.print(result.result_adc0*3.3/adc->adc0->getMaxValue(), DEC);
  Serial.print(" ");
  Serial.println(result.result_adc1*3.3/adc->adc1->getMaxValue(), DEC);


  // Print errors, if any.
  if(adc->adc0->fail_flag != ADC_ERROR::CLEAR) 
  {
    Serial.print("ADC0: "); Serial.println(getStringADCError(adc->adc0->fail_flag));
  }
      
  if(adc->adc1->fail_flag != ADC_ERROR::CLEAR) 
  {
    Serial.print("ADC1: "); Serial.println(getStringADCError(adc->adc1->fail_flag));
  }


  digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
}
