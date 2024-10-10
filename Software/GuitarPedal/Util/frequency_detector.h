#pragma once
#ifndef FREQUENCY_DETECTOR_H
#define FREQUENCY_DETECTOR_H
// Reference
// https://github.com/sonofmagni/FreqDetection/blob/main/FreqDetect.cpp

#include "daisysp.h"

using namespace daisysp;
using namespace std;

#define AVE_BUF_SIZE 8

struct LowPassFilter {
  Svf filt;
  float amp;

  void Init(float samplerate, float freq) {
    filt.Init(samplerate);
    filt.SetRes(0.01);
    filt.SetDrive(.002);
    filt.SetFreq(freq);
  }

  float Process(float in) {
    filt.Process(in);
    return filt.Low();
  }
};

class FrequencyDetector {
 public:
  FrequencyDetector();
  virtual ~FrequencyDetector();
  void Init(float sample_rate);
  void Process(float input);
  float GetPeakPeakInterval();

 private:
  float CalcMovAve();
  void AddMovAveValue(float val);
  void ResetBuffer();

  int m_lockPos = 0;
  int m_pos = 0;

  float m_sampleSum = 0;
  int m_sampleIndex = 0;
  int m_halfPeriodSampleCount = 0;
  int m_lengthNonZero = 1;

  int m_aveBufPtr = 0;
  float m_movAve = 0;
  float m_aveBuffer[AVE_BUF_SIZE];

  LowPassFilter m_lpFilter;
};

#endif