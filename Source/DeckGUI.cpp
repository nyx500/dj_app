#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(
    DJAudioPlayer* _player,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse
) : player(_player), // Sets the player pointer up
waveformDisplay(formatManagerToUse, cacheToUse)
{   
    // Resize the play/pause icons stored as juce::Image data members
    playImage = playImage.rescaled(80, 80, juce::Graphics::mediumResamplingQuality);
    pauseImage = pauseImage.rescaled(80, 80, juce::Graphics::mediumResamplingQuality);


    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Add icons
    playButton.setImages(true, true, true, playImage, 1.0, juce::Colour(), juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(), 1.0, juce::Colour(255, 255, 255), 0.0f);
    stopButton.setImages(true, true, true, pauseImage, 1.0, juce::Colour(), juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(), 1.0, juce::Colour(255, 255, 255), 0.0f);

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    //// Add the Waveform component
    addAndMakeVisible(waveformDisplay);
    // Add the Fader component
    addAndMakeVisible(fader);
    // Add the ReverbEffects component
    addAndMakeVisible(reverbEffects);

    // Add the Deck GUI as a listener to its own buttons
    playButton.addListener(this);
    stopButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);

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

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("DeckGUI", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 7;
    // Put play and stop buttons next to each other
    playButton.setBounds(getWidth() * 0.05, rowH * 0.25, getWidth() * 0.1, rowH);
    stopButton.setBounds(getWidth() * 0.18, rowH * 0.25, getWidth() * 0.1, rowH);
    waveformDisplay.setBounds(getWidth() * 0.3, 0, getWidth() * 0.7, rowH * 1.5);
    //// Add the fader display (itisa pointer to the fader, so use '->' syntax here)
    fader.setBounds(0, rowH * 1.5, getWidth(), rowH * 2);
    /*volSlider.setBounds(0, rowH * 3, getWidth(), rowH / 2);
    speedSlider.setBounds(0, rowH * 3.5, getWidth(), rowH / 2);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH / 2);
    reverbEffects.setBounds(0, rowH * 5, getWidth(), rowH * 2);*/
}


/** Implement button listener */
void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->play();
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
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
    //DBG("DeckGUI::filesDropped");

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
}


/** A file URL is passed in from the Playlist Component and loaded into the DJAudioplayer */
void DeckGUI::loadTrack(juce::URL chosenFile)
{
    //// this does work in 6.1 but the syntax is quite counter-intuitive
    //   // Link: https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    //   // - configure the dialogue
    //auto fileChooserFlags =
    //    juce::FileBrowserComponent::canSelectFiles;
    //// - launch out of the main thread
    //// - note how we use a lambda function which you've probably
    //// not seen before. Please do not worry too much about that. 
    //fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
    //    {
    //        auto chosenFile = chooser.getResult();

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