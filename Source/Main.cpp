#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

class MyAudioIODeviceCallback : public juce::AudioIODeviceCallback,
                                public juce::MidiInputCallback
{
public:
    MyAudioIODeviceCallback(juce::Synthesiser &synth) : mySynth(synth), tree("MidiMappings") {}

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
        // juce::Logger::writeToLog("Received MIDI message: " + message.getDescription());

        if (midiLearningMode)
        {
            if (message.isController() && message.getControllerValue() > 0 && message.getControllerNumber() != lastControllerValue)
            {
                lastControllerValue = message.getControllerNumber();
                juce::Logger::writeToLog("Received function and MIDI controller. Proceed with the next step.");
                juce::Logger::writeToLog("Controller Number: " + juce::String(message.getControllerNumber()));

                tree.setProperty(juce::String(functionsArray[count]), message.getControllerNumber(), nullptr);

                count++;
                if (count <= 15)
                {
                    juce::Logger::writeToLog("Move the choosen knob controller for function: " + juce::String(functionsArray[count]));
                }
                else if (count < 25)
                {
                    juce::Logger::writeToLog("Move the choosen button controller for function: " + juce::String(functionsArray[count]));
                }
                else
                {
                    juce::File rootFolder = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory);

                    std::unique_ptr<juce::XmlElement> xml(tree.createXml());

                    if (xml != nullptr)
                    {
                        juce::File fileToSave = rootFolder.getChildFile("MIDI.xml");
                        juce::Logger::writeToLog("Saving MIDI Mapping to: " + fileToSave.getFullPathName());

                        if (xml->writeTo(fileToSave, {}))
                        {
                            juce::Logger::writeToLog("Midi Mapping saved successfully");
                        }
                        else
                        {
                            juce::Logger::writeToLog("Error saving mapping");
                        }
                    }
                }
            }
        }
        else
        {
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

                mySynth.noteOn(1, noteNumber, velocity);
            }
            else if (message.isNoteOff())
            {
                int noteNumber = message.getNoteNumber();
                float velocity = message.getVelocity();

                mySynth.noteOff(1, noteNumber, velocity, false);
            }
            else if (message.isController())
            {
                int controllerValue = message.getControllerValue();
                int controllerNumber = message.getControllerNumber();

                juce::Logger::writeToLog("MIDI Controller Number: " + juce::String(controllerNumber));
                auto it = midiMapping.find(controllerNumber);
                if (it != midiMapping.end())
                {
                    switch (it->second)
                    {

                    case Oscillator_Wave:
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
                    case Modulator_Wave_Selected_Operator:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            if (controllerValue == 127)
                            {
                                if (voice->getOscillator().getFmAlgh() == 0)
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
                                else
                                {
                                    voice->getOscillator().setSelectedOperator();
                                }
                            }
                        }

                        break;
                    case Gain:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minModulation = 0.0f;
                            double maxModulation = 1.0f;

                            // Calculate the scaled frequency
                            double scaledGain = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                            voice->setGain(scaledGain);
                        }
                        break;
                    case Reverb:
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
                    case Delay:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minLevel = 0;
                            double maxLevel = 44100 * 2;
                            double minLevel2 = 0;
                            double maxLevel2 = 10000;

                            double scaledLevel = minLevel + (maxLevel - minLevel) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                            double scaledFeedback = minLevel2 + (maxLevel2 - minLevel2) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                            voice->setDelay(scaledLevel, scaledFeedback);
                        }
                        break;
                    case Attack:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {

                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minLevel = 0.0f;
                            double maxLevel = 1.0f;
                            double scaledAttack = minLevel + (maxLevel - minLevel) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                            voice->changeAttack(scaledAttack);
                        }
                        break;

                    case Decay:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minLevel = 0.0f;
                            double maxLevel = 1.0f;
                            double scaledDecay = minLevel + (maxLevel - minLevel) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                            voice->changeDecay(scaledDecay);
                        }
                        break;

                    case Sustain:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minLevel = 0.0f;
                            double maxLevel = 1.0f;
                            double scaledSustain = minLevel + (maxLevel - minLevel) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                            voice->changeSustain(scaledSustain);
                        }
                        break;

                    case Release:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minLevel = 0.0f;
                            double maxLevel = 1.0f;
                            double scaledRelease = minLevel + (maxLevel - minLevel) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                            voice->changeRelease(scaledRelease);
                        }
                        break;
                    case Whitenoise_level:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minFrequency = 0.0f;
                            double maxFrequency = 0.35f;

                            // Calculate the scaled frequency
                            double scaledFrequency = minFrequency + (maxFrequency - minFrequency) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                            voice->setWhitenoiseLevel(scaledFrequency);
                        }
                        break;
                    case FM_Frequency:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minFrequency = 0.0f;
                            double maxFrequency = 20000.0f;

                            // Calculate the scaled frequency
                            double scaledFrequency = minFrequency + (maxFrequency - minFrequency) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                            juce::Logger::writeToLog("Entrei no changeFmFreq: " + juce::String(scaledFrequency));

                            voice->changeFmFreq(scaledFrequency);
                        }
                        break;
                    case FM_Depth:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minModulation = 0.0f;
                            double maxModulation = 1000.0f;

                            // Calculate the scaled frequency
                            double scaledModulation = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                            juce::Logger::writeToLog("Entrei no changeFmDepth: " + juce::String(scaledModulation));

                            voice->changeFmDepth(scaledModulation);
                        }
                        break;
                    case Chorus:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minModulation = 0.0f;
                            double maxModulation = 1.0f;

                            // Calculate the scaled frequency
                            double scaledModulation = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

                            voice->setMixChorus(scaledModulation);
                        }
                        break;
                    case Filter_Type:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            if (controllerValue == 127)
                            {
                                int type = voice->getFilter().getFilterType();

                                switch (voice->getButtonsMode())
                                {
                                case 0:
                                    if (type != 2)
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
                                    break;
                                case 1:
                                    voice->loadPreset(1);
                                    break;
                                case 2:
                                    voice->savePreset(1);
                                    break;
                                }
                            }
                        }
                        break;
                    case LFO_Control:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            if (controllerValue == 127)
                            {
                                int LFOControl = voice->getLFOControl();
                                switch (voice->getButtonsMode())
                                {
                                case 0:
                                    if (LFOControl == 4)
                                    {
                                        voice->setLFOControl(0);
                                    }
                                    else
                                    {
                                        voice->setLFOControl(LFOControl + 1);
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
                                    break;
                                case 1:
                                    voice->loadPreset(2);
                                    break;
                                case 2:
                                    voice->savePreset(2);
                                    break;
                                }
                            }
                        }
                        break;
                    case ADSR_Control:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            if (controllerValue == 127)
                            {
                                int ADSRControl = voice->getADSRControl();
                                switch (voice->getButtonsMode())
                                {
                                case 0:
                                    if (ADSRControl == 2)
                                    {
                                        voice->setADSRControl(0);
                                    }
                                    else
                                    {
                                        voice->setADSRControl(ADSRControl + 1);
                                    }
                                    break;
                                case 1:
                                    voice->loadPreset(3);
                                    break;
                                case 2:
                                    voice->savePreset(3);
                                    break;
                                }
                            }
                        }
                        break;
                    case Panic_Button:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            if (controllerValue == 127)
                            {
                                switch (voice->getButtonsMode())
                                {
                                case 0:
                                    voice->resetSynthParams();

                                    break;
                                case 1:
                                    voice->loadPreset(4);
                                    break;
                                case 2:
                                    voice->savePreset(4);
                                    break;
                                }
                            }
                        }
                        break;
                    case Harmonics_Tables:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            if (controllerValue == 127)
                            {

                                switch (voice->getButtonsMode())
                                {
                                case 0:
                                    voice->setOscillatorHarmonics();

                                    break;
                                case 1:
                                    voice->loadPreset(5);
                                    break;
                                case 2:
                                    voice->savePreset(5);
                                    break;
                                }
                            }
                        }
                        break;
                    case Note_Spacing:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            if (controllerValue == 127)
                            {

                                switch (voice->getButtonsMode())
                                {
                                case 0:
                                    voice->setFrequencySpacing();
                                    break;
                                case 1:
                                    voice->loadPreset(6);
                                    break;
                                case 2:
                                    voice->savePreset(6);
                                    break;
                                }
                            }
                        }
                        break;
                    case Synthesis_Types:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            if (controllerValue == 127)
                            {
                                juce::Logger::writeToLog("Mode:" + juce::String(voice->getButtonsMode()));
                                switch (voice->getButtonsMode())
                                {
                                case 0:
                                    voice->setSynthesisMode();

                                    break;
                                case 1:
                                    voice->loadPreset(7);
                                    break;
                                case 2:
                                    voice->savePreset(7);
                                    break;
                                }
                            }
                        }
                        break;
                    case Buttons_Mode:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            if (controllerValue == 127)
                            {
                                voice->setButtonsMode();
                            }
                        }
                        break;
                    case CutOff:
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
                    case Resonance:
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
                    case LFO_Depth:
                        if (auto voice = dynamic_cast<SynthVoice *>(mySynth.getVoice(0)))
                        {
                            int minControllerValue = 0;
                            int maxControllerValue = 127;

                            double minModulation = 0.1f;
                            double maxModulation = 20.0f;
                            double minModulationFilter = 0.1f;
                            double maxModulationFilter = 1000.0f;

                            double scaledValue = minModulation + (maxModulation - minModulation) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);
                            double scaledValueFilter = minModulationFilter + (maxModulationFilter - minModulationFilter) * (controllerValue - minControllerValue) / (maxControllerValue - minControllerValue);

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
                                voice->changeFilterLFOFreq(scaledValueFilter);
                                break;
                            case 3:
                                voice->getOscillator().setLFOFreq(scaledValue);
                                voice->setLFOGainFreq(scaledValue);
                                break;
                            case 4:
                                voice->getOscillator().setLFOFreq(scaledValue);
                                voice->setLFOGainFreq(scaledValue);
                                voice->changeFilterLFOFreq(scaledValueFilter);
                                break;
                            default:
                                break;
                            }
                        }
                        break;
                    case LFO_Rate:
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
            }
        }
    };
    void midiMappingToArray()
    {
        juce::File rootFolder = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory);
        juce::File xmlFile = rootFolder.getChildFile("MIDI.xml");
        if (!xmlFile.existsAsFile())
        {
            juce::Logger::writeToLog("MIDI.xml does not exist at path: " + xmlFile.getFullPathName());
            juce::Logger::writeToLog("Run the software with -m flag to enter MIDI Learning Mode");
        }
        else
        {
            juce::Logger::writeToLog("MIDI.xml found at path: " + xmlFile.getFullPathName());

            auto xml = juce::XmlDocument::parse(xmlFile);
            if (xml != nullptr)
            {
                juce::Logger::writeToLog("Successfully parsed MIDI.xml");

                for (int i = 0; i < functionsArray.size(); i++)
                {
                    if (xml->hasAttribute(functionsArray[i]))
                    {
                        int controlNumber = xml->getStringAttribute(functionsArray[i]).getIntValue();
                        midiMapping[controlNumber] = static_cast<ControlFunction>(i);
                        // juce::Logger::writeToLog("Mapped " + functionsArray[i] + " to control number " + juce::String(controlNumber));
                    }
                    else
                    {
                        juce::Logger::writeToLog("Attribute " + functionsArray[i] + " not found in XML");
                    }
                }
            }
            else
            {
                juce::Logger::writeToLog("Failed to parse MIDI.xml");
            }
        }
    }
    void audioDeviceAboutToStart(juce::AudioIODevice *device) override
    {
        // This method is called when the audio device is about to start
    }

    void audioDeviceStopped() override
    {
        // This method is called when the audio device is stopped
    }
    void setMidiLearningMode(bool mode)
    {
        midiLearningMode = mode;
    }

    int getCountValue()
    {
        return count;
    }

private:
    enum ControlFunction
    {
        Gain,
        CutOff,
        Resonance,
        Attack,
        Decay,
        Sustain,
        Release,
        LFO_Depth,
        LFO_Rate,
        FM_Depth,
        Whitenoise_level,
        Chorus,
        Delay,
        Reverb,
        FM_Frequency,
        Oscillator_Wave,
        Modulator_Wave_Selected_Operator,
        Filter_Type,
        LFO_Control,
        ADSR_Control,
        Panic_Button,
        Harmonics_Tables,
        Note_Spacing,
        Synthesis_Types,
        Buttons_Mode,
        Undefined
    };
    std::map<int, ControlFunction> midiMapping;
    juce::ValueTree tree;
    int lastControllerValue;
    juce::Synthesiser &mySynth;
    bool midiLearningMode = false;
    int count = 0;
    std::vector<std::string> functionsArray = {"Gain", "CutOff", "Resonance", "Attack", "Decay", "Sustain", "Release", "LFO_Depth", "LFO_Rate", "FM_Depth", "Whitenoise_level", "Chorus", "Delay", "Reverb", "FM_Frequency", "Oscillator_Wave", "Modulator_Wave_Selected_Operator", "Filter_Type", "LFO_Control", "ADSR_Control", "Panic_Button", "Harmonics_Tables", "Note_Spacing", "Synthesis_Types", "Buttons_Mode"};
};

int main(int argc, char *argv[])
{
    juce::Synthesiser synth;
    juce::AudioDeviceManager devmgr;
    devmgr.initialiseWithDefaultDevices(0, 2);
    juce::AudioIODevice *device = devmgr.getCurrentAudioDevice();
    int lastInputIndex = 0;
    bool midiLearn = false;
    synth.addSound(new SynthSound());

    synth.addVoice(new SynthVoice());
    // Poliphony
    //  for (int i = 0; i < 5; i++)
    //  {
    //      synth.addVoice (new SynthVoice());
    //  }
    //  for (int i = 0; i < synth.getNumVoices(); i++)
    //  {
    //      if (auto voice = dynamic_cast<SynthVoice *>(synth.getVoice(i)))
    //      {
    //          voice->prepareToPlay(44100.0, 512, 2);
    //      }
    //  }
    synth.setCurrentPlaybackSampleRate(44100);

    if (auto voice = dynamic_cast<SynthVoice *>(synth.getVoice(0)))
    {
        voice->prepareToPlay(44100.0, 512, 2);
    }

    MyAudioIODeviceCallback audioIODeviceCallback(synth);

    // if (xmlMIDI == nullptr)
    // {
    //             midiLearn = true;
    //     audioIODeviceCallback.setMidiLearningMode(true);
    // }
    // else
    // {
    //     audioIODeviceCallback.midiMappingToArray(xmlMIDI);

    // }
    if (argv[1] && std::string(argv[1]) == "-m")
    {
        midiLearn = true;
        audioIODeviceCallback.setMidiLearningMode(true);
    }
    else
    {
        audioIODeviceCallback.midiMappingToArray();
    }
    // Add the audio device callback
    devmgr.addAudioCallback(&audioIODeviceCallback);
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    devmgr.setMidiInputDeviceEnabled(midiInputs[1].identifier, true);
    devmgr.addMidiInputDeviceCallback(midiInputs[1].identifier, &audioIODeviceCallback);

    devmgr.restartLastAudioDevice();

    // Run the message loop
    if (midiLearn)
    {
        juce::Logger::writeToLog("MIDI Learning Mode");
        juce::Logger::writeToLog("Move the choosen knob controller for function: Gain");
    }
    else
    {
        audioIODeviceCallback.midiMappingToArray();
    }
    while (true)
    {

        juce::Thread::sleep(100);
    }

    // Stop the audio device and remove the callback when exiting the application
    devmgr.removeAudioCallback(&audioIODeviceCallback);
    devmgr.closeAudioDevice();

    return 0;
}
