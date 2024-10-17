#include "frequency_detector_yin.h"

#include "yin.h"

#define BUFFER_LEN 2048
float buffer[BUFFER_LEN];
Yin yin;

FrequencyDetectorYin::FrequencyDetectorYin() : FrequencyDetectorInterface() {
  //
}

FrequencyDetectorYin::~FrequencyDetectorYin() {
  //
}

void FrequencyDetectorYin::Init(float sampleRate) {
  //
}

void FrequencyDetectorYin::Process(float in) {
  // TODO SK: This doesn't work right
  if (m_bufferIndex < BUFFER_LEN) {
    buffer[m_bufferIndex++] = in;
  } else if (m_bufferIndex == BUFFER_LEN) {
    m_bufferIndex = 0;

    Yin_init(&yin, BUFFER_LEN, 0.05);
    m_cachedFrequency = Yin_getPitch(&yin, buffer);
  }
}