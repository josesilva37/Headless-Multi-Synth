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

    parameters.attack = attackValue/100;
    
    adsr.setParameters(parameters);
};
void SynthVoice::changeDecay(float decayValue)
{
    juce::ADSR::Parameters parameters = adsr.getParameters();
    parameters.decay = decayValue/100;
    juce::Logger::writeToLog("ADSR Parameters: ");
    juce::Logger::writeToLog("Attack: " + juce::String(parameters.attack));
    juce::Logger::writeToLog("Decay: " + juce::String(parameters.decay));
    juce::Logger::writeToLog("Sustain: " + juce::String(parameters.release));
    juce::Logger::writeToLog("Release: " + juce::String(parameters.sustain));
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
    osc.setWaveFrequency(midiNoteNumber);
    juce::Logger::writeToLog("ADSR Parameters: ");
    juce::Logger::writeToLog("Attack: " + juce::String(adsr.getParameters().attack));
    juce::Logger::writeToLog("Decay: " + juce::String(adsr.getParameters().decay));
    juce::Logger::writeToLog("Sustain: " + juce::String(adsr.getParameters().sustain));
    juce::Logger::writeToLog("Release: " + juce::String(adsr.getParameters().release));
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
    
    osc.prepareToPlay(spec);
    osc.setType(0);
    gain.prepare(spec);
    gain.setGainLinear(0.5f);
};

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    juce::dsp::AudioBlock<float> audioBlock{outputBuffer};
    osc.getNextAudioBlock(audioBlock);
    // juce::Logger::writeToLog(juce::String(gain.getGainDecibels()));
    
    if (osc.isGainSet())
    {
        gain.setGainLinear(osc.getGain());
    }
    // juce::Logger::writeToLog("Attack: " + juce::String(adsr.getParameters().attack));
    // juce::Logger::writeToLog("Decay: " + juce::String(adsr.getParameters().decay));
    // juce::Logger::writeToLog("Sustain: " + juce::String(adsr.getParameters().sustain));
    // juce::Logger::writeToLog("Release: " + juce::String(adsr.getParameters().release));

    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());
};