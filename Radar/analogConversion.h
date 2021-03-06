#ifndef analogconversion_h
#define analogconversion_h

#include <ADC.h>
#include <ADC_util.h>

uint16_t const SAMPLE_MAX = 512;
volatile double data_Q[2][SAMPLE_MAX] = {};
volatile double data_I[2][SAMPLE_MAX] = {};
volatile uint8_t currently_filling = 0;
volatile uint8_t filled = 2;
volatile uint16_t samples_Q = 0;
volatile uint16_t samples_I = 0;
ADC *adc = new ADC();
const int QPin = A9; //Must be adc0
const int IPin = A22; //must be adc1
const uint8_t Q_READY = 1;
const uint8_t I_READY = 2;
extern volatile uint8_t buffer_status;

//Starts both ADCs syncronized in continuous mode with interrupts
//Sampling rate of 17.047 kHz (CPU clock of 144 MHz and ADCK of 24 MHz)
//Sampling rate allows detection of speeds over 115 mph
//Buffer size 512:
  //~31 ms to fill buffers
  //~27 ms to do process and transmit
void ADCInit()
{
  pinMode(QPin, INPUT);
  pinMode(IPin, INPUT);
  
  //Set # of averages per conversion
  adc->adc1->setAveraging(16);
  adc->adc0->setAveraging(16);

  //Set # of bits in conversion
  //8, 10, 12, or 16 for single ended
  adc->adc1->setResolution(12); 
  adc->adc0->setResolution(12); 

  //Set ADCK, with Bus clock @ 48 MHz:
  //LOW_SPEED: 3 MHz
  //MED_SPEED: 6 MHz
  //HIGH_SPEED: 24 MHz
  //VERY_HIGH_SPEED: 48 MHz
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED); 
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED); 

  //Set extra ADC cycles to add each conversion
  //VERY_LOW_SPEED: 20 extra
  //LOW_SPEED: 12 extra
  //MED_SPEED: 6 extra
  //HIGH_SPEED: 2 extra
  //VERY_HIGH_SPEED: 0 extra
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED); 

  //Interrupts enabled when conversions complete
  adc->adc1->enableInterrupts(adc1_isr);
  adc->adc0->enableInterrupts(adc0_isr);

  adc->startSynchronizedContinuous(QPin, IPin);                                               
}

void adc1_isr(void)
{
  if (samples_I < SAMPLE_MAX)
  {
     data_I[currently_filling][samples_I] = uint16_t(adc->adc1->analogReadContinuous()) * (double(3.3) / double(4095));
     samples_I++;
  }

  else
  {
    //If other buffer done switch filled and currently filling
    if((buffer_status & I_READY) == I_READY)
    {
      filled = currently_filling;
      currently_filling ^= 1;
      samples_I = 0;
      samples_Q = 0;
    }

    buffer_status |= Q_READY;
  }
}

void adc0_isr(void)
{ 
  if (samples_Q < SAMPLE_MAX)
  {
    data_Q[currently_filling][samples_Q] = uint16_t(adc->adc0->analogReadContinuous()) *(double(3.3) / double(4095));
    samples_Q++;
  }

  else
  {
    //If other buffer done switch filled and currently filling
    if((buffer_status & Q_READY)== Q_READY)
    {
      filled = currently_filling;
      currently_filling ^= 1;
      samples_Q = 0;
      samples_I = 0;
    }

    buffer_status |= I_READY;
  }  
}

#endif /* analogconversion_h*/
