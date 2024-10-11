#pragma once
#ifndef FREQUENCY_DETECTOR_ALT_H
#define FREQUENCY_DETECTOR_ALT_H

class FrequencyDetectorAlt {
 public:
  FrequencyDetectorAlt();
  virtual ~FrequencyDetectorAlt();
  void Init(float sampleRate);
  void Process(float in);
  float GetFrequency() const { return m_cachedFrequency; }

 private:
  float m_cachedFrequency;
  int m_bufferIndex;
};

#endif