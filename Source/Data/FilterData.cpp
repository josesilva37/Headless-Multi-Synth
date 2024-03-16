#include "FilterData.h"

FilterData::FilterData()
{
    setType(juce::dsp::StateVariableTPTFilterType::lowpass);
};

void FilterData::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    reset();
    lfo.prepare(spec);
    lfo.setFrequency(5.0f);
    prepare(spec);
};

void FilterData::setParams(const int type, const float frequency, const float resonance)
{
    setCutoffFrequency(frequency);
    setResonance(resonance);
    selectFilterType(type);
};
void FilterData::setFilterCutOffFrequency(float freq){
    setCutoffFrequency(freq);
    frequencyValue = freq;
}

void FilterData::selectFilterType(int type)
{
    switch (type)
    {
    case 0:
        setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        filterType = 0;
        break;
    case 1:
        setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        filterType = 1;
        break;
    case 2:
        setType(juce::dsp::StateVariableTPTFilterType::highpass);
        filterType = 2;
        break;
    }
}
int FilterData::getFilterType()
{
    return filterType;
}
void FilterData::processNextBlock(juce::AudioBuffer<float> &buffer)
{
    juce::dsp::AudioBlock<float> block{buffer};
    float lfoValue = lfo.processSample(0);
    float lfoMod = (lfoValue * lfoDepth) ;

    if (frequencyValue + lfoMod > 0 &&  frequencyValue + lfoMod < 20000)
    {
        setCutoffFrequency(frequencyValue + lfoMod);
    }
    process(juce::dsp::ProcessContextReplacing<float>(block));
}