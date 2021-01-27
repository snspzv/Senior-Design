
#include <KickFFT.h> //Go to line 201 and cast l as 32 bit before shifting to avoid error
#include "ADC.h"
#include "print.h"
#include "timer.h"

uint8_t const SAMPLE_MAX = 128; //Be careful that combined size of data_in and fft_result will not be greater than RAM
volatile uint16_t data_in[2][SAMPLE_MAX];
volatile uint8_t fill = 0;
volatile uint8_t send_fft = 2;
volatile uint8_t timetest = 0;

void setup() {
  ADCInit();
  timerInit();
  sei();
}

void loop() {
  uint32_t fft_result[SAMPLE_MAX];

  if(send_fft != 2)
  {
    startTimer(); //Begin countdown to restart ADC interrupts
    
    //Get more accurate timing of FFT function to use timer more effectively
    //Find out about frequency bounds to make function faster
    //Look into faster alternatives
    KickFFT<volatile uint16_t>::fft(500, 0, 500, SAMPLE_MAX, data_in[send_fft], fft_result);

    //Function(s) to evaluate fft results here
        //Some kind of low pass filtering
        //Speed measurement algorithm (may be as simple as selecting highest magnitude of fft)
        //https://core.ac.uk/download/pdf/52128797.pdf -> good starting point

    //Fucntion to transmit status here
        //Need to decide message format and what information it will contain
  }

}
