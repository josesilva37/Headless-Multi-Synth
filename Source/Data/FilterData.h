#pragma once

#include <JuceHeader.h>

class FilterData : public juce::dsp::StateVariableTPTFilter<float>
{
    public:
        FilterData();
        void setParams(const int type ,const float frequency, const float resonance );
        void prepareToPlay(juce::dsp::ProcessSpec spec);
        void processNextBlock(juce::AudioBuffer<float>& buffer);
        void selectFilterType(int filterType);
    private:
     int filterType = 0;
};
