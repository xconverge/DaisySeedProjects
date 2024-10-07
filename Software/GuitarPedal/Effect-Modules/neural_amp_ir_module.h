#pragma once
#ifndef NEURAL_AMP_IR_MODULE_H
#define NEURAL_AMP_IR_MODULE_H

#include <RTNeural/RTNeural.h>
#include <stdint.h>

#include "ImpulseResponse/ImpulseResponse.h"
#include "base_effect_module.h"
#include "daisysp.h"

// Model Weights (edit this file to add model weights trained with Colab script)
//    The models must be GRU (gated recurrent unit) with hidden size = 9,
//    snapshot models (not condidtioned on a parameter)
#include "ImpulseResponse/ir_data.h"
#include "NeuralModel/all_model_data_gru9_4count.h"

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
  void UpdateModelIndex();
  void UpdateIRIndex();

  // Impulse Response
  ImpulseResponse m_IR;
  int m_currentIRindex;

  // Neural Network Model
  // Currently only using snapshot models, they tend to sound better and
  //   we can use input level as gain.

  RTNeural::ModelT<float, 1, 1, RTNeural::GRULayerT<float, 1, 9>,
                   RTNeural::DenseT<float, 9, 1>>
      m_model;

  int m_modelInSize;
  unsigned int m_modelIndex;
  float m_nnLevelAdjust;
  int m_indexMod;
};
}  // namespace bkshepherd
#endif
#endif
