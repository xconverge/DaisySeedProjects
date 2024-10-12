#pragma once
#ifndef FREQUENCY_DETECTOR_Q_H
#define FREQUENCY_DETECTOR_Q_H
#include "frequency_detector_interface.h"

class FrequencyDetectorQ : public FrequencyDetectorInterface {
 public:
  FrequencyDetectorQ();
  virtual ~FrequencyDetectorQ();
  void Init(float sampleRate) override;
  void Process(float in) override;
  float GetFrequency() const override { return m_cachedFrequency; }

 private:
  float m_cachedFrequency;
};

#endif