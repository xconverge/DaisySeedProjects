#pragma once
#ifndef TUNER_MODULE_H
#define TUNER_MODULE_H

#include <stdint.h>

#include "../Util/frequency_detector.h"
#include "../Util/frequency_detector_alt.h"
#include "base_effect_module.h"
#include "daisysp.h"
#ifdef __cplusplus

#define USE_ALT_FREQ_DETECTOR

/** @file tuner_module.h */

using namespace daisysp;

namespace bkshepherd {

class TunerModule : public BaseEffectModule {
 public:
  TunerModule();
  ~TunerModule();

  void Init(float sample_rate) override;
  void ProcessMono(float in) override;
  void ProcessStereo(float inL, float inR) override;
  void DrawUI(OneBitGraphicsDisplay &display, int currentIndex,
              int numItemsTotal, Rectangle boundsToDrawIn,
              bool isEditing) override;

 private:
  float m_currentFrequency;
#ifdef USE_ALT_FREQ_DETECTOR
  FrequencyDetectorAlt m_frequencyDetector;
#else
  FrequencyDetector m_frequencyDetector;
#endif
};
}  // namespace bkshepherd
#endif
#endif
