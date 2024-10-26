#include "tuner_module.h"

#include <q/fx/signal_conditioner.hpp>
#include <q/pitch/pitch_detector.hpp>
#include <q/support/pitch_names.hpp>

#include "q/fx/lowpass.hpp"

using namespace bkshepherd;

using namespace daisy;
using namespace daisysp;
using namespace cycfi::q;

const char k_notes[12][3] = {"C",  "C#", "D",  "D#", "E",  "F",
                             "F#", "G",  "G#", "A",  "A#", "B"};

const uint16_t k_screenWidth = 128;

static const ParameterMetaData s_metaData[0] = {};

dynamic_smoother smoothingFilter{0.3, 48000};

// Default Constructor
TunerModule::TunerModule() : BaseEffectModule() {
  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(0);

  m_name = "TuneQ";
}

// Destructor
TunerModule::~TunerModule() {
  delete m_pitchDetector;
  m_pitchDetector = nullptr;

  delete m_preProcessor;
  m_preProcessor = nullptr;
}

void TunerModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  // The frequency detection bounds;
  frequency lowest_frequency = pitch_names::C[2];
  frequency highest_frequency = pitch_names::C[5];

  m_pitchDetector = new pitch_detector{lowest_frequency, highest_frequency,
                                       sample_rate, lin_to_db(0)};

  signal_conditioner::config preprocessor_config;
  m_preProcessor = new signal_conditioner{preprocessor_config, lowest_frequency,
                                          highest_frequency, sample_rate};
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
  // Pre-process the signal for pitch detection
  float pd_sig = m_preProcessor->operator()(in);

  // send the processed sample through the pitch detector
  m_pitchDetector->operator()(pd_sig);
  m_currentFrequency = as_float(frequency{m_pitchDetector->get_frequency()});

  // Try to get the latest frequency from the detector
  // m_currentFrequency = smoothingFilter(m_currentFrequency);

  m_note = Note(m_currentFrequency);
  m_octave = Octave(m_currentFrequency);
  m_cents = Cents(m_currentFrequency, m_note);
}

void TunerModule::ProcessStereo(float inL, float inR) { ProcessMono(inL); }

void TunerModule::DrawUI(OneBitGraphicsDisplay& display, int currentIndex,
                         int numItemsTotal, Rectangle boundsToDrawIn,
                         bool isEditing) {
  char currentNote[12];
  sprintf(currentNote, "%s%u", k_notes[m_note % 12], m_octave);

  char strbuffNote[64];
  sprintf(strbuffNote, "%s", currentNote);
  display.WriteStringAligned(strbuffNote, Font_16x26, boundsToDrawIn,
                             Alignment::topCentered, true);

  // Thresholds for tuning accuracy mapping to blocks
  const float close = 1.0f;
  const float medium = 3.0f;
  const float far = 10.0f;

  const int blockCount = 7;
  bool blockActive[blockCount] = {false};

  blockActive[3] = true;

  if (m_cents < -far) {
    blockActive[0] = true;
    blockActive[1] = true;
    blockActive[2] = true;
  } else if (m_cents < -medium) {
    blockActive[1] = true;
    blockActive[2] = true;
  } else if (m_cents < -close) {
    blockActive[2] = true;
  } else if (m_cents > far) {
    blockActive[4] = true;
    blockActive[5] = true;
    blockActive[6] = true;
  } else if (m_cents > medium) {
    blockActive[4] = true;
    blockActive[5] = true;
  } else if (m_cents > close) {
    blockActive[4] = true;
  }

  int width = boundsToDrawIn.GetWidth();
  int blockWidth = (width / blockCount);
  int top = 30;

  int x = 0;
  // Draw all of the active blocks
  for (int block = 0; block < blockCount; block++) {
    Rectangle r(x, top, blockWidth, blockWidth);
    display.DrawRect(r, true, blockActive[block]);
    x += blockWidth;
  }

  char strbuffFreq[64];
  sprintf(strbuffFreq, "%.2f", m_currentFrequency);
  display.WriteStringAligned(strbuffFreq, Font_7x10, boundsToDrawIn,
                             Alignment::bottomCentered, true);
}