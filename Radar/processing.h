#ifndef processing_h
#define processing_h

#include "arduinoFFT.h" 
#include <utility>

#define LOWEST_MAX_VAL 50

/****CHANGE THIS BACK FOR REAL USE****/
#define ROAD_DISTANCE 5 //Distance between stations in meters
/*************************************/

extern volatile double data_Q[2][512];
extern volatile double data_I[2][512];
extern volatile uint8_t filled;
extern uint16_t const SAMPLE_MAX;
extern const uint8_t TOWARDS;
extern const uint8_t AWAY;
extern const uint8_t NO_MOVEMENT;
const uint16_t SAMPLING_FREQUENCY = 17047; //Hz
static const uint8_t BELOW = 0;
static const uint8_t WITHIN = 1;
static const uint8_t ABOVE = 2;
static const uint8_t REQUIRED_DIR_COUNT = 10;

double getPeakFreq()
{
  uint16_t rawIMax;
  double zeroes[SAMPLE_MAX] = {};
  double freqI;
  arduinoFFT FFT = arduinoFFT();
  
  FFT.DCRemoval(data_I[filled], SAMPLE_MAX);
  FFT.Windowing(data_I[filled], SAMPLE_MAX, FFT_WIN_TYP_WELCH, FFT_FORWARD);
  FFT.Compute(data_I[filled], zeroes, SAMPLE_MAX, 9, FFT_FORWARD);
  FFT.ComplexToMagnitude(data_I[filled], zeroes, SAMPLE_MAX);
  freqI = FFT.MajorPeak(data_I[filled], SAMPLE_MAX, SAMPLING_FREQUENCY, rawIMax);
  
  if(rawIMax < 5)
  {
    return 0;
  }
  return freqI;
}


//Uses Doppler equation:
  //Fd = 2 * v * cos(theta) *(Ft / c)
//Uses conversion rate of:
  //1 m/s = 2.236936 mph
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
double freqToLightTime(double freq)
{
  if(freq == 0)
  {
    return uint32_t(0);
  }

  double s = (double(1) / freqToMPS(freq)) * double(ROAD_DISTANCE);

  if(s <= double(1))
  {
    return 1;
  }

  else
  {
    return round(s);
  }
}


uint32_t lightOnTime()
{
  return(uint32_t(freqToLightTime(getPeakFreq())));
}


//Checking for "Zero crossing" with 1.615 - 1.670 treated as the zero
uint8_t getDirState(double current_reading)
{
  if(current_reading < 1.615)
  {
    return BELOW;  
  }

  else if ((current_reading >= 1.615) && (current_reading <= 1.670))
  {
    return WITHIN;
  }

  else
  {
    return ABOVE;
  }
}


uint8_t getDirection()
{
  uint8_t Q_state;
  uint8_t I_state;
  uint8_t prev_Q_state = data_Q[filled][0];
  uint8_t prev_I_state = data_I[filled][0];
  uint8_t towards_count = 0;
  uint8_t away_count = 0;
  
  for(uint16_t i = 1; i < SAMPLE_MAX; i++)
  {
    Q_state = getDirState(data_Q[filled][i]);
    I_state = getDirState(data_I[filled][i]);

    if(Q_state != I_state)
    {
      if((I_state != prev_I_state) && (++towards_count > REQUIRED_DIR_COUNT))
      {
          return TOWARDS;
      }

      else if ((Q_state != prev_Q_state) && (++away_count > REQUIRED_DIR_COUNT))
      {
          return AWAY;
      }
    }
    
    prev_Q_state = Q_state;
    prev_I_state = I_state; 
  }
  
  return NO_MOVEMENT;
}



#endif /* processing_h */
