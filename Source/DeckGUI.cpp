#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(
    DJAudioPlayer* _player,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse
) : player(_player), // Set the player pointer up
waveformDisplay(formatManagerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    // Add the Waveform component
    addAndMakeVisible(waveformDisplay);
    // Add the Fader component
    addAndMakeVisible(fader);

    // Add the Deck GUI as a listener to its own buttons
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
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
    double rowH = getHeight() / 8;
    // Put play and stop buttons next to each other
    playButton.setBounds(0, 0, getWidth() / 2, rowH);
    stopButton.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    //// Add the fader display (itisa pointer to the fader, so use '->' syntax here)
    fader.setBounds(0, rowH, getWidth(), rowH);
    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    // WaveformDisplay should take up two rows (make height double the row height)
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2); // Takes up two rows in the display
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
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
    if (button == &loadButton)
    {

        // this does work in 6.1 but the syntax is quite counter-intuitive
        // Link: https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
        // - configure the dialogue
        auto fileChooserFlags =
            juce::FileBrowserComponent::canSelectFiles;
        // - launch out of the main thread
        // - note how we use a lambda function which you've probably
        // not seen before. Please do not worry too much about that. 
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                auto chosenFile = chooser.getResult();
                /* Loads URL into the player using the loadURL method */
                player->loadURL(juce::URL{ chosenFile });
                /** Does the same on the waveformDisplay, except it's not a pointer */
                waveformDisplay.loadURL(juce::URL{ chosenFile });
                waveformDisplay.setTrackLengthInSeconds(player->getTrackLengthInSeconds());
            });
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
        DBG(slider->getValue());
        player->setSpeed(slider->getValue());
    }


    if (slider == &posSlider)
    {
        DBG(slider->getValue());
        player->setPositionRelative(slider->getValue());
    }
}



// Drag and drop functions
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    DBG("DeckGUI::filesDropped");

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