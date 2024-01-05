#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/OscData.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound *sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    OscData& getOscillator() {return osc;};
private:

    OscData osc;
    // juce::dsp::Oscillator<float> osc{[](float x)
    //                                  { return x < 0.0f ? -1.0f : 1.0f; },
    //                                  200};
    juce::dsp::Gain<float> gain;
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
};