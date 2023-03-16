#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================

/** Constructor: takes in a pointer to one of the DJAudioPlayer instances in MainComponent, the MainComponent's
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
    // Resize the play/pause icons stored as juce::Image data members
    playImage = playImage.rescaled(80, 80, juce::Graphics::mediumResamplingQuality);
    pauseImage = pauseImage.rescaled(80, 80, juce::Graphics::mediumResamplingQuality);

    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Add icons
    playButton.setImages(true, true, true, playImage, 1.0, juce::Colour(), juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(), 1.0, juce::Colour(255, 255, 255), 0.0f);
    pauseButton.setImages(true, true, true, pauseImage, 1.0, juce::Colour(), juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(), 1.0, juce::Colour(255, 255, 255), 0.0f);

    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);

    addAndMakeVisible(volSlider);
    volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour());
    volSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getParentWidth() * 0.5, getParentHeight() * 2);
    volSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(speedSlider);
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour());
    speedSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getParentWidth() * 0.5, getParentHeight() * 2);
    speedSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(posSlider);
    posSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour());
    posSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getParentWidth() * 0.5, getParentHeight() * 2);
    posSlider.setTextBoxIsEditable(true);

    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);

    volLabel.setText("Volume", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);
    posLabel.setText("Position", juce::dontSendNotification);

    volLabel.setJustificationType(juce::Justification::centred);
    speedLabel.setJustificationType(juce::Justification::centred);
    posLabel.setJustificationType(juce::Justification::centred);

    //// Add the Waveform component
    addAndMakeVisible(waveformDisplay);
    // Add the Fader component
    addAndMakeVisible(fader);
    // Add the ReverbEffects component
    addAndMakeVisible(reverbEffects);

    // Add the Deck GUI as a listener to its own buttons
    playButton.addListener(this);
    pauseButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // Set slider ranges
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);

    // Set slider default positions
    volSlider.setValue(0.5);
    speedSlider.setValue(1.0);

    // Argument: how often to call the timerCallback function
    startTimer(10); //'100' means 100 milliseconds (tenth of a a second)
}

DeckGUI::~DeckGUI()
{
    // Stop the timer when destructor is called
    stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    // Set background to transparent/null colour
    g.fillAll(juce::Colour());

    // Draw a black rounded rectangle of same size as this component
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 10.0);
    // Draw a white outline around the above rectangle
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 10.0, 4.0);

    g.setColour(juce::Colour(14, 135, 250));
    g.setFont(40.0);
    g.setFont(techFont);
    g.drawSingleLineText(juce::String(deckTitle), getWidth() * 0.05, getHeight() * 0.96, juce::Justification::left);
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 7;
    // Put play and stop buttons next to each other
    playButton.setBounds(getWidth() * 0.05, rowH * 0.25, getWidth() * 0.1, rowH);
    pauseButton.setBounds(getWidth() * 0.18, rowH * 0.25, getWidth() * 0.1, rowH);
    waveformDisplay.setBounds(getWidth() * 0.3, rowH * 0.25, getWidth() * 0.68, rowH);
    fader.setBounds(getWidth() * 0.02, rowH * 1.5, getWidth() * 0.3, rowH * 5);
    volSlider.setBounds(getWidth() * 0.34, rowH * 1.5, getWidth() * 0.2, rowH * 0.8);
    volLabel.setBounds(getWidth() * 0.34, rowH * 2.3, getWidth() * 0.2, rowH / 2);
    speedSlider.setBounds(getWidth() * 0.34, rowH * 3.5, getWidth() * 0.2, rowH * 0.8);
    speedLabel.setBounds(getWidth() * 0.34, rowH * 4.3, getWidth() * 0.2, rowH / 2);
    posSlider.setBounds(getWidth() * 0.34, rowH * 5.4, getWidth() * 0.2, rowH);
    posLabel.setBounds(getWidth() * 0.34, rowH * 6.3, getWidth() * 0.2, rowH / 2);
    reverbEffects.setBounds(getWidth() * 0.54, rowH * 1.5, getWidth() * 0.47, getHeight() - rowH * 1.6);
}

/** Implement button listener */
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

// Drag and drop functions
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    // DBG("DeckGUI::filesDropped");

    // Only want one file
    if (files.size() == 1)
    {
        // Convert a string filename into a file then into a URL
        player->loadURL(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    volSlider.setValue(player->getGain());
}

/** A file URL is passed in from the Playlist Component and loaded into the DJAudioplayer */
void DeckGUI::loadTrack(juce::URL chosenFile)
{
    //// this does work in 6.1 but the syntax is quite counter-intuitive
    //   // Link: https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    //   // - configure the dialogue
    // auto fileChooserFlags =
    //    juce::FileBrowserComponent::canSelectFiles;
    //// - launch out of the main thread
    //// - note how we use a lambda function which you've probably
    //// not seen before. Please do not worry too much about that.
    // fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
    //     {
    //         auto chosenFile = chooser.getResult();

    //        // Gets the filename as a Juce String
    //        juce::String fileNameAsJuceString = chosenFile.getFileName();
    //        // Gets the filename as a std string
    //        std::string fileName = fileNameAsJuceString.toStdString();
    //        DBG(fileName);

    //        /* Loads URL into the player using the loadURL method */
    //        player->loadURL(juce::URL{ chosenFile });
    //        /** Does the same on the waveformDisplay, except it's not a pointer */
    //        waveformDisplay.loadURL(juce::URL{ chosenFile });
    //        waveformDisplay.setTrackLengthInSeconds(player->getTrackLengthInSeconds());
    //    });
    /* Loads URL into the player using the loadURL method */
    player->loadURL(chosenFile);
    /** Does the same on the waveformDisplay, except it's not a pointer */
    waveformDisplay.loadURL(chosenFile);
    waveformDisplay.setTrackLengthInSeconds(player->getTrackLengthInSeconds());
}