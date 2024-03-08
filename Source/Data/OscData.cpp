/*
  ==============================================================================

    OscData.cpp
    Created: 5 Jan 2024 4:36:56pm
    Author:  jose

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    lfo.prepare(spec);
    fmOsc.prepare(spec);
    prepare(spec);
    lfo.setFrequency(5.0f);
}
void OscData::setType(const int oscSelection)
{
    switch (oscSelection)
    {
    // Sine
    case 0:
        type = 0;
        initialise([](float x)
                   { return std::sin(x); });
        break;

    // Saw
    case 1:
        type = 1;
        initialise([](float x)
                   { return x / juce::MathConstants<float>::pi; });
        break;

    // Square
    case 2:
        type = 2;
        initialise([](float x)
                   { return x < 0.0f ? -1.0f : 1.0f; });
        break;
    // Triangle
    case 3:
        type = 3;
        initialise([](float x)
                   { return 2.0f * fabs(2.0f * (x / juce::MathConstants<float>::pi - floor(x / juce::MathConstants<float>::pi + 0.5f))) - 1.0f; });
        break;

    default:
        jassertfalse;
        break;
    }
}
int OscData::getType()
{
    return type;
}
void OscData::setFmType(const int oscSelection)
{
    switch (oscSelection)
    {
    // Sine
    case 0:
        fmType = 0;
        fmOsc.initialise([](float x)
                         { return std::sin(x); });
        break;

    // Saw
    case 1:
        fmType = 1;
        fmOsc.initialise([](float x)
                         { return x / juce::MathConstants<float>::pi; });
        break;

    // Square
    case 2:
        fmType = 2;
        fmOsc.initialise([](float x)
                         { return x < 0.0f ? -1.0f : 1.0f; });
        break;
    // Triangle
    case 3:
        fmType = 3;
        fmOsc.initialise([](float x)
                         { return 2.0f * fabs(2.0f * (x / juce::MathConstants<float>::pi - floor(x / juce::MathConstants<float>::pi + 0.5f))) - 1.0f; });
        break;

    default:
        jassertfalse;
        break;
    }
}
int OscData::getFmType()
{
    return fmType;
}
void OscData::setGain(const float levelInDecibels)
{
    gain.setGainLinear(levelInDecibels / 100);
    wasGainSet = true;
}
float OscData::getGain()
{
    return gain.getGainLinear();
}
bool OscData::isGainSet()
{
    return wasGainSet;
}

void OscData::setFmDepth(const float depth)
{
    fmDepth = depth;
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(lastMidiNote));
}
void OscData::setFmFreq(const float freq)
{
    fmOsc.setFrequency(freq);
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(lastMidiNote));
}
void OscData::setLFOFreq(const float freq)
{
    lfo.setFrequency(freq);
}
void OscData::setLFODepth(const float depth)
{
    lfoDepth = depth;
}
void OscData::setWaveFrequency(const int midiNoteNumber)
{
    juce::Logger::writeToLog(juce::String(lfoMod));

    setFrequency((juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber)));
    lastMidiNote = midiNoteNumber;
    lastFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
}
void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float> &block)
{
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
            lfoMod = lfo.processSample(block.getSample(ch, s)) * lfoDepth;
        }
    }
   
    float currentFreq = lastFreq + fmMod + lfoMod;
    // juce::Logger::writeToLog("LFO");

    // juce::Logger::writeToLog(juce::String(lfoMod));
    // juce::Logger::writeToLog("Total");

    // juce::Logger::writeToLog(juce::String(currentFreq));
    if(currentFreq <0){
        currentFreq = -currentFreq;
    }
    setFrequency(currentFreq);
    process(juce::dsp::ProcessContextReplacing<float>(block));
}