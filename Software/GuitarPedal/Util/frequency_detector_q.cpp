#include "frequency_detector_q.h"

#include <q/pitch/pitch_detector.hpp>

using namespace cycfi::q;

float sample_rate = 48000;

// The frequency detection bounds;
// C2
frequency lowest_frequency = frequency(65.41f);
// C5
frequency highest_frequency = frequency(523.251f);

// The pitch detector
pitch_detector pd{lowest_frequency, highest_frequency, sample_rate, -40_dB};

FrequencyDetectorQ::FrequencyDetectorQ() {
  //
}

FrequencyDetectorQ::~FrequencyDetectorQ() {
  //
}

void FrequencyDetectorQ::Init(float sampleRate) {
  //
}

void FrequencyDetectorQ::Process(float in) {
  if (pd(in)) {
    m_cachedFrequency = as_float(frequency{pd.get_frequency()});
  }
}