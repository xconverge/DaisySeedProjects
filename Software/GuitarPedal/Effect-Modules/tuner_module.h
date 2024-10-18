#pragma once
#ifndef TUNER_MODULE_H
#define TUNER_MODULE_H

#include <stdint.h>

#include "../Util/frequency_detector_interface.h"
#include "base_effect_module.h"
#include "daisysp.h"
#ifdef __cplusplus

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
  void DrawUI(OneBitGraphicsDisplay& display, int currentIndex,
              int numItemsTotal, Rectangle boundsToDrawIn,
              bool isEditing) override;

 private:
  float m_currentFrequency = 0;

  FrequencyDetectorInterface* m_frequencyDetector = nullptr;
};
}  // namespace bkshepherd
#endif
#endif
