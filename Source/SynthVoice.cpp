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
void SynthVoice::changeFilterType(int type){
    filter.selectFilterType(type);
}
void SynthVoice::changeFilterCutOffFreq(float cutofffreq){
    filter.setCutoffFrequency(cutofffreq);
}

void SynthVoice::changeFilterResonance(float resonsance){
    filter.setResonance(resonsance);
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
void SynthVoice::pitchWheelMoved(int newPitchWheelValue){

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
    gain.setGainLinear(0.1f);
};

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    juce::dsp::AudioBlock<float> audioBlock{outputBuffer};
    osc.getNextAudioBlock(audioBlock);

    float lfoValue = lfo.processSample(0); // Assuming LFO is mono
    float lfoMod = (lfoValue * lfoDepth)/100;

    juce::Logger::writeToLog(juce::String(lfoMod) + "-LFO -Gain " + juce::String(osc.getGain()));

    float finalGain = osc.getGain(); 
    if (finalGain + lfoMod >= 0 && finalGain + lfoMod <= 1.0f)
    {
        finalGain += lfoMod; 
    }

    juce::Logger::writeToLog(juce::String(finalGain));
    gain.setGainLinear(finalGain);

    
    filter.processNextBlock(outputBuffer);
    adsr.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
};