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
     //CTC mode (compare OCR1A)
}

void startLightTimer(double seconds)
{ 
  uint32_t ticks = uint32_t(seconds * TICKS_IN_SECOND);
  timer_iterations = ((ticks / AVAILABLE_TICKS) + 1);
  uint8_t one_iteration = uint8_t(ticks / (uint32_t)timer_iterations);
//  Serial.println(ticks);
//  Serial.println(timer_iterations);
//  Serial.println(one_iteration);
  OCR2A = one_iteration;
//  Serial.println(OCR2A);
  TIMSK2 = (1 << OCIE2A); //Interrupts enabled on OCR1 match
  TCCR2B = ((1 << CS22) | (1<<CS21) | (1<< CS20)); //1024 prescaler -> 15.625 kHz (actually starts timer)
//  Serial.println(TCCR2A, HEX);
//  Serial.println(OCR2A, HEX);
//  Serial.println(TIMSK2, HEX);
//  Serial.println(TCCR2B, HEX);
  //lightOnSolid();
}

void restartLightTimer(double seconds)
{
  TIMSK2 &= ~(1 << OCIE2A);
  TCCR2B &= ~((1 << CS22) | (1<<CS21) | (1<< CS20)); //Stop timer to restart w/ new values
  TCNT2 = 0;
  startLightTimer(seconds);
}

//Used to shutoff timer after done
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
