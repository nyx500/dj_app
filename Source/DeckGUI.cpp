#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================

/** 
 *Constructor: takes in a pointer to one of the DJAudioPlayer instances in MainComponent, the MainComponent's
 * single formatManager, the thumbnail cache, the title of the DeckGUI to print as text on this component, and
 * the custom tech font by reference
*/
DeckGUI::DeckGUI(
    DJAudioPlayer* _player,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse,
    std::string _deckTitle,
    juce::Font& _techFont) : 
        player(_player),
        waveformDisplay(formatManagerToUse, cacheToUse, _techFont),
        deckTitle(_deckTitle),
        techFont(_techFont)
{
    // Sets the size of the the Play/Pause icons stored as juce::Image data members to 80x80 pixels
    playImage = playImage.rescaled(80, 80, juce::Graphics::mediumResamplingQuality);
    pauseImage = pauseImage.rescaled(80, 80, juce::Graphics::mediumResamplingQuality);

    // Add the icons to the ImageButtons
    playButton.setImages(true, true, true, playImage, 1.0, juce::Colour(),
        juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(),
        1.0, juce::Colour(255, 255, 255), 0.0f
    );
    pauseButton.setImages(true, true, true, pauseImage, 1.0, juce::Colour(),
        juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(),
        1.0, juce::Colour(255, 255, 255), 0.0f
    );

    // Add those image buttons (play/pause) to the DeckGUI
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);

    //==========================Volume/Speed/Position sliders configuration======================================

    // Volume slider
    addAndMakeVisible(volSlider);
    // Hide the border around the current value textbox
    volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour());
    // Make the slider track white for more contrast
    volSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
    // Put text storing the current numerical value of the slider on top of the slider
    volSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getParentWidth() * 0.5, getParentHeight() * 2);
    // Allow user to set slider value by entering text into this textbox as well as by adjusting the slider
    volSlider.setTextBoxIsEditable(true);
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5);

    // Speed slider
    addAndMakeVisible(speedSlider);
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour());
    speedSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getParentWidth() * 0.5, getParentHeight() * 2);
    speedSlider.setTextBoxIsEditable(true);
    speedSlider.setRange(0.0, 10.0);
    speedSlider.setValue(1.0);

    // Position slider
    addAndMakeVisible(posSlider);
    posSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour());
    posSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getParentWidth() * 0.5, getParentHeight() * 2);
    posSlider.setTextBoxIsEditable(true);
    posSlider.setRange(0.0, 1.0);

    //==========================Volume/Speed/Position Slider Labels configuration======================================
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
    volLabel.setText("Volume", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);
    posLabel.setText("Position", juce::dontSendNotification);
    volLabel.setJustificationType(juce::Justification::centred);
    speedLabel.setJustificationType(juce::Justification::centred);
    posLabel.setJustificationType(juce::Justification::centred);
    
    //==========================Add custom components to the DecKGUI=====================================================
    addAndMakeVisible(volLabel);
    // Add the Waveform component to the DeckGUI
    addAndMakeVisible(waveformDisplay);

    // Add the Fader component to the DeckGUI
    addAndMakeVisible(fader);

    // Add the ReverbEffects component to the DeckGUI
    addAndMakeVisible(reverbEffects);

    //==========================Add the DeckGUI as a listener to the buttons and sliders===================================
    // Add the Deck GUI as a listener to its buttons and sliders
    playButton.addListener(this);
    pauseButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // TIMER CALLBACK: callback happens every 10 milliseconds (one hundredth of a second)
    startTimer(10); 
}

DeckGUI::~DeckGUI()
{
    // Stop the timer when the DeckGUI destructor is called
    stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
{
    /* 
       This demo code just fills the component's background and
       draws some placeholder text to get you started.
       You should replace everything in this method with your own
       drawing code.
    */

    // Set background to transparent/"null" colour
    g.fillAll(juce::Colour());

    // Draws a black rounded rectangle of same size as this component which will serve as the background
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 10.0);

    // Draws a white outline around the above rounded rectangle
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 10.0, 4.0);

    // Sets the text colour of the "D1/D2" (DeckGUI's name) to a bright medium blue colour with RGB 14,135,250
    g.setColour(juce::Colour(14, 135, 250));
    g.setFont(40.0);
    // Set the custom font
    g.setFont(techFont);
    // Draw the DeckGUI's name on the bottom left
    g.drawSingleLineText(juce::String(deckTitle), getWidth() * 0.05, getHeight() * 0.96, juce::Justification::left);
}


