#include "CarrOsc.h"
template void CarrOsc::process<juce::dsp::ProcessContextReplacing<float>>(juce::dsp::ProcessContextReplacing<float> const&);

CarrOsc::CarrOsc()
{
    auto &osc = processorChain.template get<oscIndex>();
    osc.initialise([](float x)
                   { return std::sin(x); },
                   128);
}

CarrOsc::~CarrOsc() {}

void CarrOsc::prepare(const juce::dsp::ProcessSpec &spec)
{
    processorChain.prepare(spec);
}

template <typename ProcessContext>
void CarrOsc::process(const ProcessContext &context) noexcept
{
    processorChain.process(context); // [9]
}

void CarrOsc::setFrequency(float newValue, bool force)
{
    force = false;
    auto &osc = processorChain.template get<oscIndex>();
    osc.setFrequency(newValue, force); // [7]
}
void CarrOsc::setLevel(float newValue)
{
    auto &gain = processorChain.template get<gainIndex>();
    gain.setGainLinear(newValue); // [8]
}

void CarrOsc::reset()
{
    processorChain.reset();
}
