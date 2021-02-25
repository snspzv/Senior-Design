#ifndef radiotimer_h
#define radiotimer_h
//
//#include "lightTimer.h"
//extern volatile uint8_t g_state;
//
/*
 * static const uint8_t OCR_MAX = 65535; 
   static const uint16_t TICKS_IN_SECOND = 15625; //@prescale 1024
  
   //time is set in 500 milliseconds ASSUMPTION IS THAT WE WILL NOT GO ABOVE 999 ms 
   static const uint16_t TIME = ((500 * TICKS_IN_SECOND)/ (uint16_t)1000);
 * 
 */

/*
 

 //sets timer 1 to be in ctc, with cycles needed equating to 500 ms in ocr
void initRadioTimer()
{
  
  TCCR1A = 0x00;
  TCCR1B = (1<<WGM12); //CTC mode

  //upper bits pushed in high
  OCR1AH = (TIME>>8);
  //get lower 8 bits into Low
  OCR1AL = (TIME & 0x00FF);
  
}
*/
/*
 //just use a const and no param, just set prescale ->101
void startRadioTimer()
{
    TCCR1B |= ((1<<CS12)| (1<<CS20)); // set prescale to 1024
}

/*
void stopRadioTimer()
{
  TCCR1B |= ((0<<CS12)| (0<<CS10));
  or TCCR1B &= ~((1<<CS12)|(1<<CS10));
}
*/
/*
ISR(/*timer interrupt vector)
{
  startLightBlinking();
  g_state = LIGHT_BLINKING;
  stopRadioTimer();
}
*/
#endif /* radiotimer_h */
