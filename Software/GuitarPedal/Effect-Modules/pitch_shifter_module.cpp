#include "pitch_shifter_module.h"

#include "../Util/pitch_shifter.h"

using namespace bkshepherd;

using namespace daisy;
using namespace daisysp;

static const char* s_semitoneBinNames[7] = {"1", "2", "3", "4", "5", "6", "7"};
static const int s_paramCount = 1;
static const ParameterMetaData s_metaData[s_paramCount] = {
    {
      name : "Semitone",
      valueType : ParameterValueType::Binned,
      valueBinCount : 7,
      valueBinNames : s_semitoneBinNames,
      defaultValue : 0,
      knobMapping : 0,
      midiCCMapping : -1
    },
};

static daisysp_modified::PitchShifter DSY_SDRAM_BSS pitchShifter;
static CrossFade pitchCrossfade;

// Default Constructor
PitchShifterModule::PitchShifterModule() : BaseEffectModule() {
  m_name = "Drop";

  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(s_paramCount);
}

// Destructor
PitchShifterModule::~PitchShifterModule() {}

void PitchShifterModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  pitchShifter.Init(sample_rate, true);

  const int semitone = GetParameterAsBinnedValue(0);
  pitchShifter.SetTransposition((float)semitone);

  pitchCrossfade.Init(CROSSFADE_CPOW);

  // TODO SK: Fix loud noise at startup
}

void PitchShifterModule::ParameterChanged(int parameter_id) {
  if (parameter_id == 0) {
    // Change semitone when knob is turned
    const int semitone = (GetParameterAsBinnedValue(0) - 1) * -1;
    pitchShifter.SetTransposition((float)semitone);
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