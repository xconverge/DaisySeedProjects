#include "neural_amp_ir_module.h"

// Model Weights (edit this file to add model weights trained with Colab script)
//    The models must be GRU (gated recurrent unit) with hidden size = 9,
//    snapshot models (not condidtioned on a parameter)
#include "ImpulseResponse/ir_data.h"
#include "NeuralModel/all_model_data_gru9_4count.h"

using namespace bkshepherd;

static const char* s_irEnabledBinNames[2] = {"OFF", "ON"};
static const char* s_modelBinNames[8] = {"1", "2", "3", "4",
                                         "5", "6", "7", "8"};
static const char* s_IRBinNames[3] = {"IR1", "IR2", "IR3"};

static const int s_paramCount = 6;
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
    {
      name : "IR Enabled",
      valueType : ParameterValueType::Binned,
      valueBinCount : 2,
      valueBinNames : s_irEnabledBinNames,
      defaultValue : 0,
      knobMapping : 3,
      midiCCMapping : -1
    },
    {
      name : "Model",
      valueType : ParameterValueType::Binned,
      valueBinCount : 8,
      valueBinNames : s_modelBinNames,
      defaultValue : 0,
      knobMapping : 4,
      midiCCMapping : -1
    },
    {
      name : "IR #",
      valueType : ParameterValueType::Binned,
      valueBinCount : 3,
      valueBinNames : s_IRBinNames,
      defaultValue : 0,
      knobMapping : 5,
      midiCCMapping : -1
    },
};

// Default Constructor
NeuralAmpIRModule::NeuralAmpIRModule()
    : BaseEffectModule(),
      m_driveMin(0.0f),
      m_driveMax(1.0f),
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

void NeuralAmpIRModule::initializeModel() {
  m_currentModelIndex = m_desiredModelIndex;

  // Setup model
  auto& gru = (m_model).template get<0>();
  auto& dense = (m_model).template get<1>();
  gru.setWVals(model_collection[m_currentModelIndex].rec_weight_ih_l0);
  gru.setUVals(model_collection[m_currentModelIndex].rec_weight_hh_l0);
  gru.setBVals(model_collection[m_currentModelIndex].rec_bias);
  dense.setWeights(model_collection[m_currentModelIndex].lin_weight);
  dense.setBias(model_collection[m_currentModelIndex].lin_bias.data());
  m_model.reset();
  m_nnLevelAdjust = model_collection[m_currentModelIndex].levelAdjust;
}

void NeuralAmpIRModule::initializeIR() {
  m_currentIRIndex = m_desiredIRIndex;

  m_IR.Init(ir_collection[m_currentIRIndex]);
}

void NeuralAmpIRModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  m_tone.Init(sample_rate);

  setupWeights();

  m_desiredModelIndex = 0;
  initializeModel();

  m_desiredIRIndex = 0;
  initializeIR();
}

void NeuralAmpIRModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  m_irEnabled = GetParameterAsBinnedValue(3) - 1;

  m_desiredModelIndex = GetParameterAsBinnedValue(4) - 1;
  if (m_desiredModelIndex != m_currentModelIndex) {
    initializeModel();
  }

  m_desiredIRIndex = GetParameterAsBinnedValue(5) - 1;
  if (m_desiredIRIndex != m_currentIRIndex) {
    initializeIR();
  }

  // Neural Net Input
  float input_arr[1] = {0.0};

  // Apply gain
  float drive =
      m_driveMin + (GetParameterAsMagnitude(2) * (m_driveMax - m_driveMin));
  input_arr[0] = in * drive;

  // Process Neural Net Model
  float ampOut = 0.0;

  ampOut = m_model.forward(input_arr) + input_arr[0];
  ampOut *= m_nnLevelAdjust;

  float level =
      m_levelMin + (GetParameterAsMagnitude(0) * (m_levelMax - m_levelMin));
  float cutoff =
      m_cutoffMin + GetParameterAsMagnitude(1) * (m_cutoffMax - m_cutoffMin);

  // Process Tone
  m_tone.SetFreq(cutoff);

  // Impulse Response
  float impulse_out = 0.0;
  if (m_irEnabled == 1) {
    impulse_out = m_IR.Process(ampOut) * 0.2;
  } else {
    impulse_out = ampOut;
  }

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