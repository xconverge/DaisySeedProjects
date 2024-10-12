#include "tuner_module.h"

#include "../Util/frequency_detector_cmsis.h"
#include "../Util/frequency_detector_q.h"
#include "../Util/frequency_detector_yin.h"

using namespace bkshepherd;

using namespace daisy;
using namespace daisysp;

const char k_notes[12][3] = {"C",  "C#", "D",  "D#", "E",  "F",
                             "F#", "G",  "G#", "A",  "A#", "B"};

const uint16_t k_screenWidth = 128;

static const int s_paramCount = 0;
static const ParameterMetaData s_metaData[s_paramCount] = {};

// Default Constructor
TunerModule::TunerModule(TunerVariant variant) : BaseEffectModule() {
  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(0);

  switch (variant) {
    case TunerVariant::Q:
      m_name = "Tuner [Q]";
      m_frequencyDetector = new FrequencyDetectorQ();
      break;
    case TunerVariant::CMSIS_FFT:
      m_name = "Tuner [CMSIS]";
      m_frequencyDetector = new FrequencyDetectorCMSIS();
      break;
    case TunerVariant::YIN:
      m_name = "Tuner [YIN]";
      m_frequencyDetector = new FrequencyDetectorYin();
      break;
  }
}

// Destructor
TunerModule::~TunerModule() {
  delete m_frequencyDetector;
  m_frequencyDetector = nullptr;
}

void TunerModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  m_frequencyDetector->Init(sample_rate);
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

  // Run the detector
  m_frequencyDetector->Process(in);

  // Try to get the latest frequency from the detector
  m_currentFrequency = round(m_frequencyDetector->GetFrequency());

  m_audioRight = m_audioLeft = in;
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

  display.WriteStringAligned(strbuff, Font_11x18, boundsToDrawIn,
                             Alignment::centered, true);

  char strbuffFreq[64];
  sprintf(strbuffFreq, "%.2f", m_currentFrequency);
  display.WriteStringAligned(strbuffFreq, Font_11x18, boundsToDrawIn,
                             Alignment::bottomCentered, true);
}