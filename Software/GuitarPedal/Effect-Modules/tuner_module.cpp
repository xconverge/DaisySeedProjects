#include "tuner_module.h"

#include "q/fx/lowpass.hpp"

using namespace bkshepherd;

using namespace daisy;
using namespace daisysp;

const char k_notes[12][3] = {"C",  "C#", "D",  "D#", "E",  "F",
                             "F#", "G",  "G#", "A",  "A#", "B"};

const uint16_t k_screenWidth = 128;

static const int s_paramCount = 0;
static const ParameterMetaData s_metaData[s_paramCount] = {};

cycfi::q::dynamic_smoother smoothingFilter{0.3, 48000};
cycfi::q::dynamic_smoother smoothingFilter2{0.3, 48000};

// Default Constructor
TunerModule::TunerModule() : BaseEffectModule() {
  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(s_paramCount);

  m_name = "TuneQ";
}

// Destructor
TunerModule::~TunerModule() {}

void TunerModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  m_frequencyDetector.Init(sample_rate);
}

float Pitch(uint8_t note) { return 440.0f * pow(2.0f, (note - 'E') / 12.0f); }

float Cents(float frequency, uint8_t note) {
  return 1200.0f * log(frequency / Pitch(note)) / log(2.0f);
}

uint8_t Note(float frequency) {
  return round(12.0f * (log(frequency / 440.0f) / log(2.0f))) + 'E';
}

uint8_t Octave(float frequency) { return Note(frequency) / 12.0f - 1.0f; }

void TunerModule::ProcessMono(float in) {
  // Run the detector
  m_frequencyDetector.Process(in);

  // Try to get the latest frequency from the detector
  m_currentFrequency = smoothingFilter(m_frequencyDetector.GetFrequency());

  m_note = Note(m_currentFrequency);
  m_octave = Octave(m_currentFrequency);

  m_cents = Cents(m_currentFrequency, m_note);
}

void TunerModule::ProcessStereo(float inL, float inR) { ProcessMono(inL); }

void TunerModule::DrawUI(OneBitGraphicsDisplay& display, int currentIndex,
                         int numItemsTotal, Rectangle boundsToDrawIn,
                         bool isEditing) {
  BaseEffectModule::DrawUI(display, currentIndex, numItemsTotal, boundsToDrawIn,
                           isEditing);
  uint16_t center = boundsToDrawIn.GetHeight() / 2;

  char currentNote[12];
  sprintf(currentNote, "%s%u", k_notes[m_note % 12], m_octave);

  const float close = 1.0f;
  const float medium = 3.0f;
  const float far = 10.0f;

  char strbuff[64];

  if (m_cents < -far) {
    sprintf(strbuff, "ooo %s    ", currentNote);
  } else if (m_cents < -medium) {
    sprintf(strbuff, " oo %s    ", currentNote);
  } else if (m_cents < -close) {
    sprintf(strbuff, "  o %s    ", currentNote);
  } else if (m_cents > far) {
    sprintf(strbuff, "    %s ooo", currentNote);
  } else if (m_cents > medium) {
    sprintf(strbuff, "    %s oo ", currentNote);
  } else if (m_cents > close) {
    sprintf(strbuff, "    %s o  ", currentNote);
  } else {
    sprintf(strbuff, "   [%s]   ", currentNote);
  }

  display.WriteStringAligned(strbuff, Font_11x18, boundsToDrawIn,
                             Alignment::centered, true);

  char strbuffCents[64];
  sprintf(strbuffCents, "%.2f", m_cents);
  display.WriteStringAligned(strbuffCents, Font_11x18, boundsToDrawIn,
                             Alignment::topCentered, true);
  char strbuffFreq[64];
  sprintf(strbuffFreq, "%.2f", m_currentFrequency);
  display.WriteStringAligned(strbuffFreq, Font_11x18, boundsToDrawIn,
                             Alignment::bottomCentered, true);
}