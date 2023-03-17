/*
  ==============================================================================

    ReverbEffects.h
    Created: 14 Mar 2023 9:34:54pm
    Author:  Ophelia
    Purpose: a GUI component containing the sliders that pass values into
    the AudioComponent (DJAudioPlayer)'s reverbAudioSource to add reverb
    effects to the current audio track.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
 */
class ReverbEffects : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    /**
     *Constructor: takes a pointer to the corresponding DJAudioPlayer in the same DeckGUI
     *This allows the ReverbEffects to passed straight into the DJAudioPlayer
     */
    ReverbEffects(DJAudioPlayer* _player);

    /** Destructor */
    ~ReverbEffects() override;

    /**
     *Implements the button listener by overriding the inherited buttonClicked method
     *Resets the reverb parameters to default ones
     */
    void buttonClicked(juce::Button* button) override;

    /**
     *Implements the slider listener by overriding the inherited sliderValueChanged method
     * Passes different Reverb effects into the audio source with the stored "player" ptr
     */
    void sliderValueChanged(juce::Slider* slider) override;

    /**
     *The paint() method gets called when a region of a component needs redrawing,
     *either because the component's repaint() method has been called, or because something
     *has happened on the screen that means a section of a window needs to be redrawn.
     */
    void paint(juce::Graphics&) override;

    /**
     *Called when this component's size has been changed
     *Draws the different reverb effect sliders
     */
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
    juce::TextButton resetParamsButton{ "Reset to Default" };

    // Can send messages from listeners to DJAudioPlayer now
    // Pass the player into the constructor
    DJAudioPlayer* player;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbEffects)
};
