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
  void prepareToPlay ( juce::dsp::ProcessSpec spec);
  void setType(const int oscSelection);
  void setGain(const float levelInDecibels);
  float getGain();
  bool isGainSet();
  void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
  void setWaveFrequency(const int midiNoteNumber);
  void setFmFreq(const float freq);
  void setFmDepth(const float depth);
private:
  juce::dsp::Oscillator<float> fmOsc { [](float x) { return std::sin (x); }};
  float fmMod {5.0f};
  float fmDepth {0};
  int lastMidiNote {0};
  juce::dsp::Gain<float> gain;
  bool wasGainSet;
};
