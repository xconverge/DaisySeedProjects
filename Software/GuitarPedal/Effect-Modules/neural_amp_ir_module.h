#pragma once
#ifndef NEURAL_AMP_IR_MODULE_H
#define NEURAL_AMP_IR_MODULE_H

#include <RTNeural/RTNeural.h>
#include <stdint.h>

#include "ImpulseResponse/ImpulseResponse.h"
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

  // Neural Network Model
  // Currently only using snapshot models, they tend to sound better and
  //   we can use input level as gain.
  RTNeural::ModelT<float, 1, 1, RTNeural::GRULayerT<float, 1, 9>,
                   RTNeural::DenseT<float, 9, 1>>
      m_model;

  int m_currentModelIndex = -1;
  float m_nnLevelAdjust = 0;

  // Impulse Response
  ImpulseResponse m_IR;
  int m_currentIRIndex = -1;

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
