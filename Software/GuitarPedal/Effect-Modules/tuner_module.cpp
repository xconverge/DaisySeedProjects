#include "tuner_module.h"

#include "../Util/audio_utilities.h"
#include "../Util/fourier.h"
#include "../Util/shy_fft.h"
#include "../Util/wave.h"

using namespace soundmath;
using namespace bkshepherd;

const char k_notes[12][3] = {"C",  "C#", "D",  "D#", "E",  "F",
                             "F#", "G",  "G#", "A",  "A#", "B"};

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

  m_cachedSampleRate = sample_rate;
}

float Pitch(uint8_t note) { return 440.0f * pow(2.0f, (note - 'E') / 12.0f); }

int16_t Cents(float frequency, uint8_t note) {
  return 1200.0f * log(frequency / Pitch(note)) / log(2.0f);
}

uint8_t Note(float frequency) {
  return round(12.0f * (log(frequency / 440.0f) / log(2.0f))) + 'E';
}

uint8_t Octave(float frequency) { return Note(frequency) / 12.0f - 1.0f; }

void TunerModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  const float frequency = 0;

  if (frequency && round(m_currentFrequency) != frequency) {
    m_currentFrequency = frequency;
  }

  m_audioRight = m_audioLeft = in;
}

void TunerModule::ProcessStereo(float inL, float inR) { ProcessMono(inL); }

void TunerModule::DrawUI(OneBitGraphicsDisplay &display, int currentIndex,
                         int numItemsTotal, Rectangle boundsToDrawIn,
                         bool isEditing) {
  BaseEffectModule::DrawUI(display, currentIndex, numItemsTotal, boundsToDrawIn,
                           isEditing);
  uint16_t center = boundsToDrawIn.GetHeight() / 2;

  int16_t cents = Cents(m_currentFrequency, Note(m_currentFrequency));

  char currentNote[12];
  sprintf(currentNote, "%s%u", k_notes[Note(m_currentFrequency) % 12],
          Octave(m_currentFrequency));

  char strbuff[64];
  if (cents < -1) {
    if (cents < -10) {
      sprintf(strbuff, "o o %s    ", currentNote);
    } else {
      sprintf(strbuff, "  o %s    ", currentNote);
    }
  } else if (cents > 1) {
    if (cents > 10) {
      sprintf(strbuff, "    %s o o", currentNote);
    } else {
      sprintf(strbuff, "    %s o  ", currentNote);
    }
  } else {
    sprintf(strbuff, "    %s    ", currentNote);
  }

  boundsToDrawIn.RemoveFromTop(center);
  display.WriteStringAligned(strbuff, Font_11x18, boundsToDrawIn,
                             Alignment::centered, true);
}