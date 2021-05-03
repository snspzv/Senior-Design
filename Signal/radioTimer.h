#ifndef radiotimer_h
#define radiotimer_h

#include "constants.h"

extern volatile uint8_t g_state;
  
double seconds = 0.5; //time to wait for packet arrival before determing the link is down 
static const uint16_t TIME = uint16_t(seconds * TICKS_IN_SECOND);

void radioTimerInit()
{
  TCCR1B = (1<<WGM12); //CTC mode
}

//Makes sure packet is being received at least once every 0.5 seconds
void startRadioTimer()
{
  TCCR1A = 0;
  OCR1AH = (TIME>>8); 
  OCR1AL = (TIME & 0x00FF);
  TIMSK1 |= (1 << OCIE1A); //Interrupt on compare match
  TCCR1B |= ((1<<CS12)| (1<<CS10)); //Prescale of 1024
}

void restartRadioTimer()
{
  TCCR1B &= ~((1<<CS12)|(1<<CS10));
  TCNT1 = 0;
  TCCR1B |= ((1<<CS12)| (1<<CS10)); //Prescale of 1024
}

void stopRadioTimer()
{
  TCCR1B &= ~((1<<CS12)|(1<<CS10));
}

//Repurposes radio timer (timer 1) to blink led on and off at interval of 1 second
//Called if more than 0.5 seconds have passed without receiving packet
void startLightBlinking()
{
  TIMSK1 = 0; //Disable timer 1 interrupts
  TCCR1A = (1 << COM1A0); //Toggle OC1A (LED pin) on compare match
  OCR1AH = uint8_t(TICKS_IN_SECOND >> 8);
  OCR1AL = uint8_t(TICKS_IN_SECOND & 0x00FF);
  TCCR1B |= ((1 << CS12) | (1<< CS10)); //1024 prescale
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
