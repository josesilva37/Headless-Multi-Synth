#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/OscData.h"
#include "Data/FilterData.h"

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
    void changeAttack(float attackValue);
    void changeDecay(float decayValue);
    void changeSustain(float sustainValue);
    void changeRelease(float releaseValue);
    void changeFmDepth(float depth);
    void changeFmFreq(float freq);
    void changeFilterCutOffFreq(float freq);
    void changeFilterType(int type);
    void changeFilterResonance(float resonance);
    void setReverbWetLevel(float level);
    FilterData &getFilter() { return filter; };
    OscData &getOscillator() { return osc; };

private:
    OscData osc;
    juce::AudioBuffer<float> synthBuffer;
    juce::dsp::Gain<float> gain;
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    FilterData filter;
    juce::dsp::Oscillator<float> lfo{[](float x)
                                     { return std::sin(x); }};
    float lfoMod;
    float lfoDepth = 0.0f;
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;


};