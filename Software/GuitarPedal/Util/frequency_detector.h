#pragma once
#ifndef FREQUENCY_DETECTOR_H
#define FREQUENCY_DETECTOR_H

class FrequencyDetector {
 public:
  FrequencyDetector();
  virtual ~FrequencyDetector();
  void Init(float sampleRate);
  void Process(float in);
  float GetFrequency() const { return m_cachedFrequency; }

 private:
  float m_cachedSampleRate;
  float m_cachedFrequency;

  int m_fft_buf_index = 0;
};

#endif