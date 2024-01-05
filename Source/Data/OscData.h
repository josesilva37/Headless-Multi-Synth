/*
  ==============================================================================

    OscData.h
    Created: 5 Jan 2024 4:36:50pm
    Author:  jose

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
public:
    // void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void setType (const int oscSelection);
    // void setGain (const float levelInDecibels);
    // void setOscPitch (const int pitch);
    // void setFreq (const int midiNoteNumber);
    // void setFmOsc (const float freq, const float depth);
    // void renderNextBlock (juce::dsp::AudioBlock<float>& audioBlock);
    // float processNextSample (float input);
    // void setParams (const int oscChoice, const float oscGain, const int oscPitch, const float fmFreq, const float fmDepth);
    // void resetAll();

private:
    // juce::dsp::Oscillator<float> fmOsc { [](float x) { return std::sin (x); }};
    // juce::dsp::Gain<float> gain;
};

// return std::sin (x); //Sine Wave
// return x / MathConstants<float>::pi; // Saw Wave
// return x < 0.0f ? -1.0f : 1.0f;  // Square Wave