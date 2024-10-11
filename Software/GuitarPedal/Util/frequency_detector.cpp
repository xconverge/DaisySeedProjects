#include "frequency_detector.h"

#include "arm_const_structs.h"
#include "arm_math.h"

#define FFT_SIZE 2048
#define FFT_BUF_SIZE (2 * FFT_SIZE)
float fft_buf[FFT_BUF_SIZE];
float output[FFT_SIZE];

FrequencyDetector::FrequencyDetector() {
  //
}

FrequencyDetector::~FrequencyDetector() {
  //
}

void FrequencyDetector::Init(float sampleRate) {
  m_cachedSampleRate = sampleRate;
}

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

void FrequencyDetector::Process(float in) {
  if (m_fft_buf_index < FFT_BUF_SIZE) {
    fft_buf[m_fft_buf_index++] = in;
    fft_buf[m_fft_buf_index++] = 0.f;
  } else if (m_fft_buf_index == FFT_BUF_SIZE) {
    // do FFT
    m_cachedFrequency = do_fft(fft_buf, output, m_cachedSampleRate);
    // start collecting samples again
    m_fft_buf_index = 0;
  }
}