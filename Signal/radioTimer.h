#ifndef radiotimer_h
#define radiotimer_h

extern volatile uint8_t state;

void initRadioTimer()
{
  
}

void startRadioTimer()
{
  
}

void restartRadioTimer()
{
  
}

ISR(/*timer interrupt vector*/)
{
  lightOnBlinking();
  state = LIGHT_BLINKING;
}
#endif /* radiotimer_h */
