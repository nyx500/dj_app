#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "Fader.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener, // inherit listeners
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    // Pass a pointer to the existing DJAudioPlayer to the constructor function in the DeckGUI class
    DeckGUI(
        DJAudioPlayer* _player,
        juce::AudioFormatManager& formatManagerToUse, // Pass in these args from mainComponent to use the data in the AudioThumbnail
        juce::AudioThumbnailCache& cacheToUse
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

    // Implement juce::Timer pure virtual function
    void timerCallback() override;


private:
    // Your private member variables go here...
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;


    // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    juce::FileChooser fChooser{ "Select a file..." };

    // Can send messages from listeners to DJAudioPlayer now
    // Create a pointer 
    // Pass the player into the constructor
    DJAudioPlayer* player;

    Fader fader{};

    // Creates an instance of the WaveformDisplay class component
    WaveformDisplay waveformDisplay;

    // A macro which handles memory leaks
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
