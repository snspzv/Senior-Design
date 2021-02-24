#include "light.h"
#include "lightTimer.h"
#include "radioTimer.h"
#include "SignalRadio.h"
#include "constants.h"

volatile uint8_t g_state = LIGHT_ON; 

void setup() {
  // put your setup code here, to run once:
  
  lightTimerInit();
//  radioTimerInit();
//  RadioSetup();
  lightInit();
//  startRadioTimer();
  Serial.begin(9600);
  sei();
  startLightTimer(double(12.7));
}

void loop() {

//  if(dataReceived())
//  {
//    restartRadioTimer();
//  
//    if(timeOn > 0)
//    {
//      if(g_state == LIGHT_ON)
//      {
//        restartLightTimer();
//      }
//  
//      else if((g_state == LIGHT_OFF) || (g_state == LIGHT_BLINKING))
//      {
//        lightOnSolid();
//        startLightTimer();
//        g_state = LIGHT_ON;
//      }
//    }
//  
//    else if ((timeOn == 0) && (g_state == LIGHT_BLINKING))
//    {
//      lightOff(); 
//      g_state = LIGHT_OFF;
//    }
//  }
    

}
