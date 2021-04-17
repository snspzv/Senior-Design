#ifndef adc_h
#define adc_h

#include <ADC.h>
#include <ADC_util.h>
#include <USBHost_t36.h>

extern uint16_t const SAMPLE_MAX;
extern volatile double data_Q[2][512];
extern volatile double data_I[2][512];
extern volatile uint8_t currently_filling;
extern volatile uint8_t filled;
extern volatile uint16_t samples_Q;
extern volatile uint16_t samples_I;
ADC *adc = new ADC();
const int QPin = A9; //Must be adc0
const int IPin = A22; //must be adc1
const uint8_t Q_READY = 1;
const uint8_t I_READY = 2;
extern volatile uint8_t buffer_status;
USBHost myusb;

//Starts both ADCs syncronized in continuous mode with interrupts
//Sampling rate of 17.784 kHz (CPU clock of 144 MHz and ADCK of 24 MHz)
//Buffer size 1024:
  //~58 ms to fill buffers
  //~61 ms to do processing
//~x ms to transmit
//Transmission time + processing < fill time
/*Possible Combinations for sampling rate >= 15kHz (with averaging at 32 and resolution at 12 bit signle ended): 
 *    Conversion speed
 *        LOW_SPEED: 
 *            None
 *        MED_SPEED:
 *            Sampling speed:
 *              VERY_LOW_SPEED: ~4.7 kHz
 *              LOW_SPEED: ~5.9 kHz
 *              MED_SPEED: ~8.6 kHz 
 *              HIGH_SPEED: (~17.47 kHz with 16 samples)
 *              VERY_HIGH_SPEED: ~9.4 kHz
 *        HIGH_SPEED:
 *            Sampling speed:
 *                VERY_LOW_SPEED: ~17.9 kHz sampling rate
 *                LOW_SPEED: ~22.1 kHz sampling rate
 *                MED_SPEED: ~26.8 kHz sampling rate
 *                HIGH_SPEED: ~31.3 kHz sampling rate
 *                VERY_HIGH_SPEED: ~34.2 kHz sampling rate
 *        VERY_HIGH_SPEED:        
 *            Sampling speed:
 *                VERY_LOW_SPEED: ~35.8 kHz sampling rate
 *                LOW_SPEED: ~44.2 kHz sampling rate
 *                MED_SPEED: ~53.6 kHz sampling rate
 *                HIGH_SPEED: ~62.6 kHz sampling rate
 *                VERY_HIGH_SPEED: ~68.2 kHz sampling rate
 */
void ADCInit()
{
  myusb.begin();
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
     data_I[currently_filling][samples_I] = uint16_t(adc->adc1->analogReadContinuous());// * 3.3 / 4095);
     samples_I++;
  }

  //Marke filled buffer as ready for processing
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
    data_Q[currently_filling][samples_Q] = uint16_t(adc->adc0->analogReadContinuous());// * 3.3 /4095);
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

#endif /* adc_h*/
