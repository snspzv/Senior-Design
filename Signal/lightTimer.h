#ifndef lighttimer_h
#define lighttimer_h

#include "constants.h"
#include "light.h"
const double AVAILABLE_TICKS = 65536; //@prescale 1024
extern volatile uint8_t g_state;
static volatile uint8_t wholeLoopCount; 
static volatile uint8_t wholeLoops;

void lightTimerInit()
{
  TCCR1A = (1 << COM1A0); //Toggle OC1A on compare match (toggle LED)
  OCR1AH = 0;
  OCR1AL = 0;
  TCCR1B = (1 << WGM12);   //CTC mode (compare OCR1A)
  TIMSK1 = (1 << TOIE1); //Interrupts enabled on overflow and OCR1 match 
}

void startLightTimer(double seconds)
{
  double timerLoops = seconds / double(4.19423);
  wholeLoops = floor(timerLoops);
  double partialLoop = timerLoops - double(wholeLoops);
  uint16_t compare = uint16_t(partialLoop * AVAILABLE_TICKS);
  OCR1AH = uint8_t(compare >> 8);
  OCR1AL = uint8_t(compare & 0x00FF);
//  Serial.println(wholeLoops);
//  Serial.println(partialLoop, 6);
//  Serial.println(compare);
  //If loop is long enough to overflow OCR1A
  //Split into multiple timer calls using interrupt
  if(wholeLoops != 0)
  {
    wholeLoopCount = 0;
    TCCR1A &= ~(1 << COM1A0); //disable toggle on compare match, we're just looking for overflows now
    TCCR1B &= ~(1 << WGM12); //change to normal mode so top at 0xFFFF instead of OCR
  }
  TCCR1B |= (1 << CS12) | (1<< CS10); //1024 prescaler -> 15.625 kHz (actually starts timer)
}

//Used when time light needs to be on causes timer to overflow
ISR(TIMER1_OVF_vect)
{
  uint8_t temp = SREG;
  wholeLoopCount++;
  if(wholeLoopCount == wholeLoops)
  {
    //TCCR1A |= (1 << COM1A0); //Toggle OC1A on compare match (toggle LED) 
    TCCR1B |= (1 << WGM12); //change to CTC mode so top at OCR and can toggle when that value is hit
    TIMSK1 &= ~(1 << TOIE1);
    TIMSK1 |= (1 << OCIE1A);
  }
  SREG = temp;
}

//Used to shutoff timer after done
ISR(TIMER0_COMPA_vect)
{
  
  digitalWrite(RELAY, LOW);
  TCCR1B &= ~((1 << CS12) | (1 << CS10)); //Stop timer
}

#endif /* lightitmer_h */
