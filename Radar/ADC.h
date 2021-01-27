#ifndef ADC_h
#define ADC_h


//extern volatile bool arr_filled;
extern uint8_t const SAMPLE_MAX;
extern volatile uint16_t data_in[2][128];
extern volatile uint8_t fill;
extern volatile uint8_t send_fft;

void ADCInit()
{
	DDRC &= ~(1 << DDC0); //set bit 0 of port c (analog pin 0) to input
	
	//Add SREG copy and restore if loop() needs arithmetic operations
  ADCSRB = 0; //Free running mode
  ADMUX |= (0 & 0x07)|    // (using analog pin 0) |
           (1 << REFS0)|  //(use AVcc (5V) for reference voltage) |  
           (1 << ADLAR);  //(left shift ADC result (8 bit resolution, only read from ADCH))
                                                              
  ADCSRA = 0;                                                   
  ADCSRA |= (1 << ADPS2) |  //(16 prescaler -> 1 MHz ADC clock, 76.9 kHz sampling rate) |
            (1 << ADATE) |  //(enable auto-trigger (free-running mode selected in B)) |
            (1 << ADIE) |   //(enable interrupts when ADC done) |
            (1 << ADEN) |   //(enable ADC) |
            (1 << ADSC);    //(start ADC)
                                                                                                                             
 //sei(); //enable global interrupts                                                                  
}

/*ISR(ADC_vect)
{
  static uint8_t samples = 0;
  data_in[samples] = ADCH;//High 8 bits of ADC result
  samples++;

  if(samples == SAMPLE_MAX)
  {
    samples = 0;
    arr_filled = true; //flag set in loop()to calculate fft   
  }
}*/

ISR(ADC_vect)
{
	uint8_t const temp = SREG;
	static uint8_t samples = 0;
	data_in[fill][samples] = ADCH;//High 8 bits of ADC result
	samples++;
	if(samples == SAMPLE_MAX)
	{
    send_fft = fill;
		fill ^= 0x01; //toggle fill to other buffer
		
		if(fill == send_fft)
			ADCSRA &= ~(1 << ADIE); //disable ADC interrupts
		
		
		
		samples = 0;

    ADCSRA &= ~(1 << ADIE); //disable ADC interrupts
	}
	
	SREG = temp;
}

#endif /* ADC_h*/
