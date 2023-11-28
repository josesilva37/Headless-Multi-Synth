#include <JuceHeader.h>
#include "CarrOsc.h"

class MyAudioIODeviceCallback : public juce::AudioIODeviceCallback
{
public:
    MyAudioIODeviceCallback(CarrOsc &oscillator) : myOscillator(oscillator) {}

     void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
                                          int numInputChannels,
                                          float* const* outputChannelData,
                                          int numOutputChannels,
                                          int numSamples,
                                          const juce::AudioIODeviceCallbackContext& callbackContext) override
    {
        // Your audio processing logic here...
        // Call the oscillator to generate audio samples
        juce::dsp::AudioBlock<float> outputBlock(outputChannelData, numOutputChannels, numSamples);
        juce::dsp::ProcessContextReplacing<float> context(outputBlock);
        myOscillator.process(context);
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
    CarrOsc &myOscillator;
};

int main()
{
    juce::AudioDeviceManager devmgr;
    devmgr.initialiseWithDefaultDevices(0, 2);
    juce::AudioIODevice *device = devmgr.getCurrentAudioDevice();
    // Create an instance of CarrOsc
    CarrOsc osc;

    // Set up the audio processing specifications
    juce::dsp::ProcessSpec processSpec;
    processSpec.sampleRate = 44100.0;   // Set your desired sample rate
    processSpec.maximumBlockSize = 512; // Set your desired block size
    processSpec.numChannels = 2;        // Set your desired number of channels

    // Initialize the oscillator
    osc.prepare(processSpec);

    osc.setFrequency(440.0, true); // Set the frequency to 440 Hz
    osc.setLevel(0.5);             // Set the level to 0.5

    MyAudioIODeviceCallback audioIODeviceCallback(osc);
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
