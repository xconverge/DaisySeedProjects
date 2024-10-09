#include "looper_module.h"

#include "../Util/audio_utilities.h"

using namespace bkshepherd;

static const ParameterMetaData s_metaData[0] = {};

// Default Constructor
LooperModule::LooperModule() : BaseEffectModule() {
  // Set the name of the effect
  m_name = "Looper";

  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(0);
}

// Destructor
LooperModule::~LooperModule() {
  // No Code Needed
}

void LooperModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);
}

void LooperModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  m_audioRight = m_audioLeft = in;
}

void LooperModule::ProcessStereo(float inL, float inR) {
  BaseEffectModule::ProcessStereo(inL, inR);

  m_audioLeft = inL;
  m_audioRight = inR;
}