// Referenced some functions from https://github.com/snail23/ironpedal
#include "tuner_module.h"

#include "../Util/audio_utilities.h"
#include "arm_const_structs.h"
#include "arm_math.h"

// Using FFT size of 2048 needs 16K of sample buffer, and only gets a resolution
// of 47Hz.
#define FFT_SIZE 2048
#define FFT_BUF_SIZE (2 * FFT_SIZE)
float fft_buf[FFT_BUF_SIZE];
float output[FFT_SIZE];

// global, so do_fft() knows when to run
int fft_buf_index = 0;

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

float do_fft(float *input, float *output, float sampleRate) {
  float32_t maxValue;
  uint32_t index = 0;

  int Fmax = sampleRate / 2;  // 24000
  uint32_t Nbins = FFT_SIZE / 2;

  /* Process the data through the CFFT/CIFFT module */
  /* this must match FFT_SIZE, and 2048 is as high as Daisy can fit */
  arm_cfft_f32(&arm_cfft_sR_f32_len2048, input, 0 /*ifftFlag*/,
               1 /*doBitReverse*/);

  /* Process the data through the Complex Magnitude Module for
  calculating the magnitude at each bin */
  arm_cmplx_mag_f32(input, output, FFT_SIZE /*fftSize*/);

  /* Calculates maxValue and returns corresponding BIN value */
  arm_max_f32(output, FFT_SIZE /*fftSize*/, &maxValue, &index);

  /*
      to calculate the freq of the selected bin
      N (Bins) = FFT Size/2
      FR = Fmax/N(Bins)
  */

  // error test
  if (index >= Nbins) return -1.f;

  // good result
  return (float)index * ((float)Fmax / Nbins);
}

void TunerModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  if (fft_buf_index < FFT_BUF_SIZE) {
    fft_buf[fft_buf_index++] = in;
    fft_buf[fft_buf_index++] = 0.f;
  }

  if (fft_buf_index == FFT_BUF_SIZE) {
    // do FFT
    float frequency = do_fft(fft_buf, output, m_cachedSampleRate);
    // start collecting samples again
    fft_buf_index = 0;

    if (frequency && round(m_currentFrequency) != frequency) {
      m_currentFrequency = frequency;
    }
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