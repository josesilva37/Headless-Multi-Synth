#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound *sound)
{
    return dynamic_cast<juce::SynthesiserSound *>(sound) != nullptr;
};

void SynthVoice::changeAttack(float attackValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    juce::Logger::writeToLog("Attack1: " + juce::String(adsr.getParameters().attack));
    juce::Logger::writeToLog("Decay1: " + juce::String(adsr.getParameters().decay));

    parameters.attack = attackValue/100;
    juce::Logger::writeToLog("Attack2: " + juce::String(adsr.getParameters().attack));
    juce::Logger::writeToLog("Decay2: " + juce::String(adsr.getParameters().decay));

    adsr.setParameters(parameters);
     juce::Logger::writeToLog("Attack3: " + juce::String(adsr.getParameters().attack));
    juce::Logger::writeToLog("Decay3: " + juce::String(adsr.getParameters().decay));

};
void SynthVoice::changeDecay(float decayValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.decay = decayValue/100;
    adsr.setParameters(parameters);
};
void SynthVoice::changeSustain(float sustainValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.sustain = sustainValue/100;
    adsr.setParameters(parameters);
};
void SynthVoice::changeRelease(float releaseValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.release = releaseValue/100;
    adsr.setParameters(parameters);
};
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    juce::Logger::writeToLog("ADSR Parameters: ");
    juce::Logger::writeToLog("Attack: " + juce::String(adsr.getParameters().attack));
    juce::Logger::writeToLog("Decay: " + juce::String(adsr.getParameters().decay));
    juce::Logger::writeToLog("Sustain: " + juce::String(adsr.getParameters().release));
    juce::Logger::writeToLog("Release: " + juce::String(adsr.getParameters().sustain));
    adsr.noteOn();
};
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
};
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue){

};
void SynthVoice::pitchWheelMoved(int newPitchWheelValue){

};
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{

    adsr.setSampleRate(sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc.prepare(spec);
    osc.setType(0);
    gain.prepare(spec);
    gain.setGainLinear(0.5f);
};

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    juce::dsp::AudioBlock<float> audioBlock{outputBuffer};
    osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    // juce::Logger::writeToLog(juce::String(gain.getGainDecibels()));

    if (osc.isGainSet())
    {
        gain.setGainLinear(osc.getGain());
    }
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());
};