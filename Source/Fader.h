/*
  ==============================================================================

    Fader.h
    Created: 21 Feb 2023 7:08:38pm
    Author:  Ophelia

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Fader  : public juce::Component
{
public:
    Fader();
    ~Fader() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::TextButton fadeInButton{ "Fade In" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Fader)
};
