#ifndef adc_h
#define adc_h


extern uint8_t const SAMPLE_MAX;
extern volatile double data_in[2][128];
extern volatile uint8_t currently_filling;
extern volatile uint8_t samples;

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

#endif /* adc_h*/
