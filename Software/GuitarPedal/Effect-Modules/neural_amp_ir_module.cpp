#include "neural_amp_ir_module.h"

#include <RTNeural/RTNeural.h>

#include "ImpulseResponse/ImpulseResponse.h"
#include "ImpulseResponse/ir_data.h"

// Model Weights (edit this file to add model weights trained with Colab script)
//    The models must be GRU (gated recurrent unit) with hidden size = 9,
//    snapshot models (not conditioned on a parameter)
#include "NeuralModel/all_model_data_gru9_4count.h"

using namespace bkshepherd;

static DaisySeed hw;

static const char *s_IRBinNames[10] = {"Rhythm",  "Lead",    "Clean",   "Marsh",     "Bogn",
                                       "Proteus", "Rectify", "Rhythm2", "US Deluxe", "British"};

static const char *s_modelBinNames[8] = {
    "Fender57", "Matchless", "Klon", "Mesa iic", "H&K Clean", "Bassman", "5150", "Splawn",
};

static const int s_paramCount = 7;
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
        defaultValue : (127 / 8) * 4,
        knobMapping : 3,
        midiCCMapping : -1
    },
    {
        name : "IR",
        valueType : ParameterValueType::Binned,
        valueBinCount : 10,
        valueBinNames : s_IRBinNames,
        defaultValue : (127 / 10) * 7,
        knobMapping : 4,
        midiCCMapping : -1
    },
    {name : "IR Enabled", valueType : ParameterValueType::Bool, defaultValue : 0, knobMapping : 5, midiCCMapping : -1},
    {
        name : "Amp Enabled",
        valueType : ParameterValueType::Bool,
        defaultValue : 127,
        knobMapping : -1,
        midiCCMapping : -1
    },
};

// Neural Network Model
// Currently only using snapshot models, they tend to sound better and
//   we can use input level as gain.
RTNeural::ModelT<float, 1, 1, RTNeural::GRULayerT<float, 1, 9>, RTNeural::DenseT<float, 9, 1>> model;

// clang-format off
// Notes: With default settings, GRU 10 is max size currently able to run on Daisy Seed
//        - Parameterized 1-knob GRU 10 is max, GRU 8 with effects is max
//        - Parameterized 2-knob/3-knob at GRU 8 is max
//        - With multi effect (reverb, etc.) added GRU 9 is recommended to allow room for processing of other effects
//        - These models should be trained using 48kHz audio data, since Daisy uses 48kHz by default.
//             Models trained with other samplerates, or running Daisy at a different samplerate will sound different.
// clang-format on

// Impulse Response
ImpulseResponse IR;

// Default Constructor
NeuralAmpIRModule::NeuralAmpIRModule()
    : BaseEffectModule(), m_gainMin(0.0f), m_gainMax(2.0f), m_levelMin(0.0f), m_levelMax(1.0f), m_cutoffMin(500),
      m_cutoffMax(20000)
{
    // Set the name of the effect
    m_name = "Amp";

    // Setup the meta data reference for this Effect
    m_paramMetaData = s_metaData;

    // Initialize Parameters for this Effect
    this->InitParams(s_paramCount);
}

// Destructor
NeuralAmpIRModule::~NeuralAmpIRModule()
{
    // No Code Needed
}

void NeuralAmpIRModule::Init(float sample_rate)
{
    BaseEffectModule::Init(sample_rate);

    // in the model data .h file
    setupWeights();

    SelectModel();
    SelectIR();

    m_irEnabled = GetParameterAsBool(5);
    m_ampEnabled = GetParameterAsBool(6);

    m_tone.Init(sample_rate);
    CalculateTone();
}

void NeuralAmpIRModule::ParameterChanged(int parameter_id)
{
    if (parameter_id == 3)
    { // Change Model
        SelectModel();
    }
    else if (parameter_id == 1)
    {
        CalculateTone();
    }
    else if (parameter_id == 4)
    { // Change IR
        SelectIR();
    }
    else if (parameter_id == 5)
    { // IR Enabled
        m_irEnabled = GetParameterAsBool(5);
    }
    else if (parameter_id == 6)
    { // Amp Enabled
        m_ampEnabled = GetParameterAsBool(6);
    }
}

void NeuralAmpIRModule::SelectModel()
{
    const int modelIndex = GetParameterAsBinnedValue(3) - 1;
    if (modelIndex != m_currentModelIndex)
    {
        auto &gru = (model).template get<0>();
        auto &dense = (model).template get<1>();
        gru.setWVals(model_collection[modelIndex].rec_weight_ih_l0);
        gru.setUVals(model_collection[modelIndex].rec_weight_hh_l0);
        gru.setBVals(model_collection[modelIndex].rec_bias);
        dense.setWeights(model_collection[modelIndex].lin_weight);
        dense.setBias(model_collection[modelIndex].lin_bias.data());
        model.reset();
        m_nnLevelAdjust = model_collection[modelIndex].levelAdjust;
        m_currentModelIndex = modelIndex;
    }
}

void NeuralAmpIRModule::SelectIR()
{
    const int irIndex = GetParameterAsBinnedValue(4) - 1;
    if (irIndex != m_currentIRIndex)
    {
        IR.Init(ir_collection[irIndex]);
        m_currentIRIndex = irIndex;
    }
}

void NeuralAmpIRModule::CalculateTone()
{
    // Set low pass filter as exponential taper
    const float cutoff =
        m_cutoffMin + GetParameterAsMagnitude(1) * GetParameterAsMagnitude(1) * (m_cutoffMax - m_cutoffMin);
    m_tone.SetFreq(cutoff);
}

void NeuralAmpIRModule::ProcessMono(float in)
{
    BaseEffectModule::ProcessMono(in);

    // Neural Net Input
    float input_arr[1] = {0.0};

    // Apply gain
    const float gain = m_gainMin + (GetParameterAsMagnitude(2) * (m_gainMax - m_gainMin));
    input_arr[0] = in * gain;

    // Process Neural Net Model
    float ampOut = 0.0;
    if (m_ampEnabled)
    {
        ampOut = model.forward(input_arr) + input_arr[0];

        // Level adjustment
        ampOut *= m_nnLevelAdjust;
    }
    else
    {
        ampOut = input_arr[0];
    }

    // Process Tone
    float filter_out = m_tone.Process(ampOut);

    // Impulse Response
    float impulse_out = 0.0;

    if (m_irEnabled)
    {
        // 0.2 is level adjust for loud output
        impulse_out = IR.Process(filter_out) * 0.2;
    }
    else
    {
        impulse_out = filter_out;
    }

    float level = m_levelMin + (GetParameterAsMagnitude(0) * (m_levelMax - m_levelMin));

    m_audioLeft = impulse_out * level;
    m_audioRight = m_audioLeft;
}

void NeuralAmpIRModule::ProcessStereo(float inL, float inR)
{
    // Calculate the mono effect
    ProcessMono(inL);

    // NOTE: Running the Neural Nets in stereo is currently not feasible due to
    // processing limitations, this will remain a MONO ONLY effect for now.
    //       The left channel output is copied to the right output, but the right
    //       input is ignored in this effect module.
}

float NeuralAmpIRModule::GetBrightnessForLED(int led_id)
{
    float value = BaseEffectModule::GetBrightnessForLED(led_id);

    if (led_id == 1)
    {
        return value * GetParameterAsMagnitude(0);
    }

    return value;
}