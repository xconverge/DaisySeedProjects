#include "pitch_shifter_module.h"

#include "../Util/pitch_shifter.h"

using namespace bkshepherd;

using namespace daisy;
using namespace daisysp;

static const char* s_semitoneBinNames[5] = {"0", "1", "2", "3", "4"};
static const char* s_directionBinNames[2] = {"DOWN", "UP"};
static const char* s_modeBinNames[2] = {"LATCH", "MOMENT"};

static const int s_paramCount = 5;
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
      defaultValue : 127,
      knobMapping : 1,
      midiCCMapping : -1
    },
    {
      name : "Direction",
      valueType : ParameterValueType::Binned,
      valueBinCount : 2,
      valueBinNames : s_directionBinNames,
      defaultValue : 0,
      knobMapping : 2,
      midiCCMapping : -1
    },
    {
      name : "Mode",
      valueType : ParameterValueType::Binned,
      valueBinCount : 2,
      valueBinNames : s_modeBinNames,
      defaultValue : 0,
      knobMapping : 3,
      midiCCMapping : -1
    },
    {
      name : "Time",
      valueType : ParameterValueType::FloatMagnitude,
      valueBinCount : 0,
      defaultValue : 127,
      knobMapping : 4,
      midiCCMapping : -1
    },
};

// Fixed loud noise at startup (First time after power cycle) by NOT
// putting this in DSY_SDRAM_BSS
static daisysp_modified::PitchShifter pitchShifter;

static CrossFade pitchCrossfade;

// Default Constructor
PitchShifterModule::PitchShifterModule() : BaseEffectModule() {
  m_name = "Pitch";

  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(s_paramCount);
}

// Destructor
PitchShifterModule::~PitchShifterModule() {}

void PitchShifterModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  pitchShifter.Init(sample_rate);

  pitchCrossfade.Init(CROSSFADE_CPOW);
  pitchCrossfade.SetPos(GetParameterAsMagnitude(1));

  m_latching = GetParameterAsBinnedValue(3) == 1;

  m_directionDown = GetParameterAsBinnedValue(2) == 1;
  if (m_directionDown) {
    m_semitoneTarget = (GetParameterAsBinnedValue(0) - 1) * -1.0f;
  } else {
    m_semitoneTarget = (GetParameterAsBinnedValue(0) - 1);
  }
  pitchShifter.SetTransposition(m_semitoneTarget);
}

void PitchShifterModule::ParameterChanged(int parameter_id) {
  if (parameter_id == 0 || parameter_id == 2) {
    // Change semitone when semitone knob is turned or direction is changed
    m_directionDown = GetParameterAsBinnedValue(2) == 1;
    if (m_directionDown) {
      m_semitoneTarget = (GetParameterAsBinnedValue(0) - 1) * -1.0f;
    } else {
      m_semitoneTarget = (GetParameterAsBinnedValue(0) - 1);
    }
    pitchShifter.SetTransposition(m_semitoneTarget);
  } else if (parameter_id == 1) {
    pitchCrossfade.SetPos(GetParameterAsMagnitude(1));
  } else if (parameter_id == 3) {
    m_latching = GetParameterAsBinnedValue(3) == 1;
  }
}

void PitchShifterModule::AlternateFootswitchPressed() {
  m_alternateFootswitchPressed = true;
}

void PitchShifterModule::AlternateFootswitchReleased() {
  m_alternateFootswitchPressed = false;
}

void PitchShifterModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);
  float out = in;
  if (m_latching) {
    float shifted = pitchShifter.Process(in);
    out = pitchCrossfade.Process(in, shifted);
  } else {
    if (m_alternateFootswitchPressed) {
      float shifted = pitchShifter.Process(in);
      out = pitchCrossfade.Process(in, shifted);
    }
  }
  m_audioRight = m_audioLeft = out;
}

void PitchShifterModule::ProcessStereo(float inL, float inR) {
  ProcessMono(inL);
}