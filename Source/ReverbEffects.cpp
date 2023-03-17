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
    roomSizeSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
    // Colours go from very light blue for slider (this one) on the very left, to dark blue on the right.
    // This is because the gradient background goes from dark on the left and light on the right.
    // Colour: "beau blue"
    // Attribution: https://www.schemecolor.com/light-to-dark-blue.php
    roomSizeSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(188, 210, 232));
    roomSizeSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour(188, 210, 232));
    roomSizeSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour(188, 210, 232));
    roomSizeSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    roomSizeSlider.setTextBoxIsEditable(true);
    roomSizeLabel.setText("Room", juce::dontSendNotification);
    roomSizeLabel.setJustificationType(juce::Justification::centredTop);
    roomSizeLabel.setColour(juce::Label::textColourId, juce::Colour(188, 210, 232));

    // Reverb Damping slider
    addAndMakeVisible(dampingSlider);
    addAndMakeVisible(dampingLabel);
    dampingSlider.addListener(this);
    dampingSlider.setRange(0.0, 1.0);
    dampingSlider.setValue(0.5);
    dampingSlider.setSliderStyle(juce::Slider::LinearVertical);
    dampingSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
    // Colour: "dark sky blue"
    // Attribution: https://www.schemecolor.com/light-to-dark-blue.php
    dampingSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(145, 186, 214));
    dampingSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour(145, 186, 214));
    dampingSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour(145, 186, 214));
    dampingSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    dampingSlider.setTextBoxIsEditable(true);
    dampingLabel.setText("Damp", juce::dontSendNotification);
    dampingLabel.setJustificationType(juce::Justification::centredTop);
    dampingLabel.setColour(juce::Label::textColourId, juce::Colour(145, 186, 214));

    // Reverb "Wet Level" slider
    addAndMakeVisible(wetLevelSlider);
    addAndMakeVisible(wetLevelLabel);
    wetLevelSlider.addListener(this);
    wetLevelSlider.setRange(0.0, 1.0);
    wetLevelSlider.setValue(0.33);
    wetLevelSlider.setSliderStyle(juce::Slider::LinearVertical);
    wetLevelSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
    // Colour: "cyan azure"
    // Attribution: https://www.schemecolor.com/light-to-dark-blue.php
    wetLevelSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(82, 138, 174));
    wetLevelSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour(82, 138, 174));
    wetLevelSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour(82, 138, 174));
    wetLevelSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    wetLevelSlider.setTextBoxIsEditable(true);
    wetLevelLabel.setText("Wet", juce::dontSendNotification);
    wetLevelLabel.setJustificationType(juce::Justification::centredTop);
    wetLevelLabel.setColour(juce::Label::textColourId, juce::Colour(82, 138, 174));

    // Reverb "Dry Level" slider
    addAndMakeVisible(dryLevelSlider);
    addAndMakeVisible(dryLevelLabel);
    dryLevelSlider.addListener(this);
    dryLevelSlider.setRange(0.0, 1.0);
    dryLevelSlider.setValue(0.4);
    dryLevelSlider.setSliderStyle(juce::Slider::LinearVertical);
    dryLevelSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
    dryLevelSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::cornflowerblue);
    dryLevelSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::cornflowerblue);
    dryLevelSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::cornflowerblue);
    dryLevelSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    dryLevelSlider.setTextBoxIsEditable(true);
    dryLevelLabel.setText("Dry", juce::dontSendNotification);
    dryLevelLabel.setJustificationType(juce::Justification::centredTop);
    dryLevelLabel.setColour(juce::Label::textColourId, juce::Colours::cornflowerblue);

    // Reverb "Width" slider
    addAndMakeVisible(reverbWidthSlider);
    addAndMakeVisible(reverbWidthLabel);
    reverbWidthSlider.addListener(this);
    reverbWidthSlider.setRange(0.0, 1.0);
    reverbWidthSlider.setValue(1.0);
    reverbWidthSlider.setSliderStyle(juce::Slider::LinearVertical);
    reverbWidthSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
    // Colour: "Egyptian blue"
    // Attribution: https://htmlcolorcodes.com/colors/shades-of-blue/
    reverbWidthSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(20, 52, 164));
    reverbWidthSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour(20, 52, 164));
    reverbWidthSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour(20, 52, 164));
    reverbWidthSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    reverbWidthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    reverbWidthSlider.setTextBoxIsEditable(true);
    reverbWidthLabel.setText("Width", juce::dontSendNotification);
    reverbWidthLabel.setJustificationType(juce::Justification::centredTop);
    reverbWidthLabel.setColour(juce::Label::textColourId, juce::Colours::cornflowerblue);

    // Reverb "Freeze Mode" slider
    addAndMakeVisible(freezeModeSlider);
    addAndMakeVisible(freezeModeLabel);
    freezeModeSlider.addListener(this);
    freezeModeSlider.setRange(0.0, 1.0);
    freezeModeSlider.setValue(0.0);
    freezeModeSlider.setSliderStyle(juce::Slider::LinearVertical);
    freezeModeSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
    // Colour: "dark midnight blue"
    // Attribution: https://www.colorhexa.com/003366
    freezeModeSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(0, 20, 40));
    freezeModeSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour(0, 20, 40));
    freezeModeSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour(0, 20, 40));
    freezeModeSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    freezeModeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getParentWidth(), 30);
    freezeModeSlider.setTextBoxIsEditable(true);
    freezeModeLabel.setText("Freeze", juce::dontSendNotification);
    freezeModeLabel.setJustificationType(juce::Justification::centredTop);
    freezeModeLabel.setColour(juce::Label::textColourId, juce::Colour(0, 20, 40));

    // The Reset Parameters to Default button
    addAndMakeVisible(resetParamsButton);
    resetParamsButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
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

    // Coordinate/Data Processing for ColourGradient Creation
    //========Functionality which gets the corners of the ReverbEffects Component to create a background colourGradient============
    // Gets the whole component's bounds
    juce::Rectangle bounds = getLocalBounds();
    // Point class: stores the x and y coords for top-left corner of this component
    juce::Point ptTopLeft = bounds.getTopLeft();
    // Stores the x-coordinate of the top-left corner of this component
    int ptTopLeftX = ptTopLeft.getX();
    // Converts the x-coordinate of the top-left corner to a float, so it can pass as a param to colourGradient
    float ptTopLeftXFloat = (float)ptTopLeftX;
    // Stores the y-coordinate of the top-left corner of this component
    int ptTopLeftY = ptTopLeft.getY();
    // Converts the y-coordinate of the top-left corner to a float
    float ptTopLeftYFloat = (float)ptTopLeftY;
    // Stores the x and y coords for bottom-right corner of this component
    juce::Point ptBottomRight = bounds.getBottomRight();
    // Stores the x-coordinate of the bottom-right corner of this component
    int ptBottomRightX = ptBottomRight.getX();
    // Converts the x-coordinate of the bottom-right corner to a float
    float ptBottomRightXFloat = (float)ptBottomRightX;
    // Stores the y-coordinate of the bottom-right corner of this component
    int ptBottomRightY = ptBottomRight.getY();
    // Converts the y-coordinate of the bottom-right corner to a float
    float ptBottomRightYFloat = (float)ptBottomRightY;

    // Creates a greyblack ColourGradient object using the coordinates stored above for the background
    juce::ColourGradient gradient(juce::Colours::black, ptTopLeftXFloat,
        ptTopLeftYFloat, juce::Colours::grey, ptBottomRightXFloat,
        ptBottomRightYFloat, true
    );

    // Sets the gradient as the current fill
    g.setGradientFill(gradient);
    // Fills a rectangle the size of the whole main component with the gradient fill
    juce::Rectangle background{ 0, 0, (int)((getWidth() * 0.96)), (int)((getHeight() * 0.98))};
    g.fillRect(background);
}

