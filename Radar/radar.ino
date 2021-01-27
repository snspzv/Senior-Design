
#include <KickFFT.h> //Go to line 201 and cast l as 32 bit before shifting to avoid error
#include "ADC.h"
#include "print.h"
#include "timer.h"

uint8_t const radarPin = A0;
uint8_t const SAMPLE_MAX = 128; //Be careful that combined size of data_in and fft_result will not be greater than RAM
//volatile uint16_t data_in[SAMPLE_MAX];  //16 bits for 0-1023 range from analog pin
volatile uint16_t data_in[2][SAMPLE_MAX];
volatile uint8_t fill = 0;
volatile uint8_t send_fft = 2;
volatile uint8_t timetest = 0;
//volatile bool arr_filled = false;


void setup() {
  ADCInit();
  Serial.begin(115200);
  timerInit();
  sei();
  //startTimer();
}

void loop() {
  uint32_t fft_result[SAMPLE_MAX];
  static uint8_t i = 1;
  //Serial.print(i);
  if(send_fft != 2)
  {
    //uint16_t elapsed = micros();
    startTimer();
    KickFFT<volatile uint16_t>::fft(500, 0, 500, SAMPLE_MAX, data_in[send_fft], fft_result);
    //send_fft = 2;
    
    //elapsed = elapsed - micros();
    Serial.print(i);
    i++;

    //arrayToSerial<uint32_t>(fft_result, SAMPLE_MAX, true);

    //Function to evaluate fft results here

    //Fucntion to transmit status here
    
  }


}
