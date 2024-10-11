#include "tuner_module.h"

#include "../Util/audio_utilities.h"
#include "../Util/fourier.h"
#include "../Util/shy_fft.h"
#include "../Util/wave.h"

using namespace soundmath;
using namespace bkshepherd;

////
#define PI 3.1415926535897932384626433832795
#define SR 48000
typedef float S;  // sample type

using namespace daisy;
using namespace daisysp;
using namespace soundmath;

// convenient lookup tables
Wave<S> hann([](S phase) -> S { return 0.5 * (1 - cos(2 * PI * phase)); });
Wave<S> halfhann([](S phase) -> S { return sin(PI * phase); });

// 4 overlapping windows of size 2^12 = 4096
const size_t order = 12;
const size_t N = (1 << order);
const S sqrtN = sqrt(N);
const size_t laps = 4;
const size_t buffsize = 2 * laps * N;

// buffers for STFT processing
// audio --> in --(fft)--> middle --(process)--> out --(ifft)--> in -->
// each of these is a few circular buffers stacked end-to-end.
// buffers for input and output (from / to user // audio callback)
S DSY_SDRAM_BSS in[buffsize];
// buffers for unprocessed frequency domain data
S DSY_SDRAM_BSS middle[buffsize];
// buffers for processed frequency domain data
S DSY_SDRAM_BSS out[buffsize];

ShyFFT<S, N, RotationPhasor>* fft;  // fft object
Fourier<S, N>* stft;                // stft object

// initial parameters for denoise process
// if testing without hardware control, try changing these values
// beta = mix between high- and low-energy frequency bands
// thresh = cutoff for designation of a bin as high- or low-energy
S beta = 1, thresh = 15;
// shy_fft packs arrays as [real, real, real, ..., imag, imag, imag, ...]
inline void denoise(const S* in, S* out) {
  // convenient constant for grabbing imaginary parts
  static const size_t offset = N / 2;

  S average = 0;
  for (size_t i = 0; i < N; i++) {
    out[i] = 0;
    average += in[i] * in[i];
  }

  average /= N;

  for (size_t i = 0; i < N / 2; i++) {
    if ((in[i] * in[i] + in[i + offset] * in[i + offset]) <
        thresh * thresh * average) {
      // rescale the low-amplitude frequency bins by (1 - beta) ...
      out[i] = (1 - beta) * in[i];
      out[i + offset] = (1 - beta) * in[i + offset];
    } else {
      // ... and the high-amplitude ones by beta
      out[i] = beta * in[i];
      out[i + offset] = beta * in[i + offset];
    }
  }
}
////

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

  // initialize FFT and STFT objects
  fft = new ShyFFT<S, N, RotationPhasor>();
  fft->Init();
  stft = new Fourier<S, N>(denoise, fft, &hann, laps, in, middle, out);
}

// Destructor
TunerModule::~TunerModule() {
  delete fft;
  delete stft;
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

void TunerModule::ProcessMono(float in) {
  BaseEffectModule::ProcessMono(in);

  const float frequency = 0;

  if (frequency && round(m_currentFrequency) != frequency) {
    m_currentFrequency = frequency;
  }

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

  boundsToDrawIn.RemoveFromTop(center);
  display.WriteStringAligned(strbuff, Font_11x18, boundsToDrawIn,
                             Alignment::centered, true);
}