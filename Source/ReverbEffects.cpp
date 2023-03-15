/*
  ==============================================================================

    ReverbEffects.cpp
    Created: 14 Mar 2023 9:34:54pm
    Author:  Ophelia

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ReverbEffects.h"

//==============================================================================
ReverbEffects::ReverbEffects(DJAudioPlayer* _player)
    : player(_player)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Add and set listeners/ranges/values for all reverb parameter sliders

    // Reverb "Room Size" slider
    addAndMakeVisible(roomSizeSlider);
    roomSizeSlider.addListener(this);
    roomSizeSlider.setRange(0.0, 1.0);
    roomSizeSlider.setValue(0.5);

    // Reverb Damping slider
    addAndMakeVisible(dampingSlider);
    dampingSlider.addListener(this);
    dampingSlider.setRange(0.0, 1.0);
    dampingSlider.setValue(0.5);

    // Reverb "Wet Level" slider
    addAndMakeVisible(wetLevelSlider);
    wetLevelSlider.addListener(this);
    wetLevelSlider.setRange(0.0, 1.0);
    wetLevelSlider.setValue(0.33);

    // Reverb "Dry Level" slider
    addAndMakeVisible(dryLevelSlider);
    dryLevelSlider.addListener(this);
    dryLevelSlider.setRange(0.0, 1.0);
    dryLevelSlider.setValue(0.4);

    // Reverb "Width" slider
    addAndMakeVisible(reverbWidthSlider);
    reverbWidthSlider.addListener(this);
    reverbWidthSlider.setRange(0.0, 1.0);   
    reverbWidthSlider.setValue(1.0);

    // Reverb "Freeze Mode" slider
    addAndMakeVisible(freezeModeSlider);
    freezeModeSlider.addListener(this);
    freezeModeSlider.setRange(0.0, 1.0);
    freezeModeSlider.setValue(0.0);

    // The Reset Parameters to Default button
    addAndMakeVisible(resetParamsButton);
    resetParamsButton.addListener(this);
}

ReverbEffects::~ReverbEffects()
{
}

void ReverbEffects::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::mediumpurple);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ReverbEffects", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void ReverbEffects::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 7;
    roomSizeSlider.setBounds(0, 0, getWidth(), rowH);
    dampingSlider.setBounds(0, rowH, getWidth(), rowH);
    wetLevelSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    dryLevelSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    reverbWidthSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    freezeModeSlider.setBounds(0, rowH * 5, getWidth(), rowH);
    resetParamsButton.setBounds(0, rowH * 6, getWidth(), rowH);
}


/** Implements the button listener by overriding the inherited buttonClicked method*/
void ReverbEffects::buttonClicked(juce::Button* button)
{
    if (button == &resetParamsButton)
    {   
        player->resetReverbParamsToDefault();
        // Reset the slider values
        roomSizeSlider.setValue(0.5);
        dampingSlider.setValue(0.5);
        wetLevelSlider.setValue(0.33);
        reverbWidthSlider.setValue(1.0);
        dryLevelSlider.setValue(0.4);
        freezeModeSlider.setValue(0.0);
    }
}

/**Implement the slider listener by overriding the inherited sliderValueChanged method*/
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
