#pragma once

#include <JuceHeader.h>;
#include "DJAudioPlayer.h";
#include "DeckGUI.h";
#include "PlaylistComponent.h";

//======================================================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==================================================================================================
    MainComponent();
    ~MainComponent() override;

    //====================Audio Source Virtual Functions Implementation=================================
    /** Tells the audio source to prepare for playing */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    /** Called repeatedly to fetch subsequent blocks of audio data */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    /** Allows the audio source to release anything it no longer needs after playback has stopped. */
    void releaseResources() override;

    //===============================Component Methods==================================================
    /** 
     *The paint() method gets called when a region of a component needs redrawing, 
     *either because the component's repaint() method has been called, or because something
     *has happened on the screen that means a section of a window needs to be redrawn 
    */
    void paint(juce::Graphics& g) override;
    /** Called when this component's size has been changed */
    void resized() override;

private:
    // Your private member variables go here...


    //=============================Audio Functionality=================================================
    
    // Instantiates the paramaters for the AudioThumbnail class in the WaveformDisplay in DeckGUIs
    juce::AudioFormatManager formatManager;
    /** 
     *If we load 100 different files the program it will cache all of them, and it will throw the first one away
     *once the number has been exceeded
    */
    juce::AudioThumbnailCache thumbCache{ 100 };
    /** An AudioSource that mixes together the output of a set of other AudioSources */
    juce::MixerAudioSource mixerSource;

    //========================Custom Font Setup==========================================================
    
    /** 
     *Sets up the custom "RobotInvaders" techy font
     *Attribution: https://forum.juce.com/t/modern-custom-font-guide/20841/3
    */
    juce::Font techFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::RobotInvaders_ttf, BinaryData::RobotInvaders_ttfSize));


    //==================================DJAudioPlayer Instances============================================
    
    // Passes in the formatManager by reference into the DJAudioPlayers
    DJAudioPlayer player1{ formatManager };
    DJAudioPlayer player2{ formatManager };


    //================================DeckGUI Instances====================================================
    /**
     *DeckGUIs set up : each takes a DJAudioPlayer, the formatManager, the string
     * with the name of the DeckGUI (D1 for Deck GUI 1 and D2 for Deck GUI 2),
     * and the font
    */
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache, "D1", techFont };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache, "D2", techFont };

    //==============================Playlist Component=======================================================

    /** Sets up and stores the playlist (music library for loading in and storing audio files from the local drive) */
    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2, techFont };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
