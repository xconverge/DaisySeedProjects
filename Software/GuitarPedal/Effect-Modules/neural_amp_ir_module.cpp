#include "neural_amp_ir_module.h"

using namespace bkshepherd;

static const int s_paramCount = 2;
static const ParameterMetaData s_metaData[s_paramCount] = {
    {
      name : "Drive",
      valueType : ParameterValueType::FloatMagnitude,
      valueBinCount : 0,
      defaultValue : 57,
      knobMapping : 1,
      midiCCMapping : 1
    },
    {
      name : "Level",
      valueType : ParameterValueType::FloatMagnitude,
      valueBinCount : 0,
      defaultValue : 40,
      knobMapping : 0,
      midiCCMapping : 21
    }};

// Default Constructor
NeuralAmpIRModule::NeuralAmpIRModule() : BaseEffectModule() {
  // Set the name of the effect
  m_name = "Neural";

  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(s_paramCount);

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
  m_IR.Init(ir_collection[irIndex]);  // ir_data is from ir_data.h
}

// Destructor
NeuralAmpIRModule::~NeuralAmpIRModule() {
  // No Code Needed
}

void NeuralAmpIRModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);
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

  // Process Tone

  // Impulse Response
  float impulse_out = m_IR.Process(ampOut) * 0.2;

  m_audioLeft = impulse_out;
  m_audioRight = impulse_out;
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