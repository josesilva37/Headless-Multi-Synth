#include <SynthVoice.h>

bool SynthVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
};
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    
};
void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    
};
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {
    
};
void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {
    
};
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels){
    // Set up the audio processing specifications
    juce::dsp::ProcessSpec processSpec;
    processSpec.sampleRate = sampleRate;   // Set your desired sample rate
    processSpec.maximumBlockSize = samplesPerBlock; // Set your desired block size
    processSpec.numChannels = outputChannels;        // Set your desired number of channels

    processorChain.prepare(spec);
    
    // osc.setFrequency(440.0, true); // Set the frequency to 440 Hz
    // osc.setLevel(0.5);             
    force = false;
    auto &osc = processorChain.template get<oscIndex>();
    osc.setFrequency(440.0, force); // [7]
    auto &gain = processorChain.template get<gainIndex>();
    gain.setGainLinear(0.5); // [8]
};

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    
    juce::dsp::AudioBlock<float> outputBlock{outputBuffer};
    juce::dsp::ProcessContextReplacing<float> context(outputBlock);
    processorChain.process(context); 


};