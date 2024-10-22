#include "pitch_shifter_module.h"

#include "../Util/pitch_shifter.h"

using namespace bkshepherd;

using namespace daisy;
using namespace daisysp;

static const char* s_semitoneBinNames[5] = {"0", "1", "2", "3", "4"};
static const int s_paramCount = 2;
static const ParameterMetaData s_metaData[s_paramCount] = {
    {
      name : "Semitone",
      valueType : ParameterValueType::Binned,
      valueBinCount : 5,
      valueBinNames : s_semitoneBinNames,
      defaultValue : (127 / 5) * 1,
      knobMapping : 0,
      midiCCMapping : -1
    },
    {
      name : "Crossfade",
      valueType : ParameterValueType::FloatMagnitude,
      valueBinCount : 0,
      defaultValue : 64,
      knobMapping : 1,
      midiCCMapping : -1
    },
};

// TODO SK: Fixed loud noise at startup (First time after power cycle) by NOT
// putting this in DSY_SDRAM_BSS
static daisysp_modified::PitchShifter pitchShifter;

static CrossFade pitchCrossfade;

// Default Constructor
PitchShifterModule::PitchShifterModule() : BaseEffectModule() {
  m_name = "Drop";

  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(s_paramCount);

  pitchCrossfade.SetPos(GetParameterAsMagnitude(1));
}

// Destructor
PitchShifterModule::~PitchShifterModule() {}

void PitchShifterModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  pitchShifter.Init(sample_rate, true);

  const int semitone = (GetParameterAsBinnedValue(0) - 1) * -1;
  pitchShifter.SetTransposition((float)semitone);

  pitchCrossfade.Init(CROSSFADE_CPOW);
}

void PitchShifterModule::ParameterChanged(int parameter_id) {
  if (parameter_id == 0) {
    // Change semitone when knob is turned
    const int semitone = (GetParameterAsBinnedValue(0) - 1) * -1;
    pitchShifter.SetTransposition((float)semitone);
  } else if (parameter_id == 1) {
    pitchCrossfade.SetPos(GetParameterAsMagnitude(1));
  }
}

void PitchShifterModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  float shifted = pitchShifter.Process(in);

  const float out = pitchCrossfade.Process(in, shifted);

  m_audioRight = m_audioLeft = out;
}

void PitchShifterModule::ProcessStereo(float inL, float inR) {
  ProcessMono(inL);
}