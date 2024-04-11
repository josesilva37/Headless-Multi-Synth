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
        if (message.isPitchWheel())
        {
            if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
            {
                voice->pitchWheelMoved(message.getPitchWheelValue());
            }
        }
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
            int controllerValue = message.getControllerValue();
            int controllerNumber = message.getControllerNumber();

            juce::Logger::writeToLog("MIDI Controller Number: " + juce::String(controllerNumber));

            switch (controllerNumber)
            {
            case (113):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    if (controllerValue == 127)
                    {
                        int type = voice->getOscillator().getType();
                        if (type != 3)
                        {
                            type = type + 1;
                        }
                        else
                        {
                            type = 0;
                        }
                        switch (type)
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
                        case 3:
                            if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                            {
                                voice->getOscillator().setType(3);
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }

                break;
            case (115):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    if (controllerValue == 127)
                    {

                        int type = voice->getOscillator().getFmType();
                        if (type != 3)
                        {
                            type = type + 1;
                        }
                        else
                        {
                            type = 0;
                        }
                        switch (type)
                        {
                        case 0:
                            if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                            {
                                voice->getOscillator().setFmType(0);
                            }
                            break;
                        case 1:
                            if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                            {
                                voice->getOscillator().setFmType(1);
                            }
                            break;
                        case 2:
                            if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                            {
                                voice->getOscillator().setFmType(2);
                            }
                            break;
                        case 3:
                            if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                            {
                                voice->getOscillator().setFmType(3);
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }

                break;
            case (74):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    int minControllerValue = 0;
                    int maxControllerValue = 127;

                    double minModulation = 0.0f;
                    double maxModulation = 1.0f;

                    // Calculate the scaled frequency
                    double scaledGain = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                    voice->getOscillator().setGain(scaledGain);
                }
                break;
            case (72):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    int minControllerValue = 0;
                    int maxControllerValue = 127;

                    double minLevel = 0.0f;
                    double maxLevel = 1.0f;

                    double scaledLevel = minLevel + (maxLevel - minLevel) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                    voice->setReverbWetLevel(scaledLevel);
                }
                break;
            case (79):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    int minControllerValue = 0;
                    int maxControllerValue = 127;

                    double minLevel = 0;
                    double maxLevel = 44100 * 10;
                    double minLevel2 = 0;
                    double maxLevel2 = 10000;

                    double scaledLevel = minLevel + (maxLevel - minLevel) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                    double scaledFeedback = minLevel2 + (maxLevel2 - minLevel2) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                    voice->setDelay(scaledLevel, scaledFeedback);
                }
                break;
            case (77):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    juce::Logger::writeToLog("Entrei no changeAttack");

                    voice->changeAttack(controllerValue);
                }
                break;

            case (93):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    juce::Logger::writeToLog("Entrei no changeDecay");

                    voice->changeDecay(controllerValue);
                }
                break;

            case (73):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    juce::Logger::writeToLog("Entrei no changeSustain");

                    voice->changeSustain(controllerValue);
                }
                break;

            case (75):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    juce::Logger::writeToLog("Entrei no changeRelease");

                    voice->changeRelease(controllerValue);
                }
                break;
            case (1):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    int minControllerValue = 0;
                    int maxControllerValue = 127;

                    double minFrequency = 0.0f;
                    double maxFrequency = 1000.0f;

                    // Calculate the scaled frequency
                    double scaledFrequency = minFrequency + (maxFrequency - minFrequency) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                    juce::Logger::writeToLog("Entrei no changeFmFreq: " + juce::String(scaledFrequency));

                    voice->changeFmFreq(scaledFrequency);
                }
                break;
            case (16):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    int minControllerValue = 0;
                    int maxControllerValue = 127;

                    double minModulation = 0.0f;
                    double maxModulation = 10000.0f;

                    // Calculate the scaled frequency
                    double scaledModulation = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                    juce::Logger::writeToLog("Entrei no changeFmDepth: " + juce::String(scaledModulation));

                    voice->changeFmDepth(scaledModulation);
                }
                break;
            case (22):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {

                    int type = voice->getFilter().getFilterType();
                    juce::Logger::writeToLog(juce::String(type));

                    if (type != 2)
                    {
                        type = type + 1;
                    }
                    else
                    {
                        type = 0;
                    }
                    juce::Logger::writeToLog(juce::String(type));

                    switch (type)
                    {
                    case 0:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            voice->getFilter().selectFilterType(0);
                        }
                        break;
                    case 1:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            voice->getFilter().selectFilterType(1);
                        }
                        break;
                    case 2:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            voice->getFilter().selectFilterType(2);
                        }
                        break;

                    default:
                        break;
                    }
                }
                break;
            case (23):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    if (controllerValue == 127)
                    {
                        int LFOControl = voice->getLFOControl();
                        juce::Logger::writeToLog(juce::String(LFOControl));
                        if (LFOControl == 4)
                        {
                            voice->setLFOControl(0);
                            juce::Logger::writeToLog(juce::String(voice->getLFOControl()));
                        }
                        else
                        {
                            voice->setLFOControl(LFOControl + 1);
                            juce::Logger::writeToLog(juce::String(voice->getLFOControl()));
                        }
                        switch (voice->getLFOControl())
                        {
                        case 0:
                            voice->setLFOGainDepth(0);
                            voice->changeFilterLFODepth(0);
                            break;
                        case 1:
                            voice->changeFilterLFODepth(0);
                            voice->getOscillator().setLFODepth(0);
                            break;
                        case 2:
                            voice->setLFOGainDepth(0);
                            voice->getOscillator().setLFODepth(0);
                            break;
                        case 3:
                            voice->changeFilterLFODepth(0);
                            break;
                        default:
                            break;
                        }
                    }
                }
                break;
            case (71):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    int minControllerValue = 0;
                    int maxControllerValue = 127;

                    double minModulation = 20.0f;
                    double maxModulation = 1000.0f;

                    double scaledFrequency = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                    voice->getFilter().setFilterCutOffFrequency(scaledFrequency);
                }
                break;
            case (76):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    int minControllerValue = 0;
                    int maxControllerValue = 127;

                    double minModulation = 0.05f;
                    double maxModulation = 0.95f;

                    double scaledResonance = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                    voice->getFilter().setResonance(scaledResonance);
                }
                break;
            case (18):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    int minControllerValue = 0;
                    int maxControllerValue = 127;

                    double minModulation = 0.1f;
                    double maxModulation = 1000.0f;

                    double scaledValue = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                    int LFOControl = voice->getLFOControl();

                    switch (LFOControl)
                    {
                    case 0:
                        voice->getOscillator().setLFOFreq(scaledValue);
                        break;
                    case 1:
                        voice->setLFOGainFreq(scaledValue);
                        break;
                    case 2:
                        voice->changeFilterLFOFreq(scaledValue);
                        break;
                    case 3:
                        voice->getOscillator().setLFOFreq(scaledValue);
                        voice->setLFOGainFreq(scaledValue);
                        break;
                    case 4:
                        voice->getOscillator().setLFOFreq(scaledValue);
                        voice->setLFOGainFreq(scaledValue);
                        voice->changeFilterLFOFreq(scaledValue);
                        break;
                    default:
                        break;
                    }
                }
                break;
            case (19):
                if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                {
                    int minControllerValue = 0;
                    int maxControllerValue = 127;

                    double minModulation = 0.0f;
                    double maxModulation = 100.0f;
                    double scaledValue = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                    int LFOControl = voice->getLFOControl();

                    switch (LFOControl)
                    {
                    case 0:
                        voice->getOscillator().setLFODepth(scaledValue);
                        break;
                    case 1:
                        voice->setLFOGainDepth(scaledValue);
                        break;
                    case 2:
                        voice->changeFilterLFODepth(scaledValue);
                        break;
                    case 3:
                        voice->getOscillator().setLFODepth(scaledValue);
                        voice->setLFOGainDepth(scaledValue);
                        break;
                    case 4:
                        voice->getOscillator().setLFODepth(scaledValue);
                        voice->setLFOGainDepth(scaledValue);
                        voice->changeFilterLFODepth(scaledValue);
                        break;
                    default:
                        break;
                    }
                }
                break;
            default:
                break;
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
