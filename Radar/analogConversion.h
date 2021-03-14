#ifndef adc_h
#define adc_h

#include <ADC.h>
#include <ADC_util.h>

extern uint8_t const SAMPLE_MAX;
extern volatile double data_real[2][128];
extern volatile double data_imag[2][128];
extern volatile uint8_t currently_filling;
extern volatile uint8_t samples_real;
extern volatile uint8_t samples_imag;
static ADC *adc = new ADC();
const int readPin = A9;
const int readPin2 = A6;

void ADCInit()
{
  pinMode(readPin, INPUT);
  pinMode(readPin2, INPUT);
  
  //Set # of averages per conversion
  adc->adc1->setAveraging(32);
  adc->adc0->setAveraging(32);

  //Set # of bits in conversion
  //8, 10, 12, or 16 for single ended
  adc->adc1->setResolution(12); 
  adc->adc0->setResolution(12); 

  //Set ADCK, with CPU @ 24 MHz and Bus clock @ 24 MHz with bus clock or bus clock /2 as input to ADCK:
  //LOW_SPEED: 3 MHz
  //MED_SPEED: 12 MHz
  //HIGH_SPEED: 24 MHz
  //VERY_HIGH_SPEED: 24 MHz
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED); // change the conversion speed
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED); // change the conversion speed

  //Set extra ADC cycles to add each conversion
  //VERY_LOW_SPEED: 20 extra
  //LOW_SPEED: 12 extra
  //MED_SPEED: 6 extra
  //HIGH_SPEED: 2 extra
  //VERY_HIGH_SPEED: 0 extra
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_LOW_SPEED); // change the sampling speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_LOW_SPEED); // change the sampling speed

  //Interrupts enabled when conversions complete
  adc->adc1->enableInterrupts(adc1_isr);
  adc->adc0->enableInterrupts(adc0_isr);

  //Puts both adcs in continuous mode
  //Come back later to modify for closer start time on each
  adc->startSynchronizedContinuous(readPin, readPin2);                                               
}

void adc1_isr(void)
{
  if (samples_real < 128)
  {
     data_real[currently_filling][samples_real] = adc->adc1->analogReadContinuous();
     samples_real++;
  }
}

void adc0_isr(void)
{
  if (samples_imag < 128)
  {
     data_imag[currently_filling][samples_imag] = adc->adc0->analogReadContinuous();
     samples_imag++;
  }
}

#endif /* adc_h*/
