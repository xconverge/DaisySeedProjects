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
  void ProcessMono(float in) override;
  void ProcessStereo(float inL, float inR) override;
  float GetBrightnessForLED(int led_id) override;

 private:
  void initializeModel();
  void initializeIR();

  // Impulse Response
  ImpulseResponse m_IR;
  int m_currentIRIndex = 0;
  int m_desiredIRIndex = 0;

  int m_irEnabled = 0;

  // Neural Network Model
  // Currently only using snapshot models, they tend to sound better and
  //   we can use input level as gain.

  RTNeural::ModelT<float, 1, 1, RTNeural::GRULayerT<float, 1, 9>,
                   RTNeural::DenseT<float, 9, 1>>
      m_model;

  unsigned int m_currentModelIndex = 0;
  unsigned int m_desiredModelIndex = 0;

  float m_nnLevelAdjust = 1.0;

  float m_driveMin;
  float m_driveMax;
  float m_levelMin;
  float m_levelMax;
  float m_cutoffMin;
  float m_cutoffMax;

  Tone m_tone;  // Low Pass
};
}  // namespace bkshepherd
#endif
#endif
