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
//        Serial.print("A: ");
//      Serial.print(TCCR1A, HEX);
//      Serial.print("\tB: ");
//      Serial.println(TCCR1B, HEX);
}

void loop() {
  if(g_packetArrived)
  {
//    EIMSK &= ~(1 << INT1); 
    if(g_state == LIGHT_BLINKING)
    {
      stopRadioTimer();
      startRadioTimer();
//      Serial.print("A: ");
//      Serial.print(TCCR1A, HEX);
//      Serial.print("\tB: ");
//      Serial.println(TCCR1B, HEX);
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
//        Serial.println("LIGHT_ON");
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

//    EIMSK |= (1 << INT1); 
  }
//  lightOnSolid();
//  delay(2000);
//  lightOnSolid();
//  startLightTimer(3);
//  restartLightTimer(5);
//  while(1);
}
