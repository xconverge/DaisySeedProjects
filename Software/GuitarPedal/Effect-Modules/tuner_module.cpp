#include "tuner_module.h"

#include "../Util/audio_utilities.h"

using namespace bkshepherd;

const uint16_t k_screenWidth = 128;

static const int s_paramCount = 0;
static const ParameterMetaData s_metaData[s_paramCount] = {};

// Default Constructor
TunerModule::TunerModule() : BaseEffectModule() {
  // Set the name of the effect
  m_name = "Tuner";

  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(0);
}

// Destructor
TunerModule::~TunerModule() {
  // No Code Needed
}

void TunerModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);
}

void TunerModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  m_audioRight = m_audioLeft = in;
}

void TunerModule::ProcessStereo(float inL, float inR) { ProcessMono(inL); }

void TunerModule::DrawUI(OneBitGraphicsDisplay& display, int currentIndex,
                         int numItemsTotal, Rectangle boundsToDrawIn,
                         bool isEditing) {
  BaseEffectModule::DrawUI(display, currentIndex, numItemsTotal, boundsToDrawIn,
                           isEditing);
  uint16_t center = boundsToDrawIn.GetHeight() / 2;

  char strbuff[64];
  const char* currentNote = "A";

  const bool veryFlat = false;
  const bool flat = false;
  const bool sharp = false;
  const bool verySharp = false;

  if (veryFlat) {
    sprintf(strbuff, "o o %s    ", currentNote);
  } else if (flat) {
    sprintf(strbuff, "  o %s    ", currentNote);
  } else if (sharp) {
    sprintf(strbuff, "    %s o  ", currentNote);
  } else if (verySharp) {
    sprintf(strbuff, "    %s o o", currentNote);
  } else {
    sprintf(strbuff, "    %s    ", currentNote);
  }

  boundsToDrawIn.RemoveFromTop(center);
  display.WriteStringAligned(strbuff, Font_11x18, boundsToDrawIn,
                             Alignment::centered, true);
}