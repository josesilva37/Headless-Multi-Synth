#include "SynthVoice.h"
#include "Data/WavetableOscillator.h"

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
    case 2:
        whitenoiseParams.attack = attackValue;
        break;
    }
};
void SynthVoice::changeDecay(float decayValue)
{
    switch (ADSRControl)
    {
    case 0:
        adsrParams.decay = decayValue;
        break;
    case 1:
        filterParams.decay = decayValue;
        break;
    case 2:
        whitenoiseParams.decay = decayValue;
        break;
    }
};
void SynthVoice::changeSustain(float sustainValue)
{

    switch (ADSRControl)
    {
    case 0:
        adsrParams.sustain = sustainValue;
        break;
    case 1:
        filterParams.sustain = sustainValue;
        break;
    case 2:
        whitenoiseParams.sustain = sustainValue;
        break;
    }
};
void SynthVoice::changeRelease(float releaseValue)
{
    switch (ADSRControl)
    {
    case 0:
        adsrParams.release = releaseValue;
        break;
    case 1:
        filterParams.release = releaseValue;
        break;
    case 2:
        whitenoiseParams.release = releaseValue;
        break;
    }
};
void SynthVoice::setGain(float level)
{
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
    if (isWavetableOn)
    {

        for (auto oscillatorIndex = 0; oscillatorIndex < oscillatorsSin.size(); ++oscillatorIndex)
        {
            int midiNoteIncreased;
            switch (frequencySpacing)
            {
            case 0:
                midiNoteIncreased = midiNoteNumber + oscillatorIndex;
                break;
            case 1:
                midiNoteIncreased = midiNoteNumber + (5 * oscillatorIndex);
                break;
            case 2:
                midiNoteIncreased = midiNoteNumber + (7 * oscillatorIndex);
                break;
            case 3:
                midiNoteIncreased = midiNoteNumber + (12 * oscillatorIndex);
                break;
            }
            if (midiNoteIncreased <= 127)
            {
                juce::Logger::writeToLog(juce::String(midiNoteIncreased));
                switch (oscillatorHarmonics)
                {
                case 0:
                    oscillatorsSin[oscillatorIndex]->setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteIncreased), getSampleRate());
                    break;
                case 1:
                    oscillatorsSinBalance[oscillatorIndex]->setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteIncreased), getSampleRate());
                    break;
                case 2:
                    oscillatorsSinOdd[oscillatorIndex]->setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteIncreased), getSampleRate());
                    break;
                case 3:
                    oscillatorsSinEven[oscillatorIndex]->setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteIncreased), getSampleRate());
                    break;
                case 4:
                    oscillatorsSinIncreasing[oscillatorIndex]->setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteIncreased), getSampleRate());
                    break;
                case 5:
                    oscillatorsSinDecreasing[oscillatorIndex]->setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteIncreased), getSampleRate());
                    break;
                case 6:
                    oscillatorsSinSparse[oscillatorIndex]->setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteIncreased), getSampleRate());
                    break;
                }
            }
        }
    }
    else
    {
        osc.setWaveFrequency(midiNoteNumber);
    }
    adsrFilter.noteOn();
    adsr.noteOn();
    adsrWhiteNoise.noteOn();
};
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsrWhiteNoise.noteOff();
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
    whitenoiseLevel = level;
}
void SynthVoice::createWaveTable()
{
    // Wavetables Initialization
    waveTableSin.setSize(1, (int)tableSize + 1);
    waveTableSin.clear();
    waveTableSinBalanced.setSize(1, (int)tableSize + 1);
    waveTableSinBalanced.clear();
    waveTableSinEven.setSize(1, (int)tableSize + 1);
    waveTableSinEven.clear();
    waveTableSinOdd.setSize(1, (int)tableSize + 1);
    waveTableSinOdd.clear();
    waveTableSinIncreasing.setSize(1, (int)tableSize + 1);
    waveTableSinIncreasing.clear();
    waveTableSinDecreasing.setSize(1, (int)tableSize + 1);
    waveTableSinDecreasing.clear();
    waveTableSinSparse.setSize(1, (int)tableSize + 1);
    waveTableSinSparse.clear();

    // Harmonics
    int balancedHarmonics[] = {1, 2, 3, 4, 5, 6, 7, 8};
    float balancedHarmonicWeights[] = {0.2f, 0.2f, 0.2f, 0.1f, 0.1f, 0.05f, 0.05f, 0.01f};
    int evenHarmonics[] = {2, 4, 6, 8, 10, 12, 14, 16};
    float evenHarmonicWeights[] = {0.5f, 0.2f, 0.1f, 0.05f, 0.03f, 0.02f, 0.01f, 0.005f};
    int oddHarmonics[] = {1, 3, 5, 7, 9, 11, 13, 15};
    float oddHarmonicWeights[] = {0.3f, 0.2f, 0.15f, 0.1f, 0.08f, 0.05f, 0.04f, 0.03f};
    int increasingHarmonics[] = {1, 2, 3, 4, 5, 6, 7, 8};
    float increasingHarmonicWeights[] = {0.1f, 0.15f, 0.2f, 0.25f, 0.3f, 0.35f, 0.4f, 0.45f};
    int decreasingHarmonics[] = {1, 2, 3, 4, 5, 6, 7, 8};
    float decreasingHarmonicWeights[] = {0.4f, 0.3f, 0.2f, 0.1f, 0.05f, 0.03f, 0.02f, 0.01f};
    int sparceHarmonics[] = {1, 3, 7, 13, 19, 25, 31, 37};
    float sparseHarmonicWeights[] = {0.5f, 0.2f, 0.1f, 0.05f, 0.03f, 0.02f, 0.01f, 0.005f};

    // Wavetable buffer
    auto *samplesSin = waveTableSin.getWritePointer(0);
    auto *samplesSinBalanced = waveTableSinBalanced.getWritePointer(0);
    auto *samplesSinOdd = waveTableSinOdd.getWritePointer(0);
    auto *samplesSinEven = waveTableSinEven.getWritePointer(0);
    auto *samplesSinIncreasing = waveTableSinIncreasing.getWritePointer(0);
    auto *samplesSinDecreasing = waveTableSinDecreasing.getWritePointer(0);
    auto *samplesSinSparse = waveTableSinSparse.getWritePointer(0);

    // Simple Sin Wavetable
    auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
    auto currentAngle = 0.0;

    for (unsigned int i = 0; i < tableSize; ++i)
    {
        auto sample = std::sin(currentAngle);
        samplesSin[i] += (float)sample;
        currentAngle += angleDelta;
    }

    // Sin Wavetables With Changed Harmonics
    for (auto harmonic = 0; harmonic < juce::numElementsInArray(evenHarmonics); ++harmonic)
    {
        auto angleDeltaBalanced = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * balancedHarmonics[harmonic];
        auto angleDeltaOdd = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * oddHarmonics[harmonic];
        auto angleDeltaEven = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * evenHarmonics[harmonic];
        auto angleDeltaIncreasing = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * increasingHarmonics[harmonic];
        auto angleDeltaDecreasing = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * decreasingHarmonics[harmonic];
        auto angleDeltaSparse = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * sparceHarmonics[harmonic];

        auto currentAngleBalanced = 0.0;
        auto currentAngleOdd = 0.0;
        auto currentAngleEven = 0.0;
        auto currentAngleIncreased = 0.0;
        auto currentAngleDecreased = 0.0;
        auto currentAngleSparse = 0.0;

        for (unsigned int i = 0; i < tableSize; ++i)
        {
            auto sampleSinBalanced = 0.0f;
            auto sampleSinOdd = 0.0f;
            auto sampleSinEven = 0.0f;
            auto sampleSinIncreased = 0.0f;
            auto sampleSinDecreased = 0.0f;
            auto sampleSinSparse = 0.0f;

            for (int table = 0; table < 6; table++)
            {
                switch (table)
                {
                case 0:
                    sampleSinBalanced = std::sin(currentAngleBalanced);
                    juce::Logger::writeToLog(juce::String(sampleSinBalanced));
                    samplesSinBalanced[i] += (float)sampleSinBalanced * balancedHarmonicWeights[harmonic];
                    currentAngleBalanced += angleDeltaBalanced;
                    break;
                case 1:
                    sampleSinOdd = std::sin(currentAngleOdd);
                    samplesSinOdd[i] += (float)sampleSinOdd * oddHarmonicWeights[harmonic];
                    currentAngleOdd += angleDeltaOdd;
                    break;
                case 2:
                    sampleSinEven = std::sin(currentAngleEven);
                    samplesSinEven[i] += (float)sampleSinEven * evenHarmonicWeights[harmonic];
                    currentAngleEven += angleDeltaEven;
                    break;
                case 3:
                    sampleSinIncreased = std::sin(currentAngleIncreased);
                    samplesSinIncreasing[i] += (float)sampleSinIncreased * increasingHarmonicWeights[harmonic];
                    currentAngleIncreased += angleDeltaIncreasing;
                    break;
                case 4:
                    sampleSinDecreased = std::sin(currentAngleDecreased);
                    samplesSinDecreasing[i] += (float)sampleSinDecreased * decreasingHarmonicWeights[harmonic];
                    currentAngleDecreased += angleDeltaDecreasing;
                    break;
                case 5:
                    sampleSinSparse = std::sin(currentAngleSparse);
                    samplesSinSparse[i] += (float)sampleSinSparse * sparseHarmonicWeights[harmonic];
                    currentAngleSparse += angleDeltaSparse;
                    break;
                default:
                    break;
                }
            }
        }
    }
    // State the last sample to equals the last sample
    samplesSin[tableSize] = samplesSin[0];
    samplesSinBalanced[tableSize] = samplesSinBalanced[0];
    samplesSinOdd[tableSize] = samplesSinOdd[0];
    samplesSinEven[tableSize] = samplesSinEven[0];
    samplesSinIncreasing[tableSize] = samplesSinIncreasing[0];
    samplesSinDecreasing[tableSize] = samplesSinDecreasing[0];
    samplesSinSparse[tableSize] = samplesSinSparse[0];
}
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    createWaveTable();
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
    whitenoiseParams.release = 0.4f;
    limiter.prepare(spec);
    limiter.setThreshold(0.5f);

    auto numberOfOscillators = 10;
    for (auto i = 0; i < numberOfOscillators; ++i)
    {
        auto *oscillatorSin = new WavetableOscillator(waveTableSin);
        oscillatorsSin.add(oscillatorSin);
        auto *oscillatorBalanced = new WavetableOscillator(waveTableSinBalanced);
        oscillatorsSinBalance.add(oscillatorBalanced);
        auto *oscillatorOdd = new WavetableOscillator(waveTableSinOdd);
        oscillatorsSinOdd.add(oscillatorOdd);
        auto *oscillatorEven = new WavetableOscillator(waveTableSinEven);
        oscillatorsSinEven.add(oscillatorEven);
        auto *oscillatorIncreasing = new WavetableOscillator(waveTableSinIncreasing);
        oscillatorsSinIncreasing.add(oscillatorIncreasing);
        auto *oscillatorDecreasing = new WavetableOscillator(waveTableSinDecreasing);
        oscillatorsSinDecreasing.add(oscillatorDecreasing);
        auto *oscillatorSparse = new WavetableOscillator(waveTableSinSparse);
        oscillatorsSinSparse.add(oscillatorSparse);
    }

    wavetableLevel = 0.25f / (float)numberOfOscillators;
};

void SynthVoice::setOscillatorHarmonics()
{
    if (oscillatorHarmonics == 6)
    {
        oscillatorHarmonics = 0;
    }
    else
    {
        oscillatorHarmonics++;
    }
}
void SynthVoice::setFrequencySpacing(){
    if(frequencySpacing == 3){
        frequencySpacing = 0;
    }else{
        frequencySpacing++;
    }
}
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
    tree.setProperty("AttackWhitenoise", adsrWhiteNoise.getParameters().attack, nullptr);
    tree.setProperty("DecayWhitenoise", adsrWhiteNoise.getParameters().decay, nullptr);
    tree.setProperty("SustainWhitenoise", adsrWhiteNoise.getParameters().sustain, nullptr);
    tree.setProperty("ReleaseWhitenoise", adsrWhiteNoise.getParameters().release, nullptr);
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
    tree.setProperty("EnabledReverb", reverb.isEnabled() ? "true" : "false", nullptr);
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
        enableDelay = xml->getStringAttribute("EnabledDelay") == "true" ? true : false;
        reverb.setEnabled(xml->getStringAttribute("EnabledReverb") == "true" ? true : false);
        adsrParams.attack = xml->getStringAttribute("Attack").getFloatValue();
        adsrParams.decay = xml->getStringAttribute("Decay").getFloatValue();
        adsrParams.sustain = xml->getStringAttribute("Sustain").getFloatValue();
        adsrParams.release = xml->getStringAttribute("Release").getFloatValue();
        filterParams.attack = xml->getStringAttribute("AttackFilter").getFloatValue();
        filterParams.decay = xml->getStringAttribute("DecayFilter").getFloatValue();
        filterParams.sustain = xml->getStringAttribute("SustainFilter").getFloatValue();
        filterParams.release = xml->getStringAttribute("ReleaseFilter").getFloatValue();
        whitenoiseParams.attack = xml->getStringAttribute("AttackWhitenoise").getFloatValue();
        whitenoiseParams.decay = xml->getStringAttribute("DecayWhitenoise").getFloatValue();
        whitenoiseParams.sustain = xml->getStringAttribute("SustainWhitenoise").getFloatValue();
        whitenoiseParams.release = xml->getStringAttribute("ReleaseWhitenoise").getFloatValue();

        // deserializeParams(xml);
    }
    else
    {
        // Handle error reading XML...
        juce::Logger::writeToLog("Failed to Load");
    }
}

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
    adsrWhiteNoise.setParameters(whitenoiseParams);

    if (isWavetableOn)
    {
        auto *leftBuffer = outputBuffer.getWritePointer(0);
        auto *rightBuffer = outputBuffer.getWritePointer(1);

        for (auto oscillatorIndex = 0; oscillatorIndex < oscillatorsSin.size(); ++oscillatorIndex)
        {
            auto *oscillator = oscillatorsSin.getUnchecked(oscillatorIndex);
            switch (oscillatorHarmonics)
            {
            case 1:
                oscillator = oscillatorsSinBalance.getUnchecked(oscillatorIndex);
                break;
            case 2:
                oscillator = oscillatorsSinOdd.getUnchecked(oscillatorIndex);
                break;
            case 3:
                oscillator = oscillatorsSinEven.getUnchecked(oscillatorIndex);
                break;
            case 4:
                oscillator = oscillatorsSinIncreasing.getUnchecked(oscillatorIndex);
                break;
            case 5:
                oscillator = oscillatorsSinDecreasing.getUnchecked(oscillatorIndex);
                break;
            case 6:
                oscillator = oscillatorsSinSparse.getUnchecked(oscillatorIndex);
                break;
            }

            for (auto sample = 0; sample < outputBuffer.getNumSamples(); ++sample)
            {
                auto levelSample = oscillator->getNextSample() * wavetableLevel;

                leftBuffer[sample] += levelSample;
                rightBuffer[sample] += levelSample;
            }
        }
    }
    else
    {
        osc.getNextAudioBlock(outputBuffer);
    }

    adsrWhiteNoise.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());
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
            buffer[sample] += random.nextFloat() * (whitenoiseLevel * adsrWhiteNoise.getNextSample()) - 0.125f;
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
