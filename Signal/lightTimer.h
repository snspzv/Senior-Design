#ifndef lighttimer_h
#define lighttimer_h

#include "constants.h"
#include "light.h"
static const uint32_t AVAILABLE_TICKS = 255; //@prescale 1024
static const uint32_t TICKS_IN_SECOND = 15625; //@prescale 1024
extern volatile uint8_t g_state;
static volatile uint8_t timer_iterations;

// FIX TO RUN ON TIMER 2 OR 0
void lightTimerInit()
{
  TCCR2A = 0;
  TCCR2B = (1 << WGM22);   //CTC mode (compare OCR1A)
}
// has to be transmitted as a double, thus casting 
void startLightTimer(double seconds)
{ 
  uint32_t ticks = uint32_t(seconds * TICKS_IN_SECOND);
  timer_iterations = ((ticks / AVAILABLE_TICKS) + 1);
  uint8_t one_iteration = ticks / (uint32_t)timer_iterations;
  TCCR2B = (1 << WGM22);
  OCR2A = uint8_t(one_iteration);
  TIMSK2 = (1 << OCIE2A); //Interrupts enabled on OCR1 match
  TCCR2B |= ((1 << CS22) | (1<<CS21) | (1<< CS20)); //1024 prescaler -> 15.625 kHz (actually starts timer)
}

void restartLightTimer(double seconds)
{
  TCCR2B &= ~((1 << CS22) | (1<<CS21) | (1<< CS20)); //Stop timer to restart w/ new values
  startLightTimer(seconds);
}

//is startlightblinking just blinking for a second on and off?
//run into the issue with ocr2, not sure how to fix at this moment
/*
void startLightBlinking()
{
  TIMSK2 = 0; //Disable timer 1 interrupts
  TCCR2A |= (1 << COM1A0); //Toggle OC1A (LED pin) on compare match
  OCR1AH = uint8_t(TICKS_IN_SECOND >> 8);
  OCR1AL = uint8_t(TICKS_IN_SECOND & 0x00FF);
  //***TODO check prescale for either timer2 or timer0 
  TCCR2B |= ((1 << CS22) | (1<<CS21) | (1<< CS20)); //1024 prescale, actually starts timer
}
*/

//Used to shutoff timer after done
ISR(TIMER2_COMPA_vect)
{
  uint8_t temp = SREG;
  timer_iterations--;
  if(timer_iterations == 0)
  {
    g_state = LIGHT_OFF;
    lightOff();
    TCCR1B &= ~((1 << CS22) | (1<<CS21) | (1<< CS20));
  }
  SREG = temp;
}

#endif /* lightitmer_h */
