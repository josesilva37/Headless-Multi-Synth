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
  int getType();
  void setFmType(const int fmSelection);
  int getFmType();
  void getNextAudioBlock(juce::AudioBuffer<float> &buffer);
  void setWaveFrequency(const int midiNoteNumber);
  void setFmFreq(const float freq);
  float getFmFreq();
  void setFmDepth(const float depth);
  float getFmDepth();
  void setLFOFreq(const float freq);
  float getLFOFreq();
  void setLFODepth(const float res);
  float getLFODepth();
  void setPitchBend(float bend);
private:
  juce::dsp::Oscillator<float> fmOsc { [](float x) { return std::sin (x); }};
  juce::dsp::Oscillator<float> carrier { [](float x) { return std::sin (x); }};
  juce::dsp::Oscillator<float> lfo { [](float x) { return std::sin (x); }};
  float fmMod {5.0f};
  float fmDepth {0};
  float lfoMod;
  float lfoDepth= 0.0f;
  float lastFreq;
  int lastMidiNote {0};
  int type;
  int fmType;
  bool pitchBend = false;
  float pitchBendFreq;
};
