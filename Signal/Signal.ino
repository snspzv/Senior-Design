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
 // startLightTimer(5);
  sei();
  startRadioTimer();
}

void loop() {
  if(g_packetArrived)
  {
    g_packetArrived = false;
    double timeOn = getTime();
    if(timeOn > 0)
    {
      Serial.println(timeOn);
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

//    else if (g_state == LIGHT_ON)
//    {
//      
//    }
//    else if ((timeOn == 0) && (g_state == LIGHT_BLINKING))
//    {
//      lightOff(); 
//      g_state = LIGHT_OFF;
//    }
    //startRadioTimer();
  }
}
