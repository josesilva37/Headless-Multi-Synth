#pragma once

#include <JuceHeader.h>

class DelayLine : public juce::dsp::DelayLine<float,juce::dsp::DelayLineInterpolationTypes::Linear>
{
    public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void renderNextBlock(juce::AudioBuffer<float> &buffer);
    

};
