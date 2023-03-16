#pragma once



#include <JuceHeader.h>;
#include "DJAudioPlayer.h";
#include "DeckGUI.h";
#include "PlaylistComponent.h";
#include "Fader.h";


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //====================Audio Source Virtual Functions Implementation=============
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
        // Your private member variables go here...

    // Instantiate the paramaters for the AudioThumbnail class in the WaveformDisplay
    // in the DeckGUI components
    juce::AudioFormatManager formatManager;
    // The cache can store 100 cached waveforms at a time -->
    // This means that as the program runs, if we load 100 different files
    // it will cache all of them, and it will throw the first one away
    // once the number has been exceeded
    juce::AudioThumbnailCache thumbCache{ 100 };

    juce::MixerAudioSource mixerSource;

    // Set up the custom font
    // Attribution: https://forum.juce.com/t/modern-custom-font-guide/20841/3
    juce::Font techFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::RobotInvaders_ttf, BinaryData::RobotInvaders_ttfSize));

    // Pass in the formatManager (a reference!!!) into the DJAudioPlayers
    DJAudioPlayer player1{ formatManager };
    // Pass in the data components into the DeckGUI instances
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache, "D1", techFont };
    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache, "D2", techFont };

    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2, techFont };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
