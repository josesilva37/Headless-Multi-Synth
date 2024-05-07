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
    OP1.prepare(spec);
    OP2.prepare(spec);
    OP3.prepare(spec);
    OP4.prepare(spec);
    OP5.prepare(spec);
    lfo.setFrequency(0.0f);
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
float OscData::getFmDepth()
{
    return fmDepth;
}
float OscData::getFmFreq()
{
    return fmOsc.getFrequency();
}
void OscData::setLFOFreq(const float freq)
{
    lfo.setFrequency(freq);
}
void OscData::setLFODepth(const float depth)
{
    lfoDepth = depth;
}
float OscData::getLFOFreq()
{
    return lfo.getFrequency();
}
float OscData::getLFODepth()
{
    return lfoDepth;
}
void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setFrequency((juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber)));
    lastMidiNote = midiNoteNumber;
    lastFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
}
void OscData::setPitchBend(float pitchBendValue)
{
    if (pitchBendValue != 8192)
    {
        pitchBend = true;
    }
    else
    {
        pitchBend = false;
    }

    double bendRange = 12.0;                // 2 semitones
    double bendFactor = bendRange / 8192.0; // 8192 is the center value of the pitch bend range

    // Convert raw pitch bend value to a bend factor ranging from -1.0 to 1.0
    double normalizedBend = (static_cast<double>(pitchBendValue) / 8192.0) - 1.0;

    // Calculate the amount of pitch bend in semitones
    double bendInSemitones = normalizedBend * bendRange;

    // Apply pitch bend to the currently playing note
    double pitchBendFrequencyMultiplier = std::pow(2.0, bendInSemitones / 12.0);
    pitchBendFreq = lastFreq * pitchBendFrequencyMultiplier;
}

void OscData::processFMAlgh1(juce::AudioBuffer<float> &buffer)
{
    juce::dsp::AudioBlock<float> block{buffer};

    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        auto *data = buffer.getWritePointer(ch);
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            op1Mod = OP1.processSample(block.getSample(ch, s)) * 50;
            op2Mod = OP2.processSample(block.getSample(ch, s)) * op2Depth;
            op3Mod = OP3.processSample(block.getSample(ch, s)) * 100;
            op4Mod = OP4.processSample(block.getSample(ch, s)) * 650;
            op5Mod = OP5.processSample(block.getSample(ch, s)) * 40;

            op5Mod += op5Mod * 0.5;

            float currentFreq = lastFreq + op1Mod;
            float chain2Freq = lastFreq + op3Mod + op4Mod + op5Mod;

            if (currentFreq < 0)
            {
                currentFreq = -currentFreq;
            }
            if (chain2Freq < 0)
            {
                chain2Freq = -currentFreq;
            }

            setFrequency(currentFreq);
            OP2.setFrequency(chain2Freq);

            data[s] += OP2.processSample(0);
        }
    }
}
void OscData::getNextAudioBlock(juce::AudioBuffer<float> &buffer)
{
    juce::dsp::AudioBlock<float> block{buffer};

    switch (FmAlgh)
    {
    case 0:
        for (int ch = 0; ch < block.getNumChannels(); ++ch)
        {
            for (int s = 0; s < block.getNumSamples(); ++s)
            {
                fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
                lfoMod = lfo.processSample(block.getSample(ch, s)) * lfoDepth;

                float currentFreq = lastFreq + fmMod + lfoMod;

                if (pitchBend)
                {
                    currentFreq += pitchBendFreq;
                }

                if (currentFreq < 0)
                {
                    currentFreq = -currentFreq;
                }

                setFrequency(currentFreq);
            }
        }
        break;
    case 1:
        processFMAlgh1(buffer);
        break;
    }

    process(juce::dsp::ProcessContextReplacing<float>(block));
}