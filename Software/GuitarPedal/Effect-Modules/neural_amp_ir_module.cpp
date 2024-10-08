#include "neural_amp_ir_module.h"

// Model Weights (edit this file to add model weights trained with Colab script)
//    The models must be GRU (gated recurrent unit) with hidden size = 9,
//    snapshot models (not condidtioned on a parameter)
#include "ImpulseResponse/ir_data.h"
#include "NeuralModel/all_model_data_gru9_4count.h"

using namespace bkshepherd;

static const char* s_IRBinNames[10] = {
    "Rhythm",  "Lead",    "Clean",   "Marsh",     "Bogn",
    "Proteus", "Rectify", "Rhythm2", "US Deluxe", "British"};

static const char* s_modelBinNames[8] = {
    "Fender57",  "Matchless", "Klon", "Mesa iic",
    "H&K Clean", "Bassman",   "5150", "Splawn",
};

static const int s_paramCount = 6;
static const ParameterMetaData s_metaData[s_paramCount] = {
    {
      name : "Level",
      valueType : ParameterValueType::FloatMagnitude,
      valueBinCount : 0,
      defaultValue : 64,
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
      name : "Gain",
      valueType : ParameterValueType::FloatMagnitude,
      valueBinCount : 0,
      defaultValue : 64,
      knobMapping : 2,
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
      valueBinCount : 10,
      valueBinNames : s_IRBinNames,
      defaultValue : 0,
      knobMapping : 5,
      midiCCMapping : -1
    },
    {
      name : "IR Enabled",
      valueType : ParameterValueType::Bool,
      valueBinCount : 0,
      defaultValue : 127,
      knobMapping : 3,
      midiCCMapping : -1
    },
};

// Default Constructor
NeuralAmpIRModule::NeuralAmpIRModule()
    : BaseEffectModule(),
      m_gainMin(0.0f),
      m_gainMax(2.0f),
      m_levelMin(2.0f),
      m_levelMax(1.0f),
      m_cutoffMin(500),
      m_cutoffMax(20000) {
  // Set the name of the effect
  m_name = "Amp";

  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(s_paramCount);
}

// Destructor
NeuralAmpIRModule::~NeuralAmpIRModule() {
  // No Code Needed
}

void NeuralAmpIRModule::SelectModel() {
  const int modelIndex = GetParameterAsBinnedValue(3) - 1;

  if (m_currentModelIndex != modelIndex) {
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
  m_currentModelIndex = modelIndex;
}

void NeuralAmpIRModule::SelectIR() {
  const int irIndex = GetParameterAsBinnedValue(4) - 1;
  if (irIndex != m_currentIRIndex) {
    m_IR.Init(ir_collection[m_currentIRIndex]);
  }
  m_currentIRIndex = irIndex;
}

void NeuralAmpIRModule::CalculateTone() {
  const float cutoff =
      m_cutoffMin + GetParameterAsMagnitude(1) * (m_cutoffMax - m_cutoffMin);
  m_tone.SetFreq(cutoff);
}

void NeuralAmpIRModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  setupWeights();

  SelectModel();
  SelectIR();

  m_tone.Init(sample_rate);
  CalculateTone();
}

void NeuralAmpIRModule::ParameterChanged(int parameter_id) {
  if (parameter_id == 4) {  // Change Model
    SelectModel();
  } else if (parameter_id == 1) {
    CalculateTone();
  } else if (parameter_id == 5) {  // Change IR
    SelectIR();
  }
}

void NeuralAmpIRModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  // Neural Net Input
  float input_arr[1] = {0.0};

  // Apply gain
  float gain =
      m_gainMin + (GetParameterAsMagnitude(2) * (m_gainMax - m_gainMin));
  input_arr[0] = m_audioLeft * gain;

  // Process Neural Net Model
  float ampOut = 0.0;
  if (true) {
    ampOut = m_model.forward(input_arr) + input_arr[0];

    // Level adjustment
    ampOut *= m_nnLevelAdjust * 0.5;
  } else {
    ampOut = input_arr[0];
  }

  // Process Tone
  const float filter_out = m_tone.Process(ampOut);

  // Impulse Response
  float impulse_out = 0.0;
  if (GetParameterAsBool(5)) {
    // 0.2 is level adjust for loud output
    impulse_out = m_IR.Process(filter_out) * 0.2;
  } else {
    impulse_out = filter_out;
  }

  float level =
      m_levelMin + (GetParameterAsMagnitude(0) * (m_levelMax - m_levelMin));

  m_audioLeft = impulse_out * level;
  m_audioRight = m_audioLeft;
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