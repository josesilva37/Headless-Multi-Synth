#include <JuceHeader.h>
#include "CarrOsc.h"
#include "SynthVoice.h"
#include "SynthSound.h"

class MyAudioIODeviceCallback : public juce::AudioIODeviceCallback
{
public:
    MyAudioIODeviceCallback(juce::Synthesiser &synth) : mySynth(synth) {}

    void audioDeviceIOCallbackWithContext(const float *const *inputChannelData,
                                          int numInputChannels,
                                          float *const *outputChannelData,
                                          int numOutputChannels,
                                          int numSamples,
                                          const juce::AudioIODeviceCallbackContext &callbackContext) override
    {
        // myOscillator.process(context);
        // synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
        juce::MidiBuffer midiMessages;
        juce::AudioBuffer<float> buffer(outputChannelData, numOutputChannels, numSamples);
        // Render the next block using the synthesizer
        mySynth.renderNextBlock(buffer, midiMessages, 0, numSamples);
    }

    void audioDeviceAboutToStart(juce::AudioIODevice *device) override
    {
        // This method is called when the audio device is about to start
    }

    void audioDeviceStopped() override
    {
        // This method is called when the audio device is stopped
    }

private:
    juce::Synthesiser &mySynth;
};

int main()
{
    juce::Synthesiser synth;
    juce::AudioDeviceManager devmgr;
    devmgr.initialiseWithDefaultDevices(0, 2);
    juce::AudioIODevice *device = devmgr.getCurrentAudioDevice();

    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());

    if (auto voice = dynamic_cast<SynthVoice *>(synth.getVoice(0)))
    {
        voice->prepareToPlay(44100.0, 512, 2);
    }


    MyAudioIODeviceCallback audioIODeviceCallback(synth);
    // Add the audio device callback
    devmgr.addAudioCallback(&audioIODeviceCallback);

    // Start the audio device
    devmgr.restartLastAudioDevice();

    // Run the message loop
    while (true)
    {

        // Sleep to prevent high CPU usage
        juce::Thread::sleep(100);
    }

    // Stop the audio device and remove the callback when exiting the application
    devmgr.removeAudioCallback(&audioIODeviceCallback);
    devmgr.closeAudioDevice();

    return 0;
}
