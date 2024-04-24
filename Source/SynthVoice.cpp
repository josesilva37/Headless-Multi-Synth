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
    switch (ADSRControl)
    {
    case 0:
        adsrParams.attack = attackValue;
        break;
    case 1:
        filterParams.attack = attackValue;
        break;
    }
};
void SynthVoice::changeDecay(float decayValue)
{
    switch (ADSRControl)
    {
    case 0:
        adsrParams.decay =  decayValue;
        break;
    case 1:
        filterParams.decay = decayValue;
        break;
    }
};
void SynthVoice::changeSustain(float sustainValue)
{

    switch (ADSRControl)
    {
    case 0:
        adsrParams.sustain =  sustainValue;
        break;
    case 1:
        filterParams.sustain = sustainValue;
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
        adsrParams.release =  releaseValue;
        break;
    case 1:
        filterParams.release = releaseValue;
        break;
    }
};
void SynthVoice::setGain(float level){
    gainLevel = level;
}
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

void SynthVoice::setWhitenoiseLevel(float level)
{
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
    gain.setRampDurationSeconds(0.5);
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
    whitenoiseLevel = 0;
}
juce::ValueTree SynthVoice::serialize()
{
    juce::ValueTree tree("SynthPreset");
    tree.setProperty("Gain", gain.getGainLinear(), nullptr);
    tree.setProperty("Attack", adsr.getParameters().attack, nullptr);
    tree.setProperty("Decay", adsr.getParameters().decay, nullptr);
    tree.setProperty("Sustain", adsr.getParameters().sustain, nullptr);
    tree.setProperty("Release", adsrFilter.getParameters().release, nullptr);
    tree.setProperty("AttackFilter", adsrFilter.getParameters().attack, nullptr);
    tree.setProperty("DecayFilter", adsrFilter.getParameters().decay, nullptr);
    tree.setProperty("SustainFilter", adsrFilter.getParameters().sustain, nullptr);
    tree.setProperty("ReleaseFilter", adsrFilter.getParameters().release, nullptr);
    tree.setProperty("LfoFrequency", lfo.getFrequency(), nullptr);
    tree.setProperty("LfoDepth", lfoDepth, nullptr);
    tree.setProperty("OscType", osc.getType(), nullptr);
    tree.setProperty("FilterCutOff", filter.getCutoffFrequency(), nullptr);
    tree.setProperty("FilterResonance", filter.getResonance(), nullptr);
    tree.setProperty("FilterType", filter.getFilterType(), nullptr);
    tree.setProperty("FilterLFOFreq", filter.getLFOFreq(), nullptr);
    tree.setProperty("FilterLFODepth", filter.getLFODepth(), nullptr);
    tree.setProperty("whitenoiseLevel", whitenoiseLevel, nullptr);
    tree.setProperty("FmType", osc.getFmType(), nullptr);
    tree.setProperty("FmDepth", osc.getFmDepth(), nullptr);
    tree.setProperty("FmFreq", osc.getFmFreq(), nullptr);
    tree.setProperty("OscLFOFreq", osc.getLFOFreq(), nullptr);
    tree.setProperty("OscLFODepth", osc.getLFODepth(), nullptr);
    tree.setProperty("Reverb", reverb.getParameters().roomSize, nullptr);
    tree.setProperty("Delay", delay.getDelay(), nullptr);
    tree.setProperty("EnabledDelay", enableDelay ? "true" : "false", nullptr);
    tree.setProperty("EnabledReverb", reverb.isEnabled() ? "true": "false", nullptr);
    return tree;
}
void SynthVoice::savePreset(int presetNumber)
{
    juce::File rootFolder = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory);
    auto tree = serialize();
    juce::Logger::writeToLog(rootFolder.getFullPathName());

    std::unique_ptr<juce::XmlElement> xml(tree.createXml());

    if (xml != nullptr)
    {
        juce::File fileToSave = rootFolder.getChildFile("preset" + juce::String(presetNumber) + ".xml");
        juce::Logger::writeToLog("Saving preset to: " + fileToSave.getFullPathName());

        if (xml->writeTo(fileToSave, {}))
        {
            juce::Logger::writeToLog("Preset saved successfully");
        }
        else
        {
            juce::Logger::writeToLog("Error saving preset");
        }
    }
    else
    {
        // Handle error creating XML...
    }
}

