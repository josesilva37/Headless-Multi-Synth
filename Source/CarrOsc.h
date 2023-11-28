#pragma once

#include <JuceHeader.h>

class CarrOsc
{
public:
    CarrOsc();
    ~CarrOsc();
    
    void prepare(const juce::dsp::ProcessSpec &spec);
    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept;
    void setFrequency(float newValue, bool force);
    void setLevel (float newValue);
    void reset();

private:
    enum
    {
        oscIndex,
        gainIndex // [2]
    };

    juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Gain<float>> processorChain;

};