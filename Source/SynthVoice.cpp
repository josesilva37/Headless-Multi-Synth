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

    switch (ADSRControl)
    {
    case 0:
        adsr.setParameters(parameters);
        break;
    case 1:
        adsrFilter.setParameters(parameters);
        juce::Logger::writeToLog("Change Filter Attack" + juce::String(adsrFilter.getParameters().attack));
        break;
    }
};
void SynthVoice::changeDecay(float decayValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.decay = decayValue / 100;
    switch (ADSRControl)
    {
    case 0:
        adsr.setParameters(parameters);
        break;
    case 1:
        adsrFilter.setParameters(parameters);
        break;
    }
};
void SynthVoice::changeSustain(float sustainValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.sustain = sustainValue / 100;
    switch (ADSRControl)
    {
    case 0:
        adsr.setParameters(parameters);
        break;
    case 1:
        adsrFilter.setParameters(parameters);
        break;
    }
};
void SynthVoice::changeRelease(float releaseValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.release = releaseValue / 100;
    switch (ADSRControl)
    {
    case 0:
        adsr.setParameters(parameters);
        break;
    case 1:
        adsrFilter.setParameters(parameters);
        break;
    }
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

void SynthVoice::setLFOControl(int type)
{
    LFOControl = type;
}
void SynthVoice::setADSRControl(int type)
{
    ADSRControl = type;
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
    whitenoiseLevel = previouseWhitenoiseLevel;
    osc.setWaveFrequency(midiNoteNumber);
    adsrFilter.noteOn();
    adsr.noteOn();
};
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    whitenoiseLevel = 0;
    adsrFilter.noteOff();
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
    reverbParams.roomSize = level;
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

void SynthVoice::setWhitenoiseLevel(float level){
    previouseWhitenoiseLevel = level;
}
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{

    adsr.setSampleRate(sampleRate);
    adsrFilter.setSampleRate(sampleRate);
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
    reverbParams.damping = 0.5f;
    reverbParams.wetLevel = 0.33f;
    reverbParams.width = 1.0f;
    delay.reset();
    delay.prepare(spec);
    delay.setMaximumDelayInSamples(44100 * 2);
    limiter.prepare(spec);
    limiter.setThreshold(0.5f);
};
void SynthVoice::resetSynthParams()
{
    adsr.reset();
    adsrFilter.reset();
    // parameters.attack = 1.0f;
    // parameters.decay = 1.0f;
    // parameters.release = 1.0f;
    // parameters.sustain = 1.0f;
    // adsr.setParameters(parameters);
    // adsrFilter.setParameters(parameters);
    lfoDepth = 0;
    lfo.setFrequency(20.0f);
    reverbParams.damping = 0.5f;
    reverbParams.wetLevel = 0.33f;
    reverbParams.width = 1.0f;
    reverb.setEnabled(false);
    delay.setDelay(0);
    LFOControl = 0;
    osc.setType(0);
    osc.setFmType(0);
    osc.setFmDepth(0);
    osc.setLFODepth(0);
    filter.setLFODepth(0);
    filter.setFilterCutOffFrequency(1000);
}
void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    juce::dsp::AudioBlock<float> audioBlock{outputBuffer};

    float lfoValue = lfo.processSample(0);

    float lfoMod = (lfoValue * lfoDepth) / 500;

    float finalGain = osc.getGain();

    if (finalGain + lfoMod >= 0 && finalGain + lfoMod <= 1.0f)
    {
        finalGain += lfoMod;
    }

    gain.setGainLinear(finalGain);

    osc.getNextAudioBlock(outputBuffer);
    adsr.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());
    adsrFilter.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());

    if (ADSRControl == 1)
    {
        filter.setFilterCutOffADSR(adsrFilter.getNextSample());
    }
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        auto *buffer = outputBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            // juce::Logger::writeToLog(juce::String(adsr.getNextSample()));
            buffer[sample] += random.nextFloat() * whitenoiseLevel  - 0.125f;
        }
    }

    filter.processNextBlock(outputBuffer);

    // Effects

    if (reverb.isEnabled())
    {
        reverb.setParameters(reverbParams);
        reverb.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    }

    if (enableDelay)
    {
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            auto *data = outputBuffer.getWritePointer(channel);
            for (int sample = 0; sample < numSamples; ++sample)
            {
                delay.pushSample(channel, data[sample]);
                data[sample] += delay.popSample(channel, delay.getDelay()) * feedbackAttenuation;
            }
        }
    }

    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    limiter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
};
