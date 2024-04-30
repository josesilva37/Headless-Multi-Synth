#include "WavetableOscillator.h"

void WavetableOscillator::setFrequency(float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = (float)tableSize / sampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;
};

// void WavetableOscillator::updateAngle()
// {
//     currentAngle += angleDelta;
//     if (currentAngle >= juce::MathConstants<float>::twoPi)
//         currentAngle -= juce::MathConstants<float>::twoPi;
// };
float WavetableOscillator::getNextSample()
{
    auto tableSize = (unsigned int)wavetable.getNumSamples();

    auto index0 = (unsigned int)currentIndex; // [6]
    auto index1 = index0 + 1;

    auto frac = currentIndex - (float)index0; // [7]

    auto *table = wavetable.getReadPointer(0); // [8]
    auto value0 = table[index0];
    auto value1 = table[index1];

    auto currentSample = value0 + frac * (value1 - value0); // [9]

    if ((currentIndex += tableDelta) > (float)tableSize) // [10]
        currentIndex -= (float)tableSize;

    return currentSample;
};
