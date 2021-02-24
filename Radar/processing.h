#ifndef processing_h
#define processing_h

#include "arduinoFFT.h" //Comment out all instances of vImag array so bigger dataset can be used
                        //vImag is always 0 in our case 
#define LOWEST_MAX_VAL 30

/****CHANGE THIS BACK FOR REAL USE****/
#define ROAD_DISTANCE 30 //Distance between stations in meters
/*************************************/

extern volatile double data_in[2][128];
extern uint8_t const SAMPLE_MAX;
uint16_t SAMPLING_FREQUENCY = 9615; //Hz
arduinoFFT FFT = arduinoFFT();


double getPeakFreq(uint16_t & rawMaxVal, uint8_t filled)
{
  FFT.DCRemoval(data_in[filled], SAMPLE_MAX);
  FFT.Windowing(data_in[filled], SAMPLE_MAX, FFT_WIN_TYP_HANN, FFT_FORWARD);
  FFT.Compute(data_in[filled], SAMPLE_MAX, FFT_FORWARD);
  FFT.ComplexToMagnitude(data_in[filled], SAMPLE_MAX);
  return(FFT.MajorPeak(data_in[filled], SAMPLE_MAX, SAMPLING_FREQUENCY, rawMaxVal));
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
  uint16_t maxVal = 0;
  double peakFreq = getPeakFreq(maxVal, filled);

  //check if amplitude at peak frequency is below movement threshold
  //probably will need to adjust this value later
  if(maxVal < LOWEST_MAX_VAL)
  {
    return 0;
  }

  return peakFreq;
  
}

#endif /* processing_h */
