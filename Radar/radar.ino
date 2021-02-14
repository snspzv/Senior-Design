
#include <KickFFT.h> //Go to line 201 and cast l as 32 bit before shifting to avoid error
#include "ADC.h"
#include "print.h"
#include "timer.h"
#include "arduinoFFT.h"

uint8_t const SAMPLE_MAX = 128; //Be careful that combined size of data_in and fft_result will not be greater than RAM
volatile uint16_t data_in[2][SAMPLE_MAX];
volatile uint8_t fill = 0;
volatile uint8_t send_fft = 2;
//volatile uint8_t timetest = 0;
volatile uint8_t samples = 0;
//volatile uint8_t count = 0;
uint16_t previous = 0;
uint16_t current = 0;


void setup() {
  ADCInit();
  timerInit();
  Serial.begin(115200);
  sei();
}

/*void loop(){
  data_in[0][0] = analogRead(A0);
  Serial.print(data_in[0][0]);
  Serial.print("\n");
  delay(10);
}*/

void loop() {
  uint32_t fft_result[SAMPLE_MAX];
  static uint8_t move_count = 0;
  //one data_in buffer is done being filled
  if(samples >= SAMPLE_MAX)
  {
    ADCSRA &= ~(1 << ADIE); //disable ADC interrupts
    //Serial.print(send_fft);
    send_fft = fill; //current fill buffer is ready to be passed to fft
    fill ^= 0x01; //toggle fill to other buffer   
    samples = 0;
    
    startTimer();//Start countdown to begin filling next buffer
    //KickFFT<volatile uint16_t>::psd(19230, 0, 7205, SAMPLE_MAX, data_in[send_fft], fft_result);
    //KickFFT<volatile uint16_t>::fft(9615, 3000, 4000, SAMPLE_MAX, data_in[send_fft], fft_result);
    
    
    for(uint8_t i = 1; i < SAMPLE_MAX; i++)
    {
      char to_send[15];
      sprintf(to_send, "%u\n", data_in[send_fft][i]);
      Serial.print(to_send);

      //Primitive way to detect movement, no direction or speed
//      if(abs((int16_t)(data_in[send_fft][i]) - (int16_t)(data_in[send_fft][i - 1])) > 1)
//      {
//        char to_send[15];
//        sprintf(to_send, "Movement!\t%u\n", move_count);
//        Serial.print(to_send);
//        move_count++;
//        break;
//      }
    }

    //Function(s) to evaluate fft results here
        //Some kind of low pass filtering
        //Speed measurement algorithm (may be as simple as selecting highest magnitude of fft)
        //https://core.ac.uk/download/pdf/52128797.pdf -> good starting point

    //Fucntion to transmit status here, pass integer number of seconds for light to turn on
        //Need to decide message format and what information it will contain
  }
  
}
