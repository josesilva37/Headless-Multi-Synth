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
    void setGain (const float levelInDecibels);
    float getGain();
    bool isGainSet();
    // void setOscPitch (const int pitch);
    // void setFreq (const int midiNoteNumber);
    // void setFmOsc (const float freq, const float depth);
    // void renderNextBlock (juce::dsp::AudioBlock<float>& audioBlock);
    // float processNextSample (float input);
    // void setParams (const int oscChoice, const float oscGain, const int oscPitch, const float fmFreq, const float fmDepth);
    // void resetAll();

private:
    juce::dsp::Gain<float> gain;
    bool wasGainSet;
};
