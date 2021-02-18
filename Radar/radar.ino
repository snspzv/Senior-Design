
//#include <KickFFT.h> //Go to line 201 and cast l as 32 bit before shifting to avoid error
#include "arduinoFFT.h" //Comment out all instances of vImag array so bigger dataset can be used
                        //vImag is always 0 in our case 
#include "ADC.h"
#include "print.h"
#include "timer.h"
#include "arduinoFFT.h"

uint8_t const SAMPLE_MAX = 128; //Be careful that combined size of data_in and fft_result will not be greater than RAM
volatile double data_in[2][SAMPLE_MAX];
volatile uint8_t fill = 0;
volatile uint8_t calc_fft = 2;
//volatile uint8_t timetest = 0;
volatile uint8_t samples = 0;
//volatile uint8_t count = 0;
uint16_t previous = 0;
uint16_t current = 0;
arduinoFFT FFT = arduinoFFT();
uint16_t SAMPLING_FREQUENCY = 9615; //Hz

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
  //uint32_t fft_result[SAMPLE_MAX];
  //static uint8_t move_count = 0;
  //one data_in buffer is done being filled
  if(samples >= SAMPLE_MAX)
  {
    ADCSRA &= ~(1 << ADIE); //disable ADC interrupts
    //Serial.print(send_fft);
    calc_fft = fill; //current fill buffer is ready to be passed to fft
    fill ^= 0x01; //toggle fill to other buffer   
    samples = 0;
    
    startTimer();//Start countdown to begin filling next buffer
    //KickFFT<volatile uint16_t>::psd(19230, 0, 7205, SAMPLE_MAX, data_in[calc_fft], fft_result);
    //KickFFT<volatile uint16_t>::fft(9615, 3000, 4000, SAMPLE_MAX, data_in[calc_fft], fft_result);
    
    uint16_t maxVal = 0;
    /*FFT*/
    FFT.DCRemoval(data_in[calc_fft], SAMPLE_MAX);
    FFT.Windowing(data_in[calc_fft], SAMPLE_MAX, FFT_WIN_TYP_HANN, FFT_FORWARD);
    FFT.Compute(data_in[calc_fft], SAMPLE_MAX, FFT_FORWARD);
    FFT.ComplexToMagnitude(data_in[calc_fft], SAMPLE_MAX);
    double peak = FFT.MajorPeak(data_in[calc_fft], SAMPLE_MAX, SAMPLING_FREQUENCY, maxVal);
    

        for(int i=0; i<(SAMPLE_MAX/2); i++)
    {
        /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
         
//        Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLE_MAX, 1);
//        Serial.print(" ");
        Serial.println(data_in[calc_fft][i], 1);    //View only this line in serial plotter to visualize the bins
    }
    
    /*for(uint8_t i = 1; i < SAMPLE_MAX; i++)
    {
//      char to_send[15];
//      sprintf(to_send, "%u\n", data_in[send_fft][i]);
//      Serial.print(to_send);

      //Primitive way to detect movement, no direction or speed
      if(abs((int16_t)(data_in[send_fft][i]) - (int16_t)(data_in[send_fft][i - 1])) > 1)
      {
        char to_send[15];
        sprintf(to_send, "Movement!\t%u\n", move_count);
        Serial.print(to_send);
        move_count++;
        break;
      }
    }*/
    /*if(movement)
    {
      send_function();
    }*/

  }
  
}
