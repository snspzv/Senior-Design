#ifndef ADC_h
#define ADC_h


extern uint8_t const SAMPLE_MAX;
extern volatile uint16_t data_in[2][128];
extern volatile uint8_t fill;
extern volatile uint8_t samples;
//extern volatile uint8_t count;

void ADCInit()
{
	DDRC &= ~(1 << DDC0); //set bit 0 of port c (analog pin 0) to input
  ADCSRB = 0; //Free running mode
  ADMUX |= (0 & 0x07)|    // (using analog pin 0) |
           (1 << REFS0);//|  //(use AVcc (5V) for reference voltage) |  
           //(1 << ADLAR);  //(left shift ADC result (8 bit resolution, only read from ADCH))
                                                              
  ADCSRA = 0; 
  //HAS BEEN CHANGED TO 128                                                  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | //(64 prescaler -> 250 kHz ADC clock, 19.23 kHz sampling rate > 14.409 kHz Nyquist frequency for 100 mph cap) |
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
      uint8_t lower = ADCL;
      data_in[fill][samples] = (ADCH << 8) | lower;//High 8 bits of ADC result
      samples++;
  }

	SREG = temp;
}

#endif /* ADC_h*/
