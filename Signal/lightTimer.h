#ifndef lighttimer_h
#define lighttimer_h

#include "constants.h"
#include "light.h"
const uint32_t AVAILABLE_TICKS = 65535; //@prescale 1024
const uint32_t TICKS_IN_SECOND = 15625; //@prescale 1024
extern volatile uint8_t g_state;
volatile uint8_t timer_iterations = 0;

void lightTimerInit()
{
  TCCR1A = 0;
  TCCR1B = (1 << WGM12);   //CTC mode (compare OCR1A)
}

void startLightTimer(double seconds)
{ 
  uint32_t ticks = uint32_t(seconds * TICKS_IN_SECOND);
  timer_iterations = ((ticks / AVAILABLE_TICKS) + 1);
  uint16_t one_iteration = ticks / (uint32_t)timer_iterations;
  OCR1AH = uint8_t(one_iteration >> 8);
  OCR1AL = uint8_t(one_iteration & 0x00FF);
  TIMSK1 = (1 << OCIE1A); //Interrupts enabled on OCR1 match
  TCCR1B |= (1 << CS12) | (1<< CS10); //1024 prescaler -> 15.625 kHz (actually starts timer)
}

void startLightBlinking()
{
  TIMSK1 = 0; //Disable timer 1 interrupts
  TCCR1A |= (1 << COM1A0); //Toggle OC1A (LED pin) on compare match
  OCR1AH = uint8_t(TICKS_IN_SECOND >> 8);
  OCR1AL = uint8_t(TICKS_IN_SECOND & 0x00FF);
  TCCR1B |= (1 << CS12) | (1 << CS10); //1024 prescale, actually starts timer
}

//Used to shutoff timer after done
ISR(TIMER1_COMPA_vect)
{
  uint8_t temp = SREG;
  timer_iterations--;
  if(timer_iterations == 0)
  {
    g_state = LIGHT_OFF;
    lightOff();
    TCCR1B &= ~((1 << CS12) | (1<< CS10));
  }
  SREG = temp;
}

#endif /* lightitmer_h */
