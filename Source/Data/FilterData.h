#pragma once

#include <JuceHeader.h>

class FilterData : public juce::dsp::StateVariableTPTFilter<float>
{
public:
    FilterData();
    void setParams(const int type, const float frequency, const float resonance);
    void prepareToPlay(juce::dsp::ProcessSpec spec);
    void processNextBlock(juce::AudioBuffer<float> &buffer);
    void selectFilterType(int filterType);
    int getFilterType();
    void setFilterCutOffFrequency(float frequency);
    void setFilterCutOffADSR(float mod);
    void setFilterRes(float res);
    void setLFOFreq (float level);
    void setLFODepth(float level);
    float getLFOFreq();
    float getLFODepth();
    
private:
    float frequencyValue=1000;
    int filterType = 0;
    juce::dsp::Oscillator<float> lfo{[](float x){ return std::sin(x); }};
    float lfoMod;
    float lfoDepth = 0.0f;
};
