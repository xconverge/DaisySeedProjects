#include "frequency_detector_alt.h"

#include "yin.h"

#define BUFFER_LEN 2048
float buffer[BUFFER_LEN];
Yin yin;

FrequencyDetectorAlt::FrequencyDetectorAlt() {
  //
}

FrequencyDetectorAlt::~FrequencyDetectorAlt() {
  //
}

void FrequencyDetectorAlt::Init(float sampleRate) {
  //
}

void FrequencyDetectorAlt::Process(float in) {
  if (m_bufferIndex < BUFFER_LEN) {
    buffer[m_bufferIndex++] = in;
  } else if (m_bufferIndex == BUFFER_LEN) {
    m_bufferIndex = 0;

    Yin_init(&yin, BUFFER_LEN, 0.05);
    m_cachedFrequency = Yin_getPitch(&yin, buffer);
  }
}