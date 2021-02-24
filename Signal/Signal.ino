#include "light.h"
#include "lightTimer.h"
#include "radioTimer.h"
#include "SignalRadio.h"
#include "constants.h"

volatile uint8_t g_state = LIGHT_OFF; 
volatile bool g_packetArrived = false;

void setup() {
  cli();
  lightTimerInit();
//  radioTimerInit();
  radioInit();
  lightInit();
  sei();
//  startRadioTimer();
}

void loop() {
  if(g_packetArrived)
  {
    //    restartRadioTimer();
    g_packetArrived = false;
    double timeOn = getTime();
    if(timeOn > 0)
    {
      if(g_state == LIGHT_ON)
      {
        restartLightTimer(timeOn);
      }
  
      else if((g_state == LIGHT_OFF) || (g_state == LIGHT_BLINKING))
      {
        lightOnSolid();
        startLightTimer(timeOn);
        g_state = LIGHT_ON;
      }
    }
  
    else if ((timeOn == 0) && (g_state == LIGHT_BLINKING))
    {
      lightOff(); 
      g_state = LIGHT_OFF;
    }
  }
}
