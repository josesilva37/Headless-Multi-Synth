#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

class MyAudioIODeviceCallback : public juce::AudioIODeviceCallback,
                                public juce::MidiInputCallback
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
        juce::MidiBuffer midiMessages;
        juce::AudioBuffer<float> buffer(outputChannelData, numOutputChannels, numSamples);

        mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    }
    void handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message) override
    {
        // Handle incoming MIDI message
        juce::Logger::writeToLog("Received MIDI message: " + message.getDescription());

        if (message.isNoteOn())
        {
            int noteNumber = message.getNoteNumber();
            float velocity = message.getVelocity();

            // Trigger a note in your synth
            mySynth.noteOn(1, noteNumber, velocity);
        }
        else if (message.isNoteOff())
        {
            int noteNumber = message.getNoteNumber();
            float velocity = message.getVelocity();

            // Release the corresponding note in your synth
            mySynth.noteOff(1, noteNumber, velocity, false);
        }
        else if (message.isController())
        {
            // Handle Control Change message
            int controllerValue = message.getControllerValue();
            // Your code here for Control Change handling
            int controllerNumber = message.getControllerNumber();
            // Value
            // std::string controllerValueString = std::to_string(controllerValue);
            // juce::Logger::writeToLog(controllerValueString);
            // juce::Logger::writeToLog("MIDI Controller Number: " + juce::String(controllerNumber));
            switch (controllerNumber)
            {
            case (127):
                switch (controllerValue)
                {
                case 0:
                    if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                    {
                        voice->getOscillator().setType(0);
                    }
                    break;
                case 1:
                    if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                    {
                        voice->getOscillator().setType(1);
                    }
                    break;
                case 2:
                    if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                    {
                        voice->getOscillator().setType(2);
                    }
                    break;

                default:
                    break;
                }
            case (126):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    voice->getOscillator().setGain(controllerValue);
                }
            case (36):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    voice->changeAttack(controllerValue);
                }
            case (37):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    voice->changeDecay(controllerValue);
                }
            case (38):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    voice->changeRelease(controllerValue);
                }
            case (39):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    voice->changeSustain(controllerValue);
                }
            }
        }
    };

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
    int lastInputIndex = 0;
    synth.addSound(new SynthSound());

    synth.addVoice(new SynthVoice());

    // for (int i = 0; i < 5; i++)
    // {
    //     synth.addVoice (new SynthVoice());
    // }

    synth.setCurrentPlaybackSampleRate(44100);

    if (auto voice = dynamic_cast<SynthVoice *>(synth.getVoice(0)))
    {
        // voice->getOscillator().setType(0);
        voice->prepareToPlay(44100.0, 512, 2);
    }
    // for (int i = 0; i < synth.getNumVoices(); i++)
    // {
    //     if (auto voice = dynamic_cast<SynthVoice *>(synth.getVoice(i)))
    //     {
    //         voice->prepareToPlay(44100.0, 512, 2);
    //     }
    // }

    MyAudioIODeviceCallback audioIODeviceCallback(synth);
    // Add the audio device callback
    devmgr.addAudioCallback(&audioIODeviceCallback);
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    for (auto input : midiInputs)
    {
        juce::Logger::writeToLog(input.identifier);
    }
    devmgr.setMidiInputDeviceEnabled(midiInputs[1].identifier, true);
    devmgr.addMidiInputDeviceCallback(midiInputs[1].identifier, &audioIODeviceCallback);

    juce::Logger::writeToLog(devmgr.getDefaultMidiOutputIdentifier());

    devmgr.restartLastAudioDevice();
    // Run the message loop
    while (true)
    {
        juce::Thread::sleep(100);
    }

    // Stop the audio device and remove the callback when exiting the application
    devmgr.removeAudioCallback(&audioIODeviceCallback);
    devmgr.closeAudioDevice();

    return 0;
}
