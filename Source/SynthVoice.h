#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/OscData.h"
#include "Data/FilterData.h"
#include "Data/WavetableOscillator.h"

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
    void setGain(float value);
    void changeAttack(float attackValue);
    void changeDecay(float decayValue);
    void changeSustain(float sustainValue);
    void changeRelease(float releaseValue);
    void changeFmDepth(float depth);
    void changeFmFreq(float freq);
    void changeFilterCutOffFreq(float freq);
    void changeFilterType(int type);
    void changeFilterResonance(float resonance);
    void changeFilterLFODepth(float level);
    void changeFilterLFOFreq(float level);
    void setReverbWetLevel(float level);
    void setDelay(float delay, float feedback);
    void setLFOGainDepth(float level);
    void setLFOGainFreq(float level);
    int getLFOControl() { return LFOControl; };
    int getADSRControl() { return ADSRControl; };
    void setLFOControl(int type);
    void setADSRControl(int type);
    void resetSynthParams();
    void setWhitenoiseLevel(float level);
    FilterData &getFilter() { return filter; };
    OscData &getOscillator() { return osc; };
    juce::ValueTree serialize();
    void savePreset(int presetNumber);
    void loadPreset(int presetNumber);
    void createWaveTable();
    void setOscillatorHarmonics();
    void setFrequencySpacing();
    void setSynthesisMode();
    void setButtonsMode();
    int getButtonsMode(){return buttonsMode;};
    void setMixChorus(float value);
private:
    OscData osc;
    juce::AudioBuffer<float> synthBuffer;
    juce::dsp::Gain<float> gain;
    juce::ADSR adsr;
    juce::ADSR adsrFilter;
    juce::ADSR adsrWhiteNoise;
    juce::ADSR::Parameters whitenoiseParams;
    juce::ADSR::Parameters adsrParams;
    juce::ADSR::Parameters filterParams;
    FilterData filter;
    juce::dsp::Oscillator<float> lfo{[](float x)
                                     { return std::sin(x); }};
    float lfoMod;
    float lfoDepth = 0.0f;
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delay;
    juce::dsp::Chorus<float> chorus;
    float chorusMix = 0;
    bool enableDelay = false;
    juce::dsp::Limiter<float> limiter;
    float feedbackAttenuation = 0.7f;
    float delayFeedback = 1.0f;
    int LFOControl = 0;
    int ADSRControl = 0;
    juce::Random random;
    float whitenoiseLevel = 0;
    float gainLevel;
    float wavetableLevel = 0.0f;
    juce::OwnedArray<WavetableOscillator> oscillatorsSin;
    juce::OwnedArray<WavetableOscillator> oscillatorsSinBalance;
    juce::OwnedArray<WavetableOscillator> oscillatorsSinOdd;
    juce::OwnedArray<WavetableOscillator> oscillatorsSinEven;
    juce::OwnedArray<WavetableOscillator> oscillatorsSinIncreasing;
    juce::OwnedArray<WavetableOscillator> oscillatorsSinDecreasing;
    juce::OwnedArray<WavetableOscillator> oscillatorsSinSparse;
    const unsigned int tableSize = 1 << 7;
    juce::AudioSampleBuffer waveTableSin;
    juce::AudioSampleBuffer waveTableSinBalanced;
    juce::AudioSampleBuffer waveTableSinEven;
    juce::AudioSampleBuffer waveTableSinOdd;
    juce::AudioSampleBuffer waveTableSinIncreasing;
    juce::AudioSampleBuffer waveTableSinDecreasing;
    juce::AudioSampleBuffer waveTableSinSparse;
    int oscillatorHarmonics = 0;
    bool isWavetableOn = false;
    int frequencySpacing = 0;
    int buttonsMode = 0;
    int synthesisMode =0;
};