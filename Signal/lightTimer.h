#ifndef lighttimer_h
#define lighttimer_h

#include "constants.h"
#include "light.h"
static const uint32_t AVAILABLE_TICKS = 255; //@prescale 1024
extern volatile uint8_t g_state;
static volatile uint16_t timer_iterations;

void lightTimerInit()
{
  TCCR2A = (1 << WGM21);
}

void startLightTimer(double seconds)
{ 
  uint32_t ticks = uint32_t(seconds * TICKS_IN_SECOND);
  timer_iterations = ((ticks / AVAILABLE_TICKS) + 1);
  uint8_t one_iteration = uint8_t(ticks / (uint32_t)timer_iterations);
  OCR2A = one_iteration;
  TIMSK2 = (1 << OCIE2A); //Interrupts enabled on OCR1 match
  TCCR2B = ((1 << CS22) | (1<<CS21) | (1<< CS20)); //1024 prescaler -> 15.625 kHz (actually starts timer)
}

void restartLightTimer(double seconds)
{
  TIMSK2 &= ~(1 << OCIE2A);
  TCCR2B &= ~((1 << CS22) | (1<<CS21) | (1<< CS20)); //Stop timer to restart w/ new values
  TCNT2 = 0;
  startLightTimer(seconds);
}

//Interrupt triggered after one "iteration" of timer is complete
//Allows timer to be set to times larger than its OCR will allow
//Ex with prescale 255: 300 ticks needed, timer will run for 150 ticks twice
ISR(TIMER2_COMPA_vect)
{ 
  uint8_t temp = SREG;
  timer_iterations--;
  if(timer_iterations == 0)
  {
    g_state = LIGHT_OFF;
    lightOff();
    TCCR2B &= ~((1 << CS22) | (1<<CS21) | (1<< CS20));
  }
  SREG = temp;
}

#endif /* lightitmer_h */
