#include "arduinoFFT.h"
#include "analogConversion.h"
#include "processing.h"
#include "radio.h"
#include <ADC.h>
#include <ADC_util.h>

volatile uint8_t buffer_status = 0;
const uint8_t BOTH_READY = 3;
const uint8_t TOWARDS = 0;
const uint8_t AWAY = 1;
const uint8_t NO_MOVEMENT = 2;

void setup() {
  Serial.begin(9600);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  ADCInit();
  radioInit();
}

void loop() {
  if(buffer_status == BOTH_READY)
  { 
    buffer_status = 0;
    uint8_t dir = getDirection();

    if((dir == NO_MOVEMENT) || (dir == AWAY))
    {
      transmit(0);
    }

    else
    {
      transmit(lightOnTime());
    }
  }
}
