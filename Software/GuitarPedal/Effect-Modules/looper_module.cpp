#include "looper_module.h"

#include "../Util/audio_utilities.h"

using namespace bkshepherd;

// 60 seconds at 48kHz
#define kBuffSize 48000 * 60

float DSY_SDRAM_BSS bufferL[kBuffSize];
float DSY_SDRAM_BSS bufferR[kBuffSize];

static const ParameterMetaData s_metaData[0] = {};

// Default Constructor
LooperModule::LooperModule() : BaseEffectModule() {
  // Set the name of the effect
  m_name = "Looper";

  // Setup the meta data reference for this Effect
  m_paramMetaData = s_metaData;

  // Initialize Parameters for this Effect
  this->InitParams(0);
}

// Destructor
LooperModule::~LooperModule() {
  // No Code Needed
}

void LooperModule::Init(float sample_rate) {
  BaseEffectModule::Init(sample_rate);

  // Init the loopers
  m_looperL.Init(bufferL, kBuffSize);
  m_looperR.Init(bufferR, kBuffSize);
}

void LooperModule::AlternateFootswitchPressed() {
  m_looperL.TrigRecord();
  m_looperR.TrigRecord();
}

void LooperModule::AlternateFootswitchHeld() {
  // If you hold the button longer than 1000 ms (1 sec),
  // clear the loop
  m_looperL.Clear();
  m_looperR.Clear();
}

void LooperModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  const float gain = 1.0f;
  const float levelLoop = 1.0f;

  float input = in * gain;

  // store signal = loop signal * loop gain + in * in_gain
  float looperOutput = m_looperL.Process(input) * levelLoop + input;

  m_audioRight = m_audioLeft = looperOutput;
}

void LooperModule::ProcessStereo(float inL, float inR) {
  BaseEffectModule::ProcessStereo(inL, inR);

  const float gain = 1.0f;
  const float levelLoop = 1.0f;

  float inputL = inL * gain;
  float inputR = inR * gain;

  // store signal = loop signal * loop gain + in * in_gain
  float looperOutputL = m_looperL.Process(inputL) * levelLoop + inputL;
  float looperOutputR = m_looperR.Process(inputR) * levelLoop + inputR;

  m_audioLeft = looperOutputL;
  m_audioRight = looperOutputR;
}

float LooperModule::GetBrightnessForLED(int led_id) {
  float value = BaseEffectModule::GetBrightnessForLED(led_id);

  if (led_id == 1) {
    // Enable the LED if the looper is recording
    return value * m_looperL.Recording();
  }

  return value;
}