#ifndef processing_h
#define processing_h

#include "arduinoFFT.h" //Comment out all instances of vImag array so bigger dataset can be used
                        //vImag is always 0 in our case 

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

double findSpeed(uint8_t filled)
{
  uint16_t maxVal = 0;
  double peakFreq = getPeakFreq(maxVal, filled);

  //check if amplitude at peak frequency is below movement threshold
  //probably will need to adjust this value later
  if(maxVal < 20)
  {
    return 0;
  }

  return freqToMPH(peakFreq);
  
}

#endif /* processing_h */
