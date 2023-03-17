/*
  ==============================================================================

    Fader.h
    Created: 21 Feb 2023 7:08:38pm
    Author:  Ophelia
    Purpose: a set of buttons & sliders to control fade-in and fade-out effects

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
class Fader : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    /** Constructor: takes in a DJAudioPlayer to be able to send fade instructions to audio source*/
    Fader(DJAudioPlayer* _player);

    /** Destructor */
    ~Fader() override;

    /**
    *The paint() method gets called when a region of a component needs redrawing,
    *either because the component's repaint() method has been called, or because something
    *has happened on the screen that means a section of a window needs to be redrawn
   */
    void paint(juce::Graphics&) override;

    /** Called when this component's size has been changed */
    void resized() override;

    /** Implements the pure virtual function that cames with the Button listener which this class inherits from */
    void buttonClicked(juce::Button* button) override;

    /** Implements the pure virtual function that cames with the Slider listener which this class inherits from */
    void sliderValueChanged(juce::Slider* slider) override;

private:

    // Loads "FadeIn"/"FadeOut"/"StopFading" icons from the Project's "SOURCE" directory
    juce::File fadeInImageFile = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" }).getChildFile("fadeIn.png");
    juce::File fadeOutImageFile = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" }).getChildFile("fadeOut.png");
    juce::File stopFadeImageFile = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" }).getChildFile("stopFade.png");

    // Converts the icons files into a juce Image format
    juce::Image fadeInImage = juce::PNGImageFormat::loadFrom(fadeInImageFile);
    juce::Image fadeOutImage = juce::PNGImageFormat::loadFrom(fadeOutImageFile);
    juce::Image stopFadeImage = juce::PNGImageFormat::loadFrom(stopFadeImageFile);

    // Creates three image buttons for FadeIn/FadeOut/StopFade
    juce::ImageButton fadeInButton{ "FadeIn" };
    juce::ImageButton fadeOutButton{ "FadeOut" };
    juce::ImageButton stopFadeButton{ "StopFade" };

    // Slider to determine how fast to fade in / fade out
    juce::Slider fadeSpeedSlider;
    juce::Label labelForFadeSpeedSlider;

    // The DJAudioPlayer pointer into the constructor of the Fader in its parent DeckGUI owner class
    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fader)
};
