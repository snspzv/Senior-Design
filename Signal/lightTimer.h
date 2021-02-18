#ifndef lighttimer_h
#define lighttimer_h

extern volatile uint8_t state;

void lightTimerInit()
{
  
}

void startLightTimer()
{
  
}

void restartLightTimer()
{
  
}

ISR(/*timer interrupt vecotr*/)
{
  lightOff();
  state = LIGHT_OFF;
}
#endif /* lightitmer_h */
