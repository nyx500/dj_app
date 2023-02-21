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
class Fader  : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    Fader();
    ~Fader() override;


    /** Implements the button listener */
    void buttonClicked(juce::Button* button) override;

    /**Implements Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Getter for fadeIn value: returns fadeIn bool value */
    bool getFadeIn();

    /** Getter for fadeOut value: returns fadeOut bool value */
    bool getFadeOut();

    /** Getter for fadeInMaxVol: returns the maximum volume 
    the music should "fade in" to */
    double getFadeInMaxVol();

    /** Getter for fadeOutMinVol: returns the minimum volume to music should
    "fade out" to */
    double getFadeOutMinVol();


private:

    juce::TextButton fadeInButton{ "Fade In" };
    juce::TextButton fadeOutButton{ "Fade Out" };
    juce::TextButton stopFadeButton{ "Stop Fade" };

    juce::Slider minVolumeSlider;
    juce::Slider maxVolumeSlider;

    bool fadeIn = false;
    bool fadeOut = false;

    double fadeInMaxVol = 0.1;
    double fadeOutMinVol = 0.9;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Fader)
};
