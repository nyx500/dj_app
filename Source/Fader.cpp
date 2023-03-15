/*
  ==============================================================================

    Fader.cpp
    Created: 21 Feb 2023 7:08:38pm
    Author:  Ophelia

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Fader.h"

//==============================================================================
Fader::Fader(DJAudioPlayer* _player) :
    player(_player)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(fadeInButton);
    addAndMakeVisible(fadeOutButton);
    addAndMakeVisible(stopFadeButton);
    addAndMakeVisible(fadeSpeedSlider);
    addAndMakeVisible(labelForFadeSpeedSlider);

    // Sets slider range for how fast to fade in or out
    fadeSpeedSlider.setRange(0.0, 0.2, 0.001);
    // Sets the default valuen for the slider to 0.002
    fadeSpeedSlider.setValue(0.002);
    fadeSpeedSlider.setSliderStyle(juce::Slider::Rotary);

    // Set fade in buttons to blue
    fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(14, 135, 250));
    fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(14, 135, 250));
    // Set stop button to red
    stopFadeButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);

    fadeInButton.addListener(this);
    fadeOutButton.addListener(this);
    stopFadeButton.addListener(this);
    fadeSpeedSlider.addListener(this);

    // Set the text for the speed slider label
    labelForFadeSpeedSlider.setText("Fade speed", juce::dontSendNotification);
    labelForFadeSpeedSlider.setColour(juce::Label::textColourId, juce::Colours::white);
}

Fader::~Fader()
{
}

void Fader::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(juce::Colour());   // clear the background

    //// draw an outline around the component
    //g.setColour (juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void Fader::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowHeight = getHeight() / 5;

    fadeInButton.setBounds(getWidth() * 0.1, 0, getWidth() * 0.8, rowHeight);
    fadeOutButton.setBounds(getWidth() * 0.1, rowHeight * 1.1, getWidth() * 0.8, rowHeight);
    stopFadeButton.setBounds(getWidth() * 0.1, rowHeight * 2.2, getWidth() * 0.8, rowHeight);
    fadeSpeedSlider.setBounds(getWidth() * 0.05, rowHeight * 3.3, getWidth() * 0.9, rowHeight * 1.4);
    labelForFadeSpeedSlider.setBounds(getWidth() * 0.45, rowHeight * 4.7, getWidth() * 0.5, rowHeight * 0.3);
}

/** Implements the button listener */
void Fader::buttonClicked(juce::Button* button)
{
    if (button == &fadeInButton)
    {   
        player->autoFadeIn();
        // Changes colour of the button to "on" button colour
        fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::deeppink);
        fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);

    }

    if (button == &fadeOutButton)
    {
        player->autoFadeOut();
        // Change colour of the button to "on" button colour
        fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::deeppink);
        fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);

    }

    if (button == &stopFadeButton)
    {   
        player->stopAutoFade();
        // Change colours back to "off" colours
        fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    }
}

/**Implements Slider::Listener */
void Fader::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &fadeSpeedSlider)
    {
        player->setFadeSpeed(slider->getValue());
    }
}
