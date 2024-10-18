#include "frequency_detector_q.h"

#include <q/fx/signal_conditioner.hpp>
#include <q/pitch/pitch_detector.hpp>
#include <q/support/pitch_names.hpp>

using namespace cycfi::q;

float sample_rate = 48000;

// The frequency detection bounds;
frequency lowest_frequency = pitch_names::C[2];
frequency highest_frequency = pitch_names::C[5];

// The pitch detector
pitch_detector pd{lowest_frequency, highest_frequency, sample_rate, -40_dB};

// The pitch detector pre-processor
signal_conditioner::config preprocessor_config;
signal_conditioner preprocessor{preprocessor_config, lowest_frequency,
                                highest_frequency, sample_rate};

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
  // Pre-process the signal for pitch detection
  float pd_sig = preprocessor(in);

  // send the processed sample through the pitch detector
  if (pd(pd_sig)) {
    m_cachedFrequency = as_float(frequency{pd.get_frequency()});
  }
}