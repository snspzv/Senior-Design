#ifndef radiotimer_h
#define radiotimer_h

#include "constants.h"

extern volatile uint8_t g_state;
  
//time is set in 500 milliseconds ASSUMPTION IS THAT WE WILL NOT GO ABOVE 999 ms 
double seconds = 0.5; 
static const uint16_t TIME = uint16_t(seconds * TICKS_IN_SECOND);

 

 //sets timer 1 to be in ctc, with cycles needed equating to 500 ms in ocr
void radioTimerInit()
{
  TCCR1B = (1<<WGM12); //CTC mode
}

 //just use a const and no param, just set prescale ->101
void startRadioTimer()
{
  TCCR1A = 0;
//  TCCR1B = (1<<WGM12);
  //upper bits pushed in high
  OCR1AH = (TIME>>8);
  //get lower 8 bits into Low
  OCR1AL = (TIME & 0x00FF);
  TIMSK1 |= (1 << OCIE1A);
  TCCR1B |= ((1<<CS12)| (1<<CS10)); // set prescale to 1024
}

void restartRadioTimer()
{
  TCCR1B &= ~((1<<CS12)|(1<<CS10));
  TCNT1 = 0;
  TCCR1B |= ((1<<CS12)| (1<<CS10)); // set prescale to 1024
}

void stopRadioTimer()
{
  TCCR1B &= ~((1<<CS12)|(1<<CS10));
}

void startLightBlinking()
{
  TIMSK1 = 0; //Disable timer 1 interrupts
  TCCR1A = (1 << COM1A0); //Toggle OC1A (LED pin) on compare match
  OCR1AH = uint8_t(TICKS_IN_SECOND >> 8);
  OCR1AL = uint8_t(TICKS_IN_SECOND & 0x00FF);
  TCCR1B |= ((1 << CS12) | (1<< CS10)); //1024 prescale, actually starts timer
}

ISR(TIMER1_COMPA_vect)
{
  uint8_t temp = SREG;
  g_state = LIGHT_BLINKING;
  stopRadioTimer();
  startLightBlinking();
  SREG = temp;
}

#endif /* radiotimer_h */
