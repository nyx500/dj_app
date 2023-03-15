/**Attribution for font typeface: https://www.fontspace.com/robot-invaders-font-f70280 */

#include "MainComponent.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

    // Displays the 2 deckGUI properties
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    // Displays the playlist sub-component
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();

    // Set up the custom font
    // Attribution: https://forum.juce.com/t/modern-custom-font-guide/20841/3
    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::RobotInvaders_ttf, BinaryData::RobotInvaders_ttfSize);
    techFont = juce::Font(typeface);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();

}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();

}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    juce::Rectangle bounds = getLocalBounds();
    juce::Point ptTopLeft = bounds.getTopLeft();
    int ptTopLeftX = ptTopLeft.getX();
    float ptTopLeftXFloat = (float)ptTopLeftX;
    int ptTopLeftY = ptTopLeft.getY();
    float ptTopLeftYFloat = (float)ptTopLeftY;
    juce::Point ptBottomRight = bounds.getBottomRight();
    int ptBottomRightX = ptBottomRight.getX();
    float ptBottomRightXFloat = (float)ptBottomRightX;
    int ptBottomRightY = ptBottomRight.getY();
    float ptBottomRightYFloat = (float)ptBottomRightY;

    juce::ColourGradient gradient(juce::Colours::black, ptTopLeftXFloat, ptTopLeftYFloat, juce::Colours::navy, ptBottomRightXFloat, ptBottomRightYFloat, true);
    g.setGradientFill(gradient);
    juce::Rectangle background(0, 0, getWidth(), getHeight());
    g.fillRect(background);

    g.setFont(techFont);
    g.setFont(40.0);
    g.setColour(juce::Colours::white);
    g.drawText("OtoDecks", getWidth() * 0.25, 0, getWidth() * 0.5, getHeight() * 0.1, juce::Justification::centredTop, true);

}

// Runs when app first opens or user resizes the app
void MainComponent::resized()
{   

    playlistComponent.setBounds(0, getHeight() * 0.08, getWidth() * 0.3, getHeight() * 0.9);

    deckGUI1.setBounds(getWidth() * 0.32, getHeight() * 0.08, getWidth() * 0.66, getHeight() * 0.42);
    deckGUI2.setBounds(getWidth() * 0.32, getHeight() * 0.52, getWidth() * 0.66, getHeight() * 0.42);



}
