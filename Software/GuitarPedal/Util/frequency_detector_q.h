#pragma once
#ifndef FREQUENCY_DETECTOR_Q_H
#define FREQUENCY_DETECTOR_Q_H

class FrequencyDetectorQ {
 public:
  FrequencyDetectorQ();
  virtual ~FrequencyDetectorQ();
  void Init(float sampleRate);
  void Process(float in);
  float GetFrequency() const { return m_cachedFrequency; }

 private:
  float m_cachedFrequency;
};

#endif