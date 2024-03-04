#include "FilterData.h"

FilterData::FilterData()
{
    setType(juce::dsp::StateVariableTPTFilterType::lowpass);
};

void FilterData::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    reset();
    prepare(spec);
};

void FilterData::setParams(const int type, const float frequency, const float resonance)
{
    setCutoffFrequency(frequency);
    setResonance(resonance);
    selectFilterType(type);
};

void FilterData::selectFilterType(int type){
 switch (type)
    {
    case 0:
        setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        filterType = 0;
    case 1:
        setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        filterType = 1;

    case 2:
        setType(juce::dsp::StateVariableTPTFilterType::highpass);
        filterType = 2;
    }
}

void FilterData::processNextBlock(juce::AudioBuffer<float>& buffer){
    juce::dsp::AudioBlock<float> block {buffer};
    process(juce::dsp::ProcessContextReplacing<float>(block));
}   