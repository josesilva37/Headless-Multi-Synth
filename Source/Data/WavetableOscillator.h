#pragma once

#include <JuceHeader.h>

class WavetableOscillator
{
public:
    WavetableOscillator(const juce::AudioSampleBuffer &wavetableToUse)
        : wavetable(wavetableToUse),
          tableSize (wavetable.getNumSamples() - 1)
    {
        jassert(wavetable.getNumChannels() == 1);
    }
    void setFrequency(float frequency, float sampleRate);
    float getNextSample();
    void updateAngle();

private:
    const juce::AudioSampleBuffer &wavetable;
    float currentIndex = 0.0f, tableDelta = 0.0f;
    const int tableSize;
    };
