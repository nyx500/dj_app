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

    // Sets slider range for how fast to fade in or out
    fadeSpeedSlider.setRange(0.0, 0.2, 0.001);
    // Sets the default valuen for the slider to 0.002
    fadeSpeedSlider.setValue(0.002);

    fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    stopFadeButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);

    fadeInButton.addListener(this);
    fadeOutButton.addListener(this);
    stopFadeButton.addListener(this);
    fadeSpeedSlider.addListener(this);
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

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void Fader::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowHeight = getHeight() / 3;

    fadeInButton.setBounds(0, 0, getWidth() / 2, rowHeight);
    fadeOutButton.setBounds(getWidth() / 2, 0, getWidth() / 2, rowHeight);
    stopFadeButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    fadeSpeedSlider.setBounds(0, rowHeight * 2, getWidth(), rowHeight);
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