/**
 *Called when this component's size has been changed
 *Draws the different reverb effect sliders
 */
void ReverbEffects::resized()
{
    // Splits the component horizontally into 7 columns with one for each slider
    double colW = getWidth() / 7;
    roomSizeLabel.setBounds(getWidth() * 0.02, 0, colW, getHeight() * 0.1);
    roomSizeSlider.setBounds(getWidth() * 0.02, getHeight() * 0.1, colW, getHeight() * 0.7);
    dampingLabel.setBounds(colW * 1.12, 0, colW, getHeight() * 0.1);
    dampingSlider.setBounds(colW * 1.12, getHeight() * 0.1, colW, getHeight() * 0.7);
    wetLevelLabel.setBounds(colW * 2.12, 0, colW, getHeight() * 0.1);
    wetLevelSlider.setBounds(colW * 2.12, getHeight() * 0.1, colW, getHeight() * 0.7);
    dryLevelLabel.setBounds(colW * 3.12, 0, colW, getHeight() * 0.1);
    dryLevelSlider.setBounds(colW * 3.12, getHeight() * 0.1, colW, getHeight() * 0.7);
    reverbWidthLabel.setBounds(colW * 4.12, 0, colW, getHeight() * 0.1);
    reverbWidthSlider.setBounds(colW * 4.12, getHeight() * 0.1, colW, getHeight() * 0.7);
    freezeModeLabel.setBounds(colW * 5.12, 0, colW, getHeight() * 0.1);
    freezeModeSlider.setBounds(colW * 5.12, getHeight() * 0.1, colW, getHeight() * 0.7);

    // At the bottom of all the sliders, set the restore to defaults button
    resetParamsButton.setBounds(getWidth() * 0.03, getHeight() * 0.87, getWidth() * 0.9, getHeight() * 0.09);
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
