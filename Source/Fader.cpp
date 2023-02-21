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
    g.drawText ("Fader", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void Fader::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    fadeInButton.setBounds(0, 0, getWidth() / 2, getHeight() / 2);

}
