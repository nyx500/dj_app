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
Fader::Fader()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(fadeInButton);
    addAndMakeVisible(fadeOutButton);
    addAndMakeVisible(stopFadeButton);
    addAndMakeVisible(minVolumeSlider);
    addAndMakeVisible(maxVolumeSlider);

    minVolumeSlider.setRange(0.0, 0.95, 0.001);
    maxVolumeSlider.setRange(0.05, 1.0, 0.001);

    fadeInButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::dodgerblue);
    fadeOutButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkslateblue);
    stopFadeButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);

    fadeInButton.addListener(this);
    fadeOutButton.addListener(this);
    stopFadeButton.addListener(this);
    minVolumeSlider.addListener(this);
    maxVolumeSlider.addListener(this);
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

    double rowHeight = getHeight() / 4;

    fadeInButton.setBounds(0, 0, getWidth() / 2, rowHeight);
    fadeOutButton.setBounds(getWidth() / 2, 0, getWidth() / 2, rowHeight);
    stopFadeButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    minVolumeSlider.setBounds(0, rowHeight * 2, getWidth(), rowHeight);
    maxVolumeSlider.setBounds(0, rowHeight * 3, getWidth(), rowHeight);
}

/** Implements the button listener */
void Fader::buttonClicked(juce::Button* button)
{
    if (button == &fadeInButton)
    {
        DBG("Fader::buttonClicked - fadeInButton clicked");
    }

    if (button == &fadeOutButton)
    {
        DBG("Fader::buttonClicked - fadeOutButton clicked");
    }

    if (button == &stopFadeButton)
    {
        DBG("Fader::buttonClicked - stopFadeButton clicked");
    }
}

/**Implements Slider::Listener */
void Fader::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &minVolumeSlider)
    {
        DBG("Fader::sliderValueChanged - minVolumeSlider changed");
    }

    if (slider == &maxVolumeSlider)
    {
        DBG("Fader::sliderValueChanged - maxVolumeSlider changed");
    }
}