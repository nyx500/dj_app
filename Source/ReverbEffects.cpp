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
    addAndMakeVisible(roomSizeSlider);
    roomSizeSlider.addListener(this);
    roomSizeSlider.setRange(0.0, 1.0);
    roomSizeSlider.setValue(0.5);
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
    roomSizeSlider.setBounds(0, 0, getWidth(), getHeight());
}


/** Implements the button listener by overriding the inherited buttonClicked method*/
void ReverbEffects::buttonClicked(juce::Button* button)
{

}

/**Implement the slider listener by overriding the inherited sliderValueChanged method*/
void ReverbEffects::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &roomSizeSlider)
    {
        player->setReverbRoomSize(slider->getValue());
    }
}
