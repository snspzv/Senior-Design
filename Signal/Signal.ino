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
  radioTimerInit();
  radioInit();
  lightInit();
  Serial.begin(115200);
  sei();
  startRadioTimer();
}

void loop() {
  if(g_packetArrived)
  {
    if(g_state == LIGHT_BLINKING)
    {
      stopRadioTimer();
      startRadioTimer();
      g_state = LIGHT_OFF;
    }

    else
    {
      restartRadioTimer();
    }
    
    g_packetArrived = false;
    uint32_t timeOn = getTime();
    if(timeOn > 0)
    {      
      if(g_state == LIGHT_ON)
      {
        lightOnSolid();
        restartLightTimer(timeOn);
      }
  
      if((g_state == LIGHT_OFF) || (g_state == LIGHT_BLINKING))
      {
        lightOnSolid();
        startLightTimer(timeOn);
        g_state = LIGHT_ON;
      }
    }
  }
}