/** Called when this component's size has been changed */
void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    // Divide the DeckGUI into 7 main rows
    double rowH = getHeight() / 7;

    // Places the Play and Pause buttons next to each other on one line
    playButton.setBounds(getWidth() * 0.05, rowH * 0.25, getWidth() * 0.1, rowH);
    pauseButton.setBounds(getWidth() * 0.18, rowH * 0.25, getWidth() * 0.1, rowH);

    // The next row below the Play and Pause buttons will show the audio thumbnail/waveform
    waveformDisplay.setBounds(getWidth() * 0.3, rowH * 0.25, getWidth() * 0.68, rowH);

    // Left column underneath Play/Pause buttons and the audio waveform: for the Fader buttons/slider
    fader.setBounds(getWidth() * 0.02, rowH * 1.5, getWidth() * 0.3, rowH * 5);

    // Center column underneath Play/Pause buttons and the audio waveform: for the volume/speed/position sliders
    volSlider.setBounds(getWidth() * 0.34, rowH * 1.5, getWidth() * 0.2, rowH * 0.8);
    volLabel.setBounds(getWidth() * 0.34, rowH * 2.3, getWidth() * 0.2, rowH / 2);
    speedSlider.setBounds(getWidth() * 0.34, rowH * 3.5, getWidth() * 0.2, rowH * 0.8);
    speedLabel.setBounds(getWidth() * 0.34, rowH * 4.3, getWidth() * 0.2, rowH / 2);
    posSlider.setBounds(getWidth() * 0.34, rowH * 5.4, getWidth() * 0.2, rowH);
    posLabel.setBounds(getWidth() * 0.34, rowH * 6.3, getWidth() * 0.2, rowH / 2);

    // Right column underneath Play/Pause buttons and the audio waveform: for the Reverb Effects sliders
    reverbEffects.setBounds(getWidth() * 0.54, rowH * 1.5, getWidth() * 0.47, getHeight() - rowH * 1.6);
}


/** Implements the pure virtual function that cames with the Button listener which this class inherits from */
void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        player->play();
    }
    if (button == &pauseButton)
    {
        player->stop();
    }
}


/** Implements the pure virtual function that cames with the Slider listener which this class inherits from */
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{

    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}


/** FileDragAndDropTarget virtual function: callback to check whether this target is interested in the set of files being offered*/
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}


/** FileDragAndDropTarget virtual function: callback to indicate that the user has dropped the files onto this component*/
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    // Only want to get one file at a time!
    if (files.size() == 1)
    {
        // Converts the filename string into a file then into a URL
        player->loadURL(juce::URL{ juce::File{files[0]} });
    }
}


/** Implements juce::Timer's inherited pure virtual function: callback routine that actually gets called periodically */
void DeckGUI::timerCallback()
{   
    waveformDisplay.setPositionRelative(player->getPositionRelative());

    // When the user clicks on Fade In/Fade Out buttons, update the position of the volume slider as the volume increases/decreases
    volSlider.setValue(player->getGain());
}

/** Passes an audio file's URL from the PlaylistComponent which owns this DeckGUI into the DeckGUI's DJAudioPlayer */
void DeckGUI::loadTrack(juce::URL chosenFile)
{
    // Attribution: https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    // Loads the audio track URL into the player using it loadURL method
    player->loadURL(chosenFile);

    // Loads the audio track data into the waveform display instance
    waveformDisplay.loadURL(chosenFile);

    // Loads the length of the track into the waveformDisplay
    waveformDisplay.setTrackLengthInSeconds(player->getTrackLengthInSeconds());
}