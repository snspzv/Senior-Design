#ifndef processing_h
#define processing_h

#include "arduinoFFT.h" 

#define LOWEST_MAX_VAL 50

/****CHANGE THIS BACK FOR REAL USE****/
#define ROAD_DISTANCE 50 //Distance between stations in meters
/*************************************/

extern volatile double data_in[2][128];
extern uint8_t const SAMPLE_MAX;
uint16_t SAMPLING_FREQUENCY = 18490; //Hz
arduinoFFT FFT = arduinoFFT();


double getPeakFreq(uint16_t & rawRealMax, uint16_t & rawImagMax, uint8_t filled)
{
  for(uint8_t i = 0; i < 128; i++)
  {
    Serial.print(data_real[filled][i]);
    Serial.print(",");
    Serial.println(data_imag[filled][i]);
  }
  
  FFT.DCRemoval(data_real[filled], SAMPLE_MAX);
  FFT.DCRemoval(data_imag[filled], SAMPLE_MAX);
  
  FFT.Windowing(data_real[filled], SAMPLE_MAX, FFT_WIN_TYP_HANN, FFT_FORWARD);
  FFT.Windowing(data_imag[filled], SAMPLE_MAX, FFT_WIN_TYP_HANN, FFT_FORWARD);
 
  FFT.Compute(data_real[filled], data_imag[filled], SAMPLE_MAX, FFT_FORWARD);
  FFT.ComplexToMagnitude(data_real[filled], data_imag[filled], SAMPLE_MAX);

  //**********Check on other MajorPeak function to possibly combine**************//
  FFT.MajorPeak(data_imag[filled], SAMPLE_MAX, SAMPLING_FREQUENCY, rawImagMax);
  return(FFT.MajorPeak(data_real[filled], SAMPLE_MAX, SAMPLING_FREQUENCY, rawRealMax));
}


//Uses Doppler equation:
  //Fd = 2 * v * cos(theta) *(Ft / c)
//Uses conversion rate of:
  //1 m/s = 2.236936 mph
//WILL NEED TO ACCOUNT FOR THETA
double freqToMPH(double freq)
{
  return (freq * double(0.013557)); 
}

double freqToMPS(double freq)
{
  return (freq * double(0.006061));
}

// (1/mps) * meters = number of seconds for car at current speed
//                    to travel to top of hill
//Returns time light should be on in seconds 
uint32_t freqToLightTime(double freq)
{
  if(freq == 0)
  {
    return uint32_t(0);
  }
  return (double(1) / freqToMPS(freq)) * double(ROAD_DISTANCE);
}

double dopplerFreq(uint8_t filled)
{
  uint16_t realMaxVal = 0;
  uint16_t imagMaxVal = 0;
  double realPeakFreq = getPeakFreq(realMaxVal, imagMaxVal, filled);
  
  //check if amplitude at peak frequency is below movement threshold
  //probably will need to adjust this value later
  if(realMaxVal < LOWEST_MAX_VAL)
  {
    return 0;
  }

  return realPeakFreq;
  
}

#endif /* processing_h */
