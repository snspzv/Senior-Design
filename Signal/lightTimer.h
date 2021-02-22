#ifndef lighttimer_h
#define lighttimer_h

extern volatile uint8_t g_state;

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
  g_state = LIGHT_OFF;
}
#endif /* lightitmer_h */
