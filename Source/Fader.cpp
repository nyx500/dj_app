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
    // Sets the size of the the FadeIn/FadeOut/StopFade icons stored as juce::Image data members to 80x80 pixels
    fadeInImage = fadeInImage.rescaled(100, 80, juce::Graphics::mediumResamplingQuality);
    fadeOutImage = fadeOutImage.rescaled(100, 80, juce::Graphics::mediumResamplingQuality);
    stopFadeImage = stopFadeImage.rescaled(100, 100, juce::Graphics::mediumResamplingQuality);

    // Adds the icons to the ImageButtons --> first colour is for normal (not held down) button state (original icon's blue for FadeIn/FadeOut)
    // Second colour is for when button is hovered over with the mouse (white)
    // Third colour is for the moment when the button is clicked (red)
    fadeInButton.setImages(true, true, true, fadeInImage, 1.0, juce::Colour(),
        juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(),
        1.0, juce::Colour(255, 0, 0), 0.0f
    );
    fadeOutButton.setImages(true, true, true, fadeOutImage, 1.0, juce::Colour(),
        juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(),
        1.0, juce::Colour(255, 0, 0), 0.0f
    );
    stopFadeButton.setImages(true, true, true, stopFadeImage, 1.0, juce::Colour(),
        juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(),
        1.0, juce::Colour(255, 0, 0), 0.0f
    );

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
    // Set text box containing number value above the slider
    fadeSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 0, 0);

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
    // Justify the label in the center below the slider
    labelForFadeSpeedSlider.setText("Fade Speed", juce::dontSendNotification);
    labelForFadeSpeedSlider.setJustificationType(juce::Justification::centred);
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
    double rowHeight = getHeight() / 6;
    fadeInButton.setBounds(0, 0, getWidth(), rowHeight * 0.9);
    fadeOutButton.setBounds(0, rowHeight * 1.1, getWidth(), rowHeight * 0.9);
    stopFadeButton.setBounds(0, rowHeight * 2.1, getWidth(), rowHeight * 0.9);
    fadeSpeedSlider.setBounds(getWidth() * 0.05, rowHeight * 3.1, getWidth() * 0.9, rowHeight * 2);
    labelForFadeSpeedSlider.setBounds(getWidth() * 0.1, rowHeight * 5.1, getWidth(), rowHeight * 0.9);
}

/** Implements the pure virtual function that cames with the Button listener which this class inherits from */
void Fader::buttonClicked(juce::Button* button)
{   

    if (button == &fadeInButton)
    {   
        // Tells the audio player child component to fade tne music in (increase the volume incrementally)
        player->autoFadeIn();

    }

    if (button == &fadeOutButton)
    {
        // Tells the audio player child component to fade the music out (decrease the volume incrementally)
        player->autoFadeOut();
    }

    if (button == &stopFadeButton)
    {   
        // Tell the audio player child component to stop any fading effects
        player->stopAutoFade();
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
