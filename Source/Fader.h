/*
  ==============================================================================

    Fader.h
    Created: 21 Feb 2023 7:08:38pm
    Author:  Ophelia

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class Fader  : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    Fader(DJAudioPlayer* _player);
    ~Fader() override;


    /** Implements the button listener */
    void buttonClicked(juce::Button* button) override;

    /**Implements Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;

    void paint (juce::Graphics&) override;
    void resized() override;


private:
    juce::TextButton fadeInButton{ "Fade In" };
    juce::TextButton fadeOutButton{ "Fade Out" };
    juce::TextButton stopFadeButton{ "Stop Fade" };
    juce::Slider fadeSpeedSlider;
    juce::Label labelForFadeSpeedSlider;

    // Pass the player into the constructor
    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Fader)
};