void SynthVoice::loadPreset(int presetNumber)
{
    juce::ValueTree tree;
    juce::File rootFolder = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory);
    auto xml{juce::XmlDocument::parse(rootFolder.getChildFile("preset" + juce::String(presetNumber) + ".xml"))};

    if (xml != nullptr)
    {
        juce::Logger::writeToLog("XML content before parsing:\n" + xml->toString());

        juce::Logger::writeToLog(juce::String(xml->getStringAttribute("Gain")));
        juce::Logger::writeToLog(juce::String(xml->getStringAttribute("Gain").getFloatValue()));
        setGain(xml->getStringAttribute("Gain").getFloatValue());
        lfo.setFrequency(xml->getStringAttribute("LfoFrequency").getFloatValue());
        setLFOGainDepth(xml->getStringAttribute("LfoDepth").getFloatValue());
        osc.setType(xml->getStringAttribute("OscType").getIntValue());
        filter.setCutoffFrequency(xml->getStringAttribute("FilterCutOff").getFloatValue());
        filter.setResonance(xml->getStringAttribute("FilterResonance").getFloatValue());
        filter.selectFilterType(xml->getStringAttribute("FilterType").getIntValue());
        filter.setLFOFreq(xml->getStringAttribute("FilterLFOFreq").getFloatValue());
        filter.setLFODepth(xml->getStringAttribute("FilterLFODepth").getFloatValue());
        setWhitenoiseLevel(xml->getStringAttribute("whitenoiseLevel").getFloatValue());
        osc.setFmType(xml->getStringAttribute("FmType").getIntValue());
        osc.setFmDepth(xml->getStringAttribute("FmDepth").getFloatValue());
        osc.setFmFreq(xml->getStringAttribute("FmFreq").getFloatValue());
        osc.setLFOFreq(xml->getStringAttribute("OscLFOFreq").getFloatValue());
        osc.setLFODepth(xml->getStringAttribute("OscLFODepth").getFloatValue());
        reverbParams.roomSize = xml->getStringAttribute("Reverb").getFloatValue();
        delay.setDelay(xml->getStringAttribute("Delay").getIntValue());
        enableDelay = xml->getStringAttribute("EnabledDelay")  == "true" ? true : false;
        reverb.setEnabled(xml->getStringAttribute("EnabledReverb")  == "true" ? true : false);
        adsrParams.attack = xml->getStringAttribute("Attack").getFloatValue();
        adsrParams.decay = xml->getStringAttribute("Decay").getFloatValue();
        adsrParams.sustain = xml->getStringAttribute("Sustain").getFloatValue();
        adsrParams.release = xml->getStringAttribute("Release").getFloatValue();
        filterParams.attack = xml->getStringAttribute("AttackFilter").getFloatValue();
        filterParams.decay = xml->getStringAttribute("DecayFilter").getFloatValue();
        filterParams.sustain = xml->getStringAttribute("SustainFilter").getFloatValue();
        filterParams.release = xml->getStringAttribute("ReleaseFilter").getFloatValue();
        // deserializeParams(xml);
    }
    else
    {
        // Handle error reading XML...
        juce::Logger::writeToLog("Failed to Load");
    }
}
// void SynthVoice::deserializeParams(const juce::ValueTree &tree)
// {
//     juce::Logger::writeToLog(juce::String(tree.getNumProperties()));
//     // std::cout << tree.getPropertyName(0) << std::endl;
// }

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    juce::dsp::AudioBlock<float> audioBlock{outputBuffer};

    float lfoValue = lfo.processSample(0);

    float lfoMod = (lfoValue * lfoDepth) / 500;

    float finalGain = gainLevel;

    if (finalGain + lfoMod >= 0 && finalGain + lfoMod <= 1.0f)
    {
        finalGain += lfoMod;
    }

    gain.setGainLinear(finalGain);

    adsr.setParameters(adsrParams);
    adsrFilter.setParameters(filterParams);

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
            buffer[sample] += random.nextFloat() * whitenoiseLevel - 0.125f;
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
