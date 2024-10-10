
// Reference
// https://github.com/sonofmagni/FreqDetection/blob/main/FreqDetect.cpp
#include "frequency_detector.h"

#include "daisy_seed.h"
#include "daisysp.h"

#define MAX_SIZE (48000 / 2)

float DSY_SDRAM_BSS buf[MAX_SIZE];

using namespace daisysp;
using namespace std;

FrequencyDetector::FrequencyDetector() {}
FrequencyDetector::~FrequencyDetector() {}

void FrequencyDetector::Init(float sample_rate) {
  m_lpFilter.Init(48000, 1000);
  ResetBuffer();
}

void FrequencyDetector::Process(float input) {
  m_lockPos = m_pos;
  buf[m_pos] = m_lpFilter.Process(input);

  m_pos++;
  if (m_pos == MAX_SIZE) {
    m_pos = 0;
  }
}

void FrequencyDetector::ResetBuffer() {
  m_pos = 0;
  for (int i = 0; i < MAX_SIZE; i++) {
    buf[i] = 0;
  }
}

// Return the average of the recent samples
float FrequencyDetector::CalcMovAve() {
  float sum = 0;
  for (int ptr = 0; ptr < AVE_BUF_SIZE; ptr++) {
    sum = sum + m_aveBuffer[ptr];
  }
  return sum / float(AVE_BUF_SIZE);
}

// Add a sample to the collection of recent samples
void FrequencyDetector::AddMovAveValue(float val) {
  if (m_aveBufPtr >= AVE_BUF_SIZE) {
    m_aveBufPtr = 0;
  }
  m_aveBuffer[m_aveBufPtr] = val;
  m_aveBufPtr++;
}

float FrequencyDetector::GetPeakPeakInterval() {
  // avoid lockpos
  // find zero point
  // find peaks using moving average
  // collect intervals
  // average n intervals
  // throw out bad intervals
  // average remaining intervals

  int sampleStart = 0;
  bool midStart = false;
#define PEAK_SAMPLE_COUNT 32
  int peakSamples[PEAK_SAMPLE_COUNT];
  int peakSampleIndex = 0;
  int peakSamplePeriods[PEAK_SAMPLE_COUNT];
#define SCAN_LENGTH 4800

  if (m_lockPos < MAX_SIZE * 0.4) {
    sampleStart = MAX_SIZE / 2;  // this still gives us 6000 samples to look
                                 // at which would be 5 cycles at 40hz
    midStart = true;
  }

  // Preload the moving average buffer
  for (int fill = 0; fill < AVE_BUF_SIZE; fill++) {
    AddMovAveValue(buf[sampleStart]);
    sampleStart++;
  }

  // TODO: messy
  int searchEnd = MAX_SIZE / 3;
  if (midStart) searchEnd = MAX_SIZE;

  // first we need to understand how big a peak is
  // this is so we can reject peaks that are not significant
  float peakValue = 0.0;
  for (int sampleIndex = sampleStart; sampleIndex < sampleStart + 1200;
       sampleIndex++)  // at least one peak at 40hz
  {
    AddMovAveValue(buf[sampleIndex]);
    float thisPeak = CalcMovAve();
    if (peakValue < thisPeak) {
      peakValue = thisPeak;
    }
  }

  // don't try to process tiny signals
  if (peakValue < 0.005) {
    return 100000;  // almost 0hz
  }

  // reject peaks that are not 80% of the one we found
  peakValue = peakValue * 0.9;

  // Preload the moving average buffer - TODO: refactor this
  for (int fill = 0; fill < AVE_BUF_SIZE; fill++) {
    AddMovAveValue(buf[sampleStart]);
    sampleStart++;
  }

  peakSampleIndex = 0;
  bool foundThisPeak = false;
  // collect peaks using moving average
  for (int sampleIndex = sampleStart; sampleIndex < searchEnd; sampleIndex++) {
    AddMovAveValue(buf[sampleIndex]);
    // done with the previous peak
    if (CalcMovAve() < peakValue * -0.2) {
      foundThisPeak = false;
    }

    if (CalcMovAve() > peakValue && !foundThisPeak) {
      // continue looking until the peak value starts to decrease (the peak
      // of the peak)
      float nearPeak = CalcMovAve();
      sampleIndex++;
      for (; sampleIndex < searchEnd; sampleIndex++) {
        AddMovAveValue(buf[sampleIndex]);
        float newPeak = CalcMovAve();
        if (nearPeak <= newPeak) {
          nearPeak = newPeak;
        } else  // if (nearPeak > newPeak)
        {
          peakSamples[peakSampleIndex] = sampleIndex;
          peakSampleIndex++;
          foundThisPeak = true;
          break;
        }
      }
      if (peakSampleIndex >= PEAK_SAMPLE_COUNT) break;
    }
  }

  // don't try to process with too few samples
  if (peakSampleIndex < 4) {
    return 100000;  // almost 0hzP
  }

  peakSampleIndex--;

  int numPeakSamples = peakSampleIndex + 1;
  int numPeriods = peakSampleIndex;
  // now we have peakSamples[] with the positive peaks and numPeakSamples is
  // the count convert the array of peak sample pointers into an array of
  // periods
  for (peakSampleIndex = 1; peakSampleIndex < numPeakSamples;
       peakSampleIndex++) {
    peakSamplePeriods[peakSampleIndex - 1] =
        peakSamples[peakSampleIndex] - peakSamples[peakSampleIndex - 1];
  }

  // average the periods - would median work better?
  int totalPeriod = 0;
  int averagePeriod = 0;
  for (int perIndex = 0; perIndex < numPeriods; perIndex++) {
    totalPeriod = totalPeriod + peakSamplePeriods[perIndex];
  }

  averagePeriod = totalPeriod / numPeriods;

  int goodPerIndex = 0;
  int goodPeriods[PEAK_SAMPLE_COUNT];
  // throw out bad values
  for (int perIndex = 0; perIndex < numPeriods; perIndex++) {
    if (peakSamplePeriods[perIndex] < averagePeriod * 1.6 &&
        peakSamplePeriods[perIndex] > averagePeriod * 0.65) {
      goodPeriods[goodPerIndex] = peakSamplePeriods[perIndex];
      goodPerIndex++;
    }
  }

  int goodPerSum = 0;
  int goodPerCount = goodPerIndex;
  for (goodPerIndex = 0; goodPerIndex < goodPerCount; goodPerIndex++) {
    goodPerSum += goodPeriods[goodPerIndex];
  }
  int goodPerAverage = goodPerSum / goodPerCount;  // TODO: messy

  return goodPerAverage * 0.0000208333;
}