/**Attribution for font typeface: https://www.fontspace.com/robot-invaders-font-f70280 */

#include "MainComponent.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"

//==============================================================================
MainComponent::MainComponent()
{   
    // Sets efault size of the whole program
    setSize(800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio) && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted)
            { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels to open
        setAudioChannels(0, 2);
    }

    // Displays the 2 DeckGUI components
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    // Displays the Playlist Component (Music Library)
    addAndMakeVisible(playlistComponent);

    // Registers basic audio formats
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================

/** Tells the audio source to prepare for playing */
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{   
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

/** Called repeatedly to fetch subsequent blocks of audio data */
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

/** Allows the audio source to release anything it no longer needs after playback has stopped. */
void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
/**
 *The paint() method gets called when a region of a component needs redrawing,
 *either because the component's repaint() method has been called, or because something
 *has happened on the screen that means a section of a window needs to be redrawn
*/
void MainComponent::paint(juce::Graphics& g)
{   
    
    g.drawImage(backgroundImg, getLocalBounds().toFloat());
    //=========================================================================================================================

    // Sets up the custom Robot Invader font
    g.setFont(techFont);
    g.setFont(40.0);
    g.setColour(juce::Colours::white);
    // Draws the title of the program at the top of the Main Component ("OtoDecks")
    g.drawText("OtoDecks", getWidth() * 0.25, 0, getWidth() * 0.5, getHeight() * 0.1, juce::Justification::centredTop, true);
}


/** Called when this component's size has been changed */
void MainComponent::resized()
{   
    // Left-hand side, 40% of total width: Playlist Component
    playlistComponent.setBounds(getWidth() * 0.01, getHeight() * 0.08, getWidth() * 0.4, getHeight() * 0.9);
    // Right-hand side, 60% of total width: Two Deck GUIs stacked in a column, with Deck GUI 1 on top
    deckGUI1.setBounds(getWidth() * 0.42, getHeight() * 0.08, getWidth() * 0.57, getHeight() * 0.43);
    deckGUI2.setBounds(getWidth() * 0.42, getHeight() * 0.55, getWidth() * 0.57, getHeight() * 0.43);
}
