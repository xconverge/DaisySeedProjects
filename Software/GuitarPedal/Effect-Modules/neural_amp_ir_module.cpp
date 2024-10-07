#include "neural_amp_ir_module.h"

// Model Weights (edit this file to add model weights trained with Colab script)
//    The models must be GRU (gated recurrent unit) with hidden size = 9,
//    snapshot models (not condidtioned on a parameter)
#include "ImpulseResponse/ir_data.h"
#include "NeuralModel/all_model_data_gru9_4count.h"

using namespace bkshepherd;

static const int s_paramCount = 3;
static const ParameterMetaData s_metaData[s_paramCount] = {
    {
      name : "Level",
      valueType : ParameterValueType::FloatMagnitude,
      valueBinCount : 0,
      defaultValue : 40,
      knobMapping : 0,
      midiCCMapping : -1
    },
    {
      name : "Cutoff",
      valueType : ParameterValueType::FloatMagnitude,
      valueBinCount : 0,
      defaultValue : 64,
      knobMapping : 1,
      midiCCMapping : -1
    },
    {
      name : "Drive",
      valueType : ParameterValueType::FloatMagnitude,
      valueBinCount : 0,
      defaultValue : 57,
      knobMapping : 2,
      midiCCMapping : -1
    },
};

// Default Constructor
NeuralAmpIRModule::NeuralAmpIRModule()
    : BaseEffectModule(),
      m_driveMin(0.4f),
      m_driveMax(0.8f),
      m_levelMin(0.01f),
      m_levelMax(0.20f),
      m_cutoffMin(500),
      m_cutoffMax(20000) {
  // Set the name of the effect
  m_name = "Neural";

  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(s_paramCount);
}

// Destructor
NeuralAmpIRModule::~NeuralAmpIRModule() {
  // No Code Needed
}

void NeuralAmpIRModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  m_tone.Init(sample_rate);

  setupWeights();

  int modelIndex = 0;

  // Setup model
  auto& gru = (m_model).template get<0>();
  auto& dense = (m_model).template get<1>();
  m_modelInSize = 1;
  gru.setWVals(model_collection[modelIndex].rec_weight_ih_l0);
  gru.setUVals(model_collection[modelIndex].rec_weight_hh_l0);
  gru.setBVals(model_collection[modelIndex].rec_bias);
  dense.setWeights(model_collection[modelIndex].lin_weight);
  dense.setBias(model_collection[modelIndex].lin_bias.data());
  m_model.reset();

  m_nnLevelAdjust = model_collection[modelIndex].levelAdjust;

  // Setup IR
  int irIndex = 0;
  m_IR.Init(ir_collection[irIndex]);
}

void NeuralAmpIRModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  // Neural Net Input
  float input_arr[1] = {0.0};

  input_arr[0] = in;

  // Process Neural Net Model
  float ampOut = 0.0;
  ampOut = m_model.forward(input_arr) + input_arr[0];
  ampOut *= m_nnLevelAdjust;

  float level =
      m_levelMin + (GetParameterAsMagnitude(0) * (m_levelMax - m_levelMin));
  float cutoff =
      m_cutoffMin + GetParameterAsMagnitude(1) * (m_cutoffMax - m_cutoffMin);
  // float drive =
  //     m_driveMin + (GetParameterAsMagnitude(2) * (m_driveMax - m_driveMin));

  // Process Tone
  m_tone.SetFreq(cutoff);

  // Impulse Response
  float impulse_out = m_IR.Process(ampOut) * 0.2;

  m_audioLeft = impulse_out * level;
  m_audioRight = impulse_out * level;
}

void NeuralAmpIRModule::ProcessStereo(float inL, float inR) {
  // Calculate the mono effect
  ProcessMono(inL);
}

float NeuralAmpIRModule::GetBrightnessForLED(int led_id) {
  float value = BaseEffectModule::GetBrightnessForLED(led_id);

  if (led_id == 1) {
    return value * GetParameterAsMagnitude(0);
  }

  return value;
}