#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound *sound)
{
    return dynamic_cast<juce::SynthesiserSound *>(sound) != nullptr;
};

void SynthVoice::changeFmDepth(float depth)
{
    osc.setFmDepth(depth);
}
void SynthVoice::changeFmFreq(float freq)
{
    osc.setFmFreq(freq);
}
void SynthVoice::changeAttack(float attackValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();

    parameters.attack = attackValue / 100;

    adsr.setParameters(parameters);
};
void SynthVoice::changeDecay(float decayValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.decay = decayValue / 100;
    adsr.setParameters(parameters);
};
void SynthVoice::changeSustain(float sustainValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.sustain = sustainValue / 100;
    adsr.setParameters(parameters);
};
void SynthVoice::changeRelease(float releaseValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.release = releaseValue / 100;
    adsr.setParameters(parameters);
};
void SynthVoice::changeFilterType(int type)
{
    filter.selectFilterType(type);
}
void SynthVoice::changeFilterCutOffFreq(float cutofffreq)
{
    filter.setFilterCutOffFrequency(cutofffreq);
}

void SynthVoice::changeFilterResonance(float resonsance)
{
    filter.setResonance(resonsance);
}
int SynthVoice::getLFOControl()
{
    return LFOControl;
}
void SynthVoice::setLFOControl(int type)
{
    LFOControl = type;
}
void SynthVoice::setLFOGainDepth(float level)
{
    lfoDepth = level;
}
void SynthVoice::setLFOGainFreq(float level)
{
    lfo.setFrequency(level);
}
void SynthVoice::changeFilterLFODepth(float level)
{
    filter.setLFODepth(level);
}
void SynthVoice::changeFilterLFOFreq(float level)
{
    filter.setLFOFreq(level);
}
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    osc.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
};
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
};
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue){

};
void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

    osc.setPitchBend(newPitchWheelValue);
};

void SynthVoice::setReverbWetLevel(float level)
{

    if (level == 0)
    {
        reverb.setEnabled(false);
    }
    else
    {
        reverb.setEnabled(true);
    }

    reverbParams.wetLevel = level;
    reverbParams.width = level;
};

void SynthVoice::setDelay(float delayLevel, float feedback)
{
    if (delayLevel == 0)
    {
        enableDelay = false;
    }
    else
    {
        enableDelay = true;
        delay.setDelay(delayLevel);
    }
};
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{

    adsr.setSampleRate(sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    lfo.prepare(spec);
    lfo.setFrequency(20.0f);
    osc.prepareToPlay(spec);
    osc.setType(0);
    filter.prepareToPlay(spec);
    gain.prepare(spec);
    gain.setGainLinear(0.3f);
    reverb.prepare(spec);
    reverb.setEnabled(false);
    delay.reset();
    delay.prepare(spec);
    delay.setMaximumDelayInSamples(44100 * 2);
    limiter.prepare(spec);
    limiter.setThreshold(0.5f);
};

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    juce::dsp::AudioBlock<float> audioBlock{outputBuffer};
    osc.getNextAudioBlock(outputBuffer);

    float lfoValue = lfo.processSample(0);

    float lfoMod = (lfoValue * lfoDepth) / 500;

    float finalGain = osc.getGain();
    if (finalGain + lfoMod >= 0 && finalGain + lfoMod <= 1.0f)
    {
        finalGain += lfoMod;
    }

    gain.setGainLinear(finalGain);

    filter.processNextBlock(outputBuffer);
    adsr.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());
    if (reverb.isEnabled())
    {
        reverb.setParameters(reverbParams);
        reverb.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    }

    if (enableDelay)
    {
        float blockDelayFeedback = delayFeedback;
        juce::Logger::writeToLog(juce::String(blockDelayFeedback));
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            auto *data = outputBuffer.getWritePointer(channel);
            for (int sample = 0; sample < numSamples; ++sample)
            {
                delay.pushSample(channel, data[sample]);
                data[sample] += delay.popSample(channel, delay.getDelay()) * blockDelayFeedback;
            }
        }

        blockDelayFeedback *= feedbackAttenuation;
    }
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    limiter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
};
