#pragma once
#ifndef PITCH_SHIFTER_MODULE_H
#define PITCH_SHIFTER_MODULE_H

#include <stdint.h>

#include "base_effect_module.h"
#include "daisysp.h"
#ifdef __cplusplus

/** @file tuner_module.h */

using namespace daisysp;

namespace bkshepherd {

class PitchShifterModule : public BaseEffectModule {
 public:
  PitchShifterModule();
  ~PitchShifterModule();

  void Init(float sample_rate) override;
  void ProcessMono(float in) override;
  void ProcessStereo(float inL, float inR) override;
  void ParameterChanged(int parameter_id) override;
};
}  // namespace bkshepherd
#endif
#endif
