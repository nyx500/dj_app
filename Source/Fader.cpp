/*
  ==============================================================================

    Fader.cpp
    Created: 21 Feb 2023 7:08:38pm
    Author:  Ophelia
    Purpose: a set of buttons & sliders to control fade-in and fade-out effects

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Fader.h"

//==============================================================================
/** Constructor: takes in a DJAudioPlayer to be able to send fade instructions to audio source*/
Fader::Fader(DJAudioPlayer* _player) : player(_player)
{
    // Add all the fader buttons
    addAndMakeVisible(fadeInButton);
    addAndMakeVisible(fadeOutButton);
    addAndMakeVisible(stopFadeButton);
    addAndMakeVisible(fadeSpeedSlider);
    addAndMakeVisible(labelForFadeSpeedSlider);

    // Set the slider range for how fast to fade in or out
    // 0.001 - the increments which the user can apply when manipulating the fade speed
    fadeSpeedSlider.setRange(0.0, 0.2, 0.001);

    // Sets the default value for the fadespeed slider to 0.002
    fadeSpeedSlider.setValue(0.002);
    // Make the slider round    
    fadeSpeedSlider.setSliderStyle(juce::Slider::Rotary);

    // Initially set fader buttons to darkgrey --> bright blue when 'on', dark grey when 'off'
    fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    // Set stop button to red
    stopFadeButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);

    // Add this component as a listener to the buttons & sliders
    fadeInButton.addListener(this);
    fadeOutButton.addListener(this);
    stopFadeButton.addListener(this);
    fadeSpeedSlider.addListener(this);

    // Set the text for the speed slider label
    labelForFadeSpeedSlider.setText("Fade speed", juce::dontSendNotification);
    labelForFadeSpeedSlider.setColour(juce::Label::textColourId, juce::Colours::white);
}

/** Destructor */
Fader::~Fader()
{
}

/**
 *The paint() method gets called when a region of a component needs redrawing,
 *either because the component's repaint() method has been called, or because something
 *has happened on the screen that means a section of a window needs to be redrawn
*/
void Fader::paint(juce::Graphics& g)
{   
    // Make background transparent (no background)
    g.fillAll(juce::Colour());
}


/** Called when this component's size has been changed */
void Fader::resized()
{   
    // Divide the component into five rows then put the buttons/sliders in a column on the left-hand side of the DeckGUI
    double rowHeight = getHeight() / 5;
    fadeInButton.setBounds(getWidth() * 0.1, 0, getWidth() * 0.8, rowHeight);
    fadeOutButton.setBounds(getWidth() * 0.1, rowHeight * 1.1, getWidth() * 0.8, rowHeight);
    stopFadeButton.setBounds(getWidth() * 0.1, rowHeight * 2.2, getWidth() * 0.8, rowHeight);
    fadeSpeedSlider.setBounds(getWidth() * 0.05, rowHeight * 3.3, getWidth() * 0.9, rowHeight * 1.4);
    labelForFadeSpeedSlider.setBounds(getWidth() * 0.45, rowHeight * 4.7, getWidth() * 0.5, rowHeight * 0.3);
}

/** Implements the pure virtual function that cames with the Button listener which this class inherits from */
void Fader::buttonClicked(juce::Button* button)
{   

    if (button == &fadeInButton)
    {   
        // Tells the audio player child component to fade tne music in (increase the volume incrementally)
        player->autoFadeIn();
        // Change colour of fadeIn (increase volume) button to bright blue
        fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(14, 135, 250));
        // Grey out the inactive fadeOut button
        fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    }

    if (button == &fadeOutButton)
    {

        // Tells the audio player child component to fade the music out (decrease the volume incrementally)
        player->autoFadeOut();
        // Change colour of fadeOut (decrease volume) button to bright blue
        fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(14, 135, 250));
        // Grey out the inactive fadeOut button
        fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    }

    if (button == &stopFadeButton)
    {   
        // Tell the audio player child component to stop any fading effects
        player->stopAutoFade();
        // Grey out both the fader buttons
        fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    }
}

/** Implements the pure virtual function that cames with the Slider listener which this class inherits from */
void Fader::sliderValueChanged(juce::Slider* slider)
{   
    // Pass in the new fade speed value into the child DJAudioPlayer
    if (slider == &fadeSpeedSlider)
    {
        player->setFadeSpeed(slider->getValue());
    }
}
