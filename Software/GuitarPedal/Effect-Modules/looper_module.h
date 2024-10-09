#pragma once
#ifndef LOOPER_MODULE_H
#define LOOPER_MODULE_H

#include <stdint.h>

#include "base_effect_module.h"
#include "daisysp.h"
#ifdef __cplusplus

/** @file looper_module.h */

using namespace daisysp;

namespace bkshepherd {

class LooperModule : public BaseEffectModule {
 public:
  LooperModule();
  ~LooperModule();

  void Init(float sample_rate) override;
  void ProcessMono(float in) override;
  void ProcessStereo(float inL, float inR) override;
  float GetBrightnessForLED(int led_id) override;
  void AlternateFootswitchPressed() override;
  void AlternateFootswitchHeld() override;

 private:
  // Loopers and the buffers they'll use
  Looper m_looperL;
  Looper m_looperR;
};
}  // namespace bkshepherd
#endif
#endif