#ifndef timer_h
#define timer_h

extern volatile uint8_t send_fft;


void timerInit()
{
	uint8_t temp = SREG;
	cli();
	TCCR1B = 0;
	TCCR1A = 0;
	TCNT1 = 0; 
 
  //WILL NEED TO BE ADJUSTED ONCE NEW FUNCTIONS ARE ADDED
	OCR1A = 11657; //fft funcntion time - (128 slots in array to fill * 13 ADC clocks per conversion * (16 MHz / 16 prescale)) = 0.04663 seconds
                 //(16 MHz / 64 prescale) = 250000 ticks per second
                 //250000 * 0.04663 = 11657.5 ticks 
	
	TCCR1B |= (1 << WGM12);	//CTC mode					
	TIMSK1 |= (1 << OCIE1A); //Interrupts on OCR1A match enabled
	SREG = temp;
}

void startTimer()
{
	TCCR1B |= (1 << CS11) | (1 <<CS10); //64 prescaler
}

void stopTimer()
{
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 <<CS10)); //prescaler to 0
}

ISR(TIMER1_COMPA_vect)
{
  uint8_t temp = SREG;
  ADCSRA |= (1 << ADIE); //Re-enable ADC interrupts
  stopTimer();
  send_fft = 2; //reset access to fft function for next loop
  SREG = temp;
}
#endif /* timer_h */