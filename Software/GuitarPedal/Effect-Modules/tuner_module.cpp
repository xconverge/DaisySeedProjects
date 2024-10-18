#include "tuner_module.h"

using namespace bkshepherd;

using namespace daisy;
using namespace daisysp;

const char k_notes[12][3] = {"C",  "C#", "D",  "D#", "E",  "F",
                             "F#", "G",  "G#", "A",  "A#", "B"};

const uint16_t k_screenWidth = 128;

static const int s_paramCount = 0;
static const ParameterMetaData s_metaData[s_paramCount] = {};

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

int16_t Cents(float frequency, uint8_t note) {
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
  m_currentFrequency = m_frequencyDetector.GetFrequency();
}

void TunerModule::ProcessStereo(float inL, float inR) { ProcessMono(inL); }

void TunerModule::DrawUI(OneBitGraphicsDisplay& display, int currentIndex,
                         int numItemsTotal, Rectangle boundsToDrawIn,
                         bool isEditing) {
  BaseEffectModule::DrawUI(display, currentIndex, numItemsTotal, boundsToDrawIn,
                           isEditing);
  uint16_t center = boundsToDrawIn.GetHeight() / 2;

  int16_t cents = Cents(m_currentFrequency, Note(m_currentFrequency));

  char currentNote[12];
  sprintf(currentNote, "%s%u", k_notes[Note(m_currentFrequency) % 12],
          Octave(m_currentFrequency));

  const int8_t close = 1;
  const int8_t medium = 3;
  const int8_t far = 10;

  char strbuff[64];

  if (cents < -far) {
    sprintf(strbuff, "ooo %s    ", currentNote);
  } else if (cents < -medium) {
    sprintf(strbuff, " oo %s    ", currentNote);
  } else if (cents < -close) {
    sprintf(strbuff, "  o %s    ", currentNote);
  } else if (cents > far) {
    sprintf(strbuff, "    %s ooo", currentNote);
  } else if (cents > medium) {
    sprintf(strbuff, "    %s oo ", currentNote);
  } else if (cents > close) {
    sprintf(strbuff, "    %s o  ", currentNote);
  } else {
    sprintf(strbuff, "   [%s]   ", currentNote);
  }

  display.WriteStringAligned(strbuff, Font_11x18, boundsToDrawIn,
                             Alignment::centered, true);

  char strbuffFreq[64];
  sprintf(strbuffFreq, "%.2f", m_currentFrequency);
  display.WriteStringAligned(strbuffFreq, Font_11x18, boundsToDrawIn,
                             Alignment::bottomCentered, true);
}