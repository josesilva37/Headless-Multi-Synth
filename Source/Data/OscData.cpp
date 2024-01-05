/*
  ==============================================================================

    OscData.cpp
    Created: 5 Jan 2024 4:36:56pm
    Author:  jose

  ==============================================================================
*/

#include "OscData.h"

void OscData::setType (const int oscSelection)
{
    switch (oscSelection)
    {
        // Sine
        case 0:
            initialise ([](float x) { return std::sin (x); });
            break;
            
        // Saw
        case 1:
            initialise ([] (float x) { return x / juce::MathConstants<float>::pi; });
            break;
          
        // Square
        case 2:
            initialise ([] (float x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
            
        default:
            // You shouldn't be here!
            jassertfalse;
            break;
    }
}
