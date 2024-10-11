#pragma once
#ifndef FREQUENCY_DETECTOR_H
#define FREQUENCY_DETECTOR_H

class FrequencyDetector {
 public:
  FrequencyDetector();
  virtual ~FrequencyDetector();
  void Init(float sampleRate);
  float Process(float in);
};

#endif