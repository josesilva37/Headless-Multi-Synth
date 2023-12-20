#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
};
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    adsr.noteOn();
};
void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();
};
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {
    
};
void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {
    
};
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels){

    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec processSpec;
    processSpec.sampleRate = sampleRate;   // Set your desired sample rate
    processSpec.maximumBlockSize = samplesPerBlock; // Set your desired block size
    processSpec.numChannels = outputChannels;        // Set your desired number of channels

    processorChain.prepare(processSpec);
            
    auto &osc = processorChain.template get<oscIndex>();
    osc.setFrequency(440.0, false);
    auto &gain = processorChain.template get<gainIndex>();
    gain.setGainLinear(0.5); 


};

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    
    juce::dsp::AudioBlock<float> outputBlock{outputBuffer};
    juce::dsp::ProcessContextReplacing<float> context(outputBlock);
    processorChain.process(context); 

    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);

};