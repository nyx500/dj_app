/*
  ==============================================================================

    ReverbEffects.cpp
    Created: 14 Mar 2023 9:34:54pm
    Author:  Ophelia
    Purpose: a GUI component containing the sliders that pass values into
    the AudioComponent (DJAudioPlayer)'s reverbAudioSource to add reverb
    effects to the current audio track.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ReverbEffects.h"

//==============================================================================

/**
 *Constructor: takes a pointer to the corresponding DJAudioPlayer in the same DeckGUI
 *This allows the ReverbEffects to passed straight into the DJAudioPlayer
 */
ReverbEffects::ReverbEffects(DJAudioPlayer* _player)
    : player(_player)
{
    // Add and set listeners/ranges/values for all the reverb parameter sliders

    // Reverb "Room Size" slider
    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(roomSizeLabel);
    roomSizeSlider.addListener(this);
    roomSizeSlider.setRange(0.0, 1.0);
    roomSizeSlider.setValue(0.5);
    roomSizeSlider.setSliderStyle(juce::Slider::LinearVertical);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    roomSizeLabel.setText("Room", juce::dontSendNotification);
    roomSizeLabel.setJustificationType(juce::Justification::centredTop);
    roomSizeLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    // Reverb Damping slider
    addAndMakeVisible(dampingSlider);
    addAndMakeVisible(dampingLabel);
    dampingSlider.addListener(this);
    dampingSlider.setRange(0.0, 1.0);
    dampingSlider.setValue(0.5);
    dampingSlider.setSliderStyle(juce::Slider::LinearVertical);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    dampingLabel.setText("Damp", juce::dontSendNotification);
    dampingLabel.setJustificationType(juce::Justification::centredTop);
    dampingLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    // Reverb "Wet Level" slider
    addAndMakeVisible(wetLevelSlider);
    addAndMakeVisible(wetLevelLabel);
    wetLevelSlider.addListener(this);
    wetLevelSlider.setRange(0.0, 1.0);
    wetLevelSlider.setValue(0.33);
    wetLevelSlider.setSliderStyle(juce::Slider::LinearVertical);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    wetLevelLabel.setText("Wet", juce::dontSendNotification);
    wetLevelLabel.setJustificationType(juce::Justification::centredTop);
    wetLevelLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    // Reverb "Dry Level" slider
    addAndMakeVisible(dryLevelSlider);
    addAndMakeVisible(dryLevelLabel);
    dryLevelSlider.addListener(this);
    dryLevelSlider.setRange(0.0, 1.0);
    dryLevelSlider.setValue(0.4);
    dryLevelSlider.setSliderStyle(juce::Slider::LinearVertical);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    dryLevelLabel.setText("Dry", juce::dontSendNotification);
    dryLevelLabel.setJustificationType(juce::Justification::centredTop);
    dryLevelLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    // Reverb "Width" slider
    addAndMakeVisible(reverbWidthSlider);
    addAndMakeVisible(reverbWidthLabel);
    reverbWidthSlider.addListener(this);
    reverbWidthSlider.setRange(0.0, 1.0);
    reverbWidthSlider.setValue(1.0);
    reverbWidthSlider.setSliderStyle(juce::Slider::LinearVertical);
    reverbWidthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    reverbWidthLabel.setText("Width", juce::dontSendNotification);
    reverbWidthLabel.setJustificationType(juce::Justification::centredTop);
    reverbWidthLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    // Reverb "Freeze Mode" slider
    addAndMakeVisible(freezeModeSlider);
    addAndMakeVisible(freezeModeLabel);
    freezeModeSlider.addListener(this);
    freezeModeSlider.setRange(0.0, 1.0);
    freezeModeSlider.setValue(0.0);
    freezeModeSlider.setSliderStyle(juce::Slider::LinearVertical);
    freezeModeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    freezeModeLabel.setText("Freeze", juce::dontSendNotification);
    freezeModeLabel.setJustificationType(juce::Justification::centredTop);
    freezeModeLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    // The Reset Parameters to Default button
    addAndMakeVisible(resetParamsButton);
    resetParamsButton.addListener(this);
}

/** Destructor */
ReverbEffects::~ReverbEffects()
{
}

/**
 *The paint() method gets called when a region of a component needs redrawing,
 *either because the component's repaint() method has been called, or because something
 *has happened on the screen that means a section of a window needs to be redrawn.
 */
void ReverbEffects::paint(juce::Graphics& g)
{
    // No background
    g.fillAll(juce::Colour());
}

/**
 *Called when this component's size has been changed
 *Draws the different reverb effect sliders
 */
void ReverbEffects::resized()
{
    // Splits the component horizontally into 7 columns with one for each slider
    double colW = getWidth() / 7;
    roomSizeLabel.setBounds(0, 0, colW, getHeight() * 0.1);
    roomSizeSlider.setBounds(0, getHeight() * 0.1, colW, getHeight() * 0.7);
    dampingLabel.setBounds(colW, 0, colW, getHeight() * 0.1);
    dampingSlider.setBounds(colW, getHeight() * 0.1, colW, getHeight() * 0.7);
    wetLevelLabel.setBounds(colW * 2, 0, colW, getHeight() * 0.1);
    wetLevelSlider.setBounds(colW * 2, getHeight() * 0.1, colW, getHeight() * 0.7);
    dryLevelLabel.setBounds(colW * 3, 0, colW, getHeight() * 0.1);
    dryLevelSlider.setBounds(colW * 3, getHeight() * 0.1, colW, getHeight() * 0.7);
    reverbWidthLabel.setBounds(colW * 4, 0, colW, getHeight() * 0.1);
    reverbWidthSlider.setBounds(colW * 4, getHeight() * 0.1, colW, getHeight() * 0.7);
    freezeModeLabel.setBounds(colW * 5, 0, colW, getHeight() * 0.1);
    freezeModeSlider.setBounds(colW * 5, getHeight() * 0.1, colW, getHeight() * 0.7);

    // At the bottom of all the sliders, set the restore to defaults button
    resetParamsButton.setBounds(0, getHeight() * 0.9, getWidth() * 0.9, getHeight() * 0.1);
}

/**
 *Implements the button listener by overriding the inherited buttonClicked method
 *Resets the reverb parameters to default ones
 */
void ReverbEffects::buttonClicked(juce::Button* button)
{
    if (button == &resetParamsButton)
    {
        player->resetReverbParamsToDefault();
        roomSizeSlider.setValue(0.5);
        dampingSlider.setValue(0.5);
        wetLevelSlider.setValue(0.33);
        reverbWidthSlider.setValue(1.0);
        dryLevelSlider.setValue(0.4);
        freezeModeSlider.setValue(0.0);
    }
}

/**
 *Implements the slider listener by overriding the inherited sliderValueChanged method
 * Passes different Reverb effects into the audio source with the stored "player" ptr
 */
void ReverbEffects::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &roomSizeSlider)
    {
        player->setReverbRoomSize(slider->getValue());
    }
    if (slider == &dampingSlider)
    {
        player->setReverbDamping(slider->getValue());
    }
    if (slider == &wetLevelSlider)
    {
        player->setReverbWetLevel(slider->getValue());
    }
    if (slider == &dryLevelSlider)
    {
        player->setReverbDryLevel(slider->getValue());
    }
    if (slider == &reverbWidthSlider)
    {
        player->setReverbWidth(slider->getValue());
    }
    if (slider == &freezeModeSlider)
    {
        player->setReverbFreezeMode(slider->getValue());
    }
}
