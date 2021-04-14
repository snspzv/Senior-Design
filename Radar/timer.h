#ifndef timer_h
#define timer_h
//
//
//void timerInit()
//{
//	uint8_t temp = SREG;
//	cli();
//	TCCR1B = 0;
//	TCCR1A = 0;
//	TCNT1 = 0; 
// 
//  //WILL NEED TO BE ADJUSTED ONCE NEW FUNCTIONS ARE ADDED
//  //Estimating FFT processing time is 0.0416 seconds
//	OCR1A = 56576; //FFT processing - (128 slots in array to fill * 13 ADC clocks per conversion * (1 / (16 MHz / 128 prescale))) = 0.028288 seconds
//                 //(16 MHz / 8 prescale) = 2000000 ticks per second
//                 //2000000 * 0.026874 = 56576 ticks 
//	
//	TCCR1B |= (1 << WGM12);	//CTC mode					
//	TIMSK1 |= (1 << OCIE1A); //Interrupts on OCR1A match enabled
//	SREG = temp;
//}
//
//void startTimer()
//{
//	TCCR1B |= (1 << CS11); //8 prescaler
//}
//
//void stopTimer()
//{
//	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 <<CS10)); //prescaler to 0
//}
//
//ISR(TIMER1_COMPA_vect)
//{
//  uint8_t temp = SREG;
//  ADCSRA |= (1 << ADIE); //Re-enable ADC interrupts
//  stopTimer();
//  SREG = temp;
//}
#endif /* timer_h */
