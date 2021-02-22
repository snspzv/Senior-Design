#include "light.h"
#include "lightTimer.h"
#include "radioTimer.h"
#include "SignalRadio.h"
#define LIGHT_OFF 0
#define LIGHT_ON 1
#define LIGHT_BLINKING 2

volatile uint8_t g_state = LIGHT_OFF; 

void setup() {
  // put your setup code here, to run once:
  lightTimerInit();
  radioTimerInit();
  RadioSetup();
  lightInit();
  startRadioTimer();
}

void loop() {
  
  if(dataReceived())
  {
    restartRadioTimer();
  
    if(timeOn > 0)
    {
      if(g_state == LIGHT_ON)
      {
        restartLightTimer();
      }
  
      else if((g_state == LIGHT_OFF) || (g_state == LIGHT_BLINKING))
      {
        lightOnSolid();
        startLightTimer();
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