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
  void prepareToPlay(juce::dsp::ProcessSpec spec);
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
  void setOp1Freq(float freq) { OP1.setFrequency(freq); };
  void setOp1Depth(float depth) { op1Depth = depth; };
  void setOp2Freq(float freq) { OP2.setFrequency(freq); };
  void setOp2Depth(float depth) { op2Depth = depth; };
  void setOp3Freq(float freq) { OP3.setFrequency(freq); };
  void setOp3Depth(float depth) { op3Depth = depth; };
  void setOp4Freq(float freq) { OP4.setFrequency(freq); };
  void setOp4Depth(float depth) { op4Depth = depth; };
  void setOp5Freq(float freq) { OP5.setFrequency(freq); };
  void setOp5Depth(float depth) { op5Depth = depth; };
  void setPitchBend(float bend);
  void processFMAlgh1(juce::AudioBuffer<float> &buffer);
  void processFMAlgh2(juce::AudioBuffer<float> &buffer);
  void processFMAlgh3(juce::AudioBuffer<float> &buffer);
  void processFMAlgh4(juce::AudioBuffer<float> &buffer);
  void processFMAlgh5(juce::AudioBuffer<float> &buffer);
  void processFMAlgh6(juce::AudioBuffer<float> &buffer);
  void getFMOperatorsTreeParams(juce::ValueTree tree);
  void setFmAlgh(int fm) { FmAlgh = fm; };
  int getFmAlgh() { return FmAlgh; };
  void setSelectedOperator()
  {
    if (selectedOperator != 5)
    {
      selectedOperator++;
    }
    else
    {
      selectedOperator = 1;
    }
  };

private:
  juce::dsp::Oscillator<float> fmOsc{[](float x)
                                     { return std::sin(x); }};
  juce::dsp::Oscillator<float> OP1{[](float x)
                                   { return std::sin(x); }};
  juce::dsp::Oscillator<float> OP2{[](float x)
                                   { return std::sin(x); }};
  juce::dsp::Oscillator<float> OP3{[](float x)
                                   { return std::sin(x); }};
  juce::dsp::Oscillator<float> OP4{[](float x)
                                   { return std::sin(x); }};
  juce::dsp::Oscillator<float> OP5{[](float x)
                                   { return std::sin(x); }};
  juce::dsp::Oscillator<float> lfo{[](float x)
                                   { return std::sin(x); }};
  float fmMod{5.0f};
  float fmDepth{0};
  float op1Mod, op2Mod, op3Mod, op4Mod, op5Mod;
  float op1Depth, op2Depth, op3Depth, op4Depth, op5Depth;
  float lfoMod;
  float lfoDepth = 0.0f;
  float lastFreq;
  int lastMidiNote{0};
  int type;
  int fmType;
  bool pitchBend = false;
  float pitchBendFreq;
  int FmAlgh = 0;
  int selectedOperator = 1;
};
