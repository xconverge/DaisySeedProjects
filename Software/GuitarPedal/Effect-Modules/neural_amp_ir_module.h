#pragma once
#ifndef NEURAL_AMP_IR_MODULE_H
#define NEURAL_AMP_IR_MODULE_H

#include <stdint.h>

#include "base_effect_module.h"
#include "daisysp.h"

#ifdef __cplusplus

/** @file neural_amp_ir.h */

using namespace daisysp;

namespace bkshepherd {

class NeuralAmpIRModule : public BaseEffectModule {
 public:
  NeuralAmpIRModule();
  ~NeuralAmpIRModule();

  void Init(float sample_rate) override;
  void ParameterChanged(int parameter_id) override;
  void ProcessMono(float in) override;
  void ProcessStereo(float inL, float inR) override;
  float GetBrightnessForLED(int led_id) override;

 private:
  void SelectModel();
  void SelectIR();
  void CalculateTone();

  int m_currentModelIndex = -1;
  float m_nnLevelAdjust = 0;

  int m_currentIRIndex = -1;

  bool m_irEnabled = true;
  bool m_ampEnabled = true;

  float m_gainMin;
  float m_gainMax;

  float m_levelMin;
  float m_levelMax;

  float m_cutoffMin;
  float m_cutoffMax;

  Tone m_tone;  // Low Pass
};
}  // namespace bkshepherd
#endif
#endif
