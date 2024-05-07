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
    if (FmAlgh == 0)
    {

        fmDepth = depth;
        setFrequency(juce::MidiMessage::getMidiNoteInHertz(lastMidiNote));
    }
    else
    {
        switch (selectedOperator)
        {
        case 1:
            op1Depth = depth;
            break;
        case 2:
            op2Depth = depth;
            break;
        case 3:
            op3Depth = depth;
            break;
        case 4:
            op4Depth = depth;
            break;
        case 5:
            op5Depth = depth;
            break;
        default:
            break;
        }
    }
}
void OscData::setFmFreq(const float freq)
{
    if (FmAlgh == 0)
    {
        fmOsc.setFrequency(freq);
        setFrequency(juce::MidiMessage::getMidiNoteInHertz(lastMidiNote));
    }
    else
    {
        switch (selectedOperator)
        {
        case 1:
            OP1.setFrequency(freq);
            break;
        case 2:
            OP2.setFrequency(freq);
            break;
        case 3:
            OP3.setFrequency(freq);
            break;
        case 4:
            OP4.setFrequency(freq);
            break;
        case 5:
            OP5.setFrequency(freq);
            break;
        default:
            break;
        }
    }
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
    juce::Logger::writeToLog("op1" + juce::String(OP1.getFrequency()));
    juce::Logger::writeToLog("op2" + juce::String(OP2.getFrequency()));
    juce::Logger::writeToLog("op3" + juce::String(OP3.getFrequency()));
    juce::Logger::writeToLog("op4" + juce::String(OP4.getFrequency()));
    juce::Logger::writeToLog("op5" + juce::String(OP5.getFrequency()));

    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        auto *data = buffer.getWritePointer(ch);
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            op1Mod = OP1.processSample(block.getSample(ch, s)) * op1Depth;
            op2Mod = OP2.processSample(block.getSample(ch, s)) * op2Depth;
            op3Mod = OP3.processSample(block.getSample(ch, s)) * op3Depth;
            op4Mod = OP4.processSample(block.getSample(ch, s)) * op4Depth;
            op5Mod = OP5.processSample(block.getSample(ch, s)) * op5Depth;

            float op5Frequency = OP5.getFrequency() + op5Mod * 0.5;
            OP5.setFrequency(std::abs(op5Frequency));

            float op4Frequency = OP4.getFrequency() + op5Mod;
            OP4.setFrequency(std::abs(op4Frequency));

            float op3Frequency = OP3.getFrequency() + op4Mod;
            OP3.setFrequency(std::abs(op3Frequency));

            float chain2Freq = lastFreq + op3Mod + op4Mod + op5Mod;

            float currentFreq = lastFreq + op1Mod;

            setFrequency(std::abs(currentFreq));
            OP2.setFrequency(std::abs(chain2Freq));

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
                setFrequency(std::abs(currentFreq));
            }
        }
        break;
    case 1:
        processFMAlgh1(buffer);
        break;
    }

    process(juce::dsp::ProcessContextReplacing<float>(block));
}