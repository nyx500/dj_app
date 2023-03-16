#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "Fader.h"
#include "ReverbEffects.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    // Pass a pointer to the existing DJAudioPlayer to the constructor function in the DeckGUI class
    DeckGUI(
        DJAudioPlayer* _player,
        juce::AudioFormatManager& formatManagerToUse, // Pass in these args from mainComponent to use the data in the AudioThumbnail
        juce::AudioThumbnailCache& cacheToUse,
        std::string _deckTitle,
        juce::Font _techFont
    );
    ~DeckGUI() override;

    void paint(juce::Graphics&) override;
    void resized() override;


    /** Implement button listener: always write override when using something from another class */
    void buttonClicked(juce::Button* button) override; // [2] Need to override the pure virtual function or it won't work

    /**Implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;

    // Drag and drop functions
    bool isInterestedInFileDrag(const juce::StringArray& iles) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    // Implement juce::Timer's inherited pure virtual function
    void timerCallback() override;

    /** A file URL is passed in from the Playlist Component and loaded into the DJAudioplayer */
    void loadTrack(juce::URL chosenFile);


private:
    // Your private member variables go here...
    // 
    // Play and pause icons stored here

    // Load play/pause images from Source directory
    juce::File playImageFile = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" }).getChildFile("play.png");
    juce::File pauseImageFile = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" }).getChildFile("pause.png");
    // Converts the files to a juce Image format
    juce::Image playImage = juce::PNGImageFormat::loadFrom(playImageFile);
    juce::Image pauseImage = juce::PNGImageFormat::loadFrom(pauseImageFile);

    juce::ImageButton playButton{ "PLAY"};
    juce::ImageButton stopButton{ "STOP" };

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::Label volLabel;
    juce::Label speedLabel;
    juce::Label posLabel;

    // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    juce::FileChooser fChooser{ "Select a file..." };

    // Can send messages from listeners to DJAudioPlayer now
    // Create a pointer 
    // Pass the player into the constructor
    DJAudioPlayer* player;

    // Buttons to fade in/fade out the track
    Fader fader{player};

    // Stores buttons for reverb effects on the track
    ReverbEffects reverbEffects{player};

    // Creates an instance of the WaveformDisplay class component
    WaveformDisplay waveformDisplay;

    // Deck number/name
    std::string deckTitle;

    // Custom font
    juce::Font techFont;

    // A macro which handles memory leaks
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
