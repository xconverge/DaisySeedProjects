#pragma once
#ifndef FREQUENCY_DETECTOR_CMSIS_H
#define FREQUENCY_DETECTOR_CMSIS_H

#include "frequency_detector_interface.h"

class FrequencyDetectorCMSIS : public FrequencyDetectorInterface {
 public:
  FrequencyDetectorCMSIS();
  virtual ~FrequencyDetectorCMSIS();
  void Init(float sampleRate) override;
  void Process(float in) override;
  float GetFrequency() const override { return m_cachedFrequency; }

 private:
  float m_cachedSampleRate;
  float m_cachedFrequency;

  int m_fft_buf_index = 0;
};

#endif