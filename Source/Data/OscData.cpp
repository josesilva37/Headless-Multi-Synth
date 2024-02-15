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
    fmOsc.prepare(spec);
    prepare(spec);
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

    default:
        jassertfalse;
        break;
    }
}
int OscData::getType(){
    return type;
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
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod);
}
void OscData::setFmFreq(const float freq)
{
    fmOsc.setFrequency(freq);
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod);
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber;
}
void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float> &block)
{
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
        }
    }
    // juce::Logger::writeToLog(juce::String(fmOsc.getFrequency()) + "  -  " + juce::String(fmDepth));

    process(juce::dsp::ProcessContextReplacing<float>(block));
}