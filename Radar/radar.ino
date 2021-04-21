
//#include <KickFFT.h> //Go to line 201 and cast l as 32 bit before shifting to avoid error
#include "arduinoFFT.h"
#include "analogConversion.h"
//#include "timer.h"
#include <arduinoFFT.h>
#include "processing.h"
//#include "RadarRadio.h"
#include "radioTest.h"
#include <ADC.h>
#include <ADC_util.h>
uint16_t const SAMPLE_MAX = 512; //Be careful that combined size of data_in and fft_result will not be greater than RAM
volatile double data_Q[2][SAMPLE_MAX] = {};
volatile double data_I[2][SAMPLE_MAX] = {};
volatile uint8_t currently_filling = 0;
volatile uint8_t filled = 2;
volatile uint16_t samples_Q = 0;
volatile uint16_t samples_I = 0;
extern ADC *adc;
volatile uint8_t buffer_status = 0;
const uint8_t BOTH_READY = 3;
#include <utility>

void setup() {
  //__disable_irq();
  Serial.begin(9600);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  ADCInit();
  //timerInit();
  radioInit();
  //__enable_irq();
}
int t = 0;
int last_taken = 0;
void loop() {

  double zeroes[SAMPLE_MAX] = {};
  if(buffer_status == BOTH_READY)
  { 
//    t = micros();
    buffer_status = 0;
//    FFT.DCRemoval(data_Q[filled], SAMPLE_MAX);
    FFT.DCRemoval(data_I[filled], SAMPLE_MAX);
//    FFT.Windowing(data_Q[filled], SAMPLE_MAX, FFT_WIN_TYP_WELCH, FFT_FORWARD);
    FFT.Windowing(data_I[filled], SAMPLE_MAX, FFT_WIN_TYP_WELCH, FFT_FORWARD);
    FFT.Compute(data_I[filled], zeroes, SAMPLE_MAX,9, FFT_FORWARD);
//    FFT.Compute(data_Q[filled], zeroes, SAMPLE_MAX, FFT_FORWARD);
    FFT.ComplexToMagnitude(data_I[filled], zeroes, SAMPLE_MAX);
//    FFT.ComplexToMagnitude(data_Q[filled], zeroes, SAMPLE_MAX);
    uint16_t rawIMax, rawQMax;
    double freqI = FFT.MajorPeak(data_I[filled], SAMPLE_MAX, SAMPLING_FREQUENCY, rawIMax);
//    double freqQ = FFT.MajorPeak(data_Q[filled], SAMPLE_MAX, SAMPLING_FREQUENCY, rawQMax);
//    Serial.println( micros() - t);
//
//    if(rawIMax > 5)
//    {
//      Serial.print("MPH: ");
//      Serial.print(freqToMPH(freqI)); 
//      Serial.print("\tHz: ");
//      Serial.print(freqI);
//      Serial.print("\tMagnitude: ");
//      Serial.println(rawIMax);
        if(rawIMax < 5)
        {
          freqI = 0;
        }
//        Serial.println(uint32_t((freqToLightTime(freqI))));
      transmit(uint32_t(freqToLightTime(freqI)));// * 1000));
//    transmit(16);
    //delay(1000);
////      Serial.print("MPH: ");
////      Serial.print(freqToMPH(freqQ)); 
////      Serial.print("\tHz: ");
////      Serial.print(freqQ);
////      Serial.print("\tMagnitude: ");
////      Serial.println(rawQMax);
//    }
////
////    else
////    {
////      last_taken = 0;
////    }
//Serial.println(micros() - t);
//t = micros();
//    else
//      Serial.println(0);
//    for(int i = 0; i < 512; i++)
//    {
//      Serial.print(data_I[filled][i] * (double(3.3) / double(4095)), DEC);
//      Serial.print(" ");
//      Serial.println(data_Q[filled][i] * (double(3.3) / double(4095)), DEC);
//    }
//    delay(2000);
//    std::pair<double,double> freqs = dopplerFreq(filled);
//    Serial.print("Q freq: ");
//    Serial.print(freqs.first);
//    Serial.print("\tI freq: ");
//    Serial.println(freqs.second);
  }
}
