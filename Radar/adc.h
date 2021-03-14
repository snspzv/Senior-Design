#ifndef adc_h
#define adc_h

// for the Teensy, 
// from pedvide's github
// do we need?
#include <VREF.h>
#include <ADC.h>
#include <ADC_util.h>

//teensy clock rate is 180 MHz
extern uint8_t const SAMPLE_MAX;
extern volatile double data_in[2][128];
extern volatile uint8_t currently_filling;
extern volatile uint8_t samples;
/*
void ADCInit()
{
	DDRC &= ~(1 << DDC0); //set bit 0 of port c (analog pin 0) to input
  ADCSRB = 0; //Free running mode
  ADMUX |= (0 & 0x07)|    // (using analog pin 0) |
           (1 << REFS0);//|  //(use AVcc (5V) for reference voltage) |  
           //(1 << ADLAR);  //(left shift ADC result (8 bit resolution, only read from ADCH))
                                                              
  ADCSRA = 0; 
  //PRECALE CURRENTLY 128 -> 125 kHz ADC clock,  9.615 kHz sampling rate, ~65 mph cap
  //Having ADC clock < 150 kHz allows for 10 bit precision, likely will need to go to 8 bit precision for real application to exceed 65 mph cap, OK for lab testing                                                  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | //(64 prescaler -> 250 kHz ADC clock, 19.23 kHz sampling rate > 14.752 kHz Nyquist frequency for 100 mph cap) |
            (1 << ADATE) |  //(enable auto-trigger (free-running mode selected in B)) |
            (1 << ADIE) |   //(enable interrupts when ADC done) |
            (1 << ADEN) |   //(enable ADC) |
            (1 << ADSC);    //(start ADC)                                                               
}

ISR(ADC_vect)
{
	uint8_t const temp = SREG;
  if(samples < 128)
  {
      uint8_t lower = ADCL; //unecessary for 8 bit precision
      data_in[currently_filling][samples] = (ADCH << 8) | lower;//10 bit precision
      //data_in[currently_filling][samples] = ADCH; //8 bit precision, must set ADLAR before using
      samples++;
  }

	SREG = temp;
}
*/
/*
//interrupts possibly found in adc_pdb 
//teensy isr here()
{
  
}
*/

void ADCInit()
{

Serial.begin(9600);


// we might be able to do it like this, I believe that the pins 
// are enumerated, and the isr's are setup like pedvide has setup
// so that should be plug n play

//option 1 declare everything here not using pedvide's stuff 

//long sample time
//ADLSMP = 1;
atomic::setBitFlag(adc_regs.CFG1, ADC_CFG1_ADLSMP);

// asynchronous clock output disabled, only enabled if selected by adiclk
//ADACKEN = 0;
atomic::clearBitFlag(adc_regs.CFG2, ADC_CFG2_ADACKEN);

// bus clock divded by 2
//ADICLK = 1;
atomic::setBitFlag(adc_regs.CFG1, ADC_CFG1_ADICLK(1));

// divide bus clock again by 2
//ADIV = 01;
atomic::setBitFlag(adc_regs.CFG1, ADC_CFG1_ADIV(1));

//hardware average function enabled
//AVGE = 1;
atomic::setBitFlag(adc_regs.SC3, ADC_SC3_AVGE);

//ADCO = 1; continuous conversions 
//
atomic::setBitFlag(adc_regs.SC3, ADC_SC3_ADCO);


// 32 samples averaged
//AVGS = 3;
atomic::setBitFlag(adc_regs.SC3, ADC_SC3_AVGS(3));

//long conversion time 
//ADLSMP = 1;
atomic::setBitFlag(adc_regs.CFG, ADC_CFG_ADLSMP);


// default longest sample time, +20adck cycles, 24 adck cycles total
//ADLSTS = 0;
atomic::setBitFlag(adc_regs.CFG2, ADC_CFG2_ADLSTS(0));

//highspeed conversion sequence selected w/ 2 additional adck cycles to 
//total conversion time
//ADHSC = 1;
atomic::setBitFlag(adc_regs.CFG2, ADC_CFG2_ADHSC);

//DIFF = 0 , differential conversions and input channels are selected
atomic::clearBitFlag(adc_regs.SC1A, ADC_SC1A_DIFF);


//MODE = 01 , single ended 12 bit conversion
atomic::setBitFlag(adc_regs.CFG1, ADC_CFG1_MODE(1));


//enable interrupts
//AIEN = 1;
atomic::setBitFlag(adc_regs.SC1A, ADC_SC1_AIEN);

attachInterruptVector(IRQ_ADC, isr);
NVIC_SET_PRIORITY(IRQ_ADC, priority);
NVIC_ENABLE_IRQ(IRQ_ADC);

///// ADC0 ////

//pedvide's setting for sampling speed does not use the combination that
// we are looking for i.e. adlsts = 00
// these also set the bits, too, but will need to 
//still declare everything other than adlsmp, adsts, adhsc and adlpc

/*
adc->adc0->setAveraging(1); // set number of averages
adc->adc0->setResolution(8); // set bits of resolution
//high_speed is adding 6cc's to the clock rate
adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED); // change the conversion speed
adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MEDIUM_SPEED; // change the sampling speed

////// ADC1 /////
adc->adc1->setAveraging(1); // set number of averages
adc->adc1->setResolution(8); // set bits of resolution
adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED); // change the conversion speed
adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_MEDIUM_SPEED); // change the sampling speed

// we have started to get values from the ADC, still need to read the values
// with readSynchronizedContinuous
adc->startSynchronizedContinuous(readPin, readPin2);

*/
delay(100);
Serial.println("end setup");
}


#endif /* adc_h*/
