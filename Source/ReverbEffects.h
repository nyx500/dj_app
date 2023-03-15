/*
  ==============================================================================

    ReverbEffects.h
    Created: 14 Mar 2023 9:34:54pm
    Author:  Ophelia

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class ReverbEffects  : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    ReverbEffects(DJAudioPlayer* _player);
    ~ReverbEffects() override;

    /** Implements the button listener by overriding the inherited buttonClicked method*/
    void buttonClicked(juce::Button* button) override;

    /**Implement the slider listener by overriding the inherited sliderValueChanged method*/
    void sliderValueChanged(juce::Slider* slider) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Sliders to control various "reverb parameters" that go in the Reverb::Parameters "struct"
    juce::Slider roomSizeSlider;
    juce::Slider dampingSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;
    juce::Slider reverbWidthSlider;
    juce::Slider freezeModeSlider;

    // Slider labels
    juce::Label roomSizeLabel;
    juce::Label dampingLabel;
    juce::Label wetLevelLabel;
    juce::Label dryLevelLabel;
    juce::Label reverbWidthLabel;
    juce::Label freezeModeLabel;


    // Button to reset the parameters to defaults
    juce::TextButton resetParamsButton{"Reset to Default"};

    // Can send messages from listeners to DJAudioPlayer now
    // Pass the player into the constructor
    DJAudioPlayer* player;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbEffects)
};
