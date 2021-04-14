#ifndef processing_h
#define processing_h

#include "arduinoFFT.h" 
#include <utility>

#define LOWEST_MAX_VAL 50

/****CHANGE THIS BACK FOR REAL USE****/
#define ROAD_DISTANCE 50 //Distance between stations in meters
/*************************************/

extern volatile double data_Q[2][512];
extern volatile double data_I[2][512];
extern uint16_t const SAMPLE_MAX;
uint16_t SAMPLING_FREQUENCY = 17047; //Hz
arduinoFFT FFT = arduinoFFT();


std::pair<double, double> getPeakFreq(uint16_t & rawQMax, uint16_t & rawIMax, uint8_t filled)
{

//  FFT.DCRemoval(data_Q[filled], SAMPLE_MAX);
//  FFT.DCRemoval(data_I[filled], SAMPLE_MAX);
//
//
//  FFT.Windowing(data_Q[filled], SAMPLE_MAX, FFT_WIN_TYP_HANN, FFT_FORWARD);
//  FFT.Windowing(data_I[filled], SAMPLE_MAX, FFT_WIN_TYP_HANN, FFT_FORWARD);

  FFT.Compute(data_I[filled], data_Q[filled], SAMPLE_MAX, FFT_FORWARD);

  //FFT.ComplexToMagnitude(data_I[filled], data_Q[filled], SAMPLE_MAX);

//    for(uint16_t i = 0; i < 128; i++)
//  {
//    Serial.print(uint16_t(data_I[filled][i]), DEC);
//    Serial.print(",");
//    Serial.println(uint16_t(data_Q[filled][i]), DEC);
//  }

  
////  //**********Check on other MajorPeak function to possibly combine**************//
  //FFT.MajorPeak(data_I[filled], SAMPLE_MAX, SAMPLING_FREQUENCY, rawImagMax);
  return std::make_pair(FFT.MajorPeak(data_Q[filled], SAMPLE_MAX, SAMPLING_FREQUENCY, rawQMax),FFT.MajorPeak(data_I[filled], SAMPLE_MAX, SAMPLING_FREQUENCY, rawIMax) );

}


//Uses Doppler equation:
  //Fd = 2 * v * cos(theta) *(Ft / c)
//Uses conversion rate of:
  //1 m/s = 2.236936 mph
//WILL NEED TO ACCOUNT FOR THETA
double freqToMPH(double freq)
{
  return (freq * double(0.011118)); 
}

double freqToMPS(double freq)
{
  return (freq * double(0.02487));
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

std::pair<double, double> dopplerFreq(uint8_t filled)
{
  uint16_t QMaxVal = 0;
  uint16_t IMaxVal = 0;
  std::pair<double, double> peakFreqs = getPeakFreq(QMaxVal, IMaxVal, filled);
  
  //check if amplitude at peak frequency is below movement threshold
  //probably will need to adjust this value later
//  if(IMaxVal < LOWEST_MAX_VAL)
//  {
//    return 0;
//  }

  return peakFreqs;
  
}

#endif /* processing_h */
