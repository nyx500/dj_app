#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "Fader.h"
#include "ReverbEffects.h"

//==============================================================================
class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    /** 
     *Constructor: takes in a pointer to one of the DJAudioPlayer instances in MainComponent, the MainComponent's
     * single formatManager, the thumbnail cache, the title of the DeckGUI to print as text on this component, and
     * the custom tech font by reference
    */
    DeckGUI(
        DJAudioPlayer* _player,
        juce::AudioFormatManager& formatManagerToUse, // Pass in these args from mainComponent to use the data in the AudioThumbnail
        juce::AudioThumbnailCache& cacheToUse,
        std::string _deckTitle,
        juce::Font& _techFont);

    ~DeckGUI() override;

    /**
     *The paint() method gets called when a region of a component needs redrawing,
     *either because the component's repaint() method has been called, or because something
     *has happened on the screen that means a section of a window needs to be redrawn
    */
    void paint(juce::Graphics&) override;

    /** Called when this component's size has been changed */
    void resized() override;

    /** Implements the pure virtual function that cames with the Button listener which this class inherits from */
    void buttonClicked(juce::Button* button) override;

    /** Implements the pure virtual function that cames with the Slider listener which this class inherits from */
    void sliderValueChanged(juce::Slider* slider) override;

    /** FileDragAndDropTarget virtual function: callback to check whether this target is interested in the set of files being offered*/
    bool isInterestedInFileDrag(const juce::StringArray& iles) override;

    /** FileDragAndDropTarget virtual function: callback to indicate that the user has dropped the files onto this component*/
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    /** Implements juce::Timer's inherited pure virtual function: callback routine that actually gets called periodically */
    void timerCallback() override;

    /** Passes an audio file's URL from the PlaylistComponent which owns this DeckGUI into the DeckGUI's DJAudioPlayer */
    void loadTrack(juce::URL chosenFile);

private:
    // Your private member variables go here...
    
    // Loads "Play"/"Pause" icons from the Project's "SOURCE" directory
    juce::File playImageFile = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" }).getChildFile("play.png");
    juce::File pauseImageFile = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" }).getChildFile("pause.png");
    
    // Converts the icons files into a juce Image format
    juce::Image playImage = juce::PNGImageFormat::loadFrom(playImageFile);
    juce::Image pauseImage = juce::PNGImageFormat::loadFrom(pauseImageFile);

    // Create two image buttons for Play and Pause
    juce::ImageButton playButton{ "PLAY" };
    juce::ImageButton pauseButton{ "PAUSE" };

    // Create volume, speed, and relative position sliders and their respective labels
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    juce::Label volLabel;
    juce::Label speedLabel;
    juce::Label posLabel;

    // Attribution: File Choose object to select audio file from local disk https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    juce::FileChooser fChooser{ "Select a file..." };

    // Pointer to a DJAudioPlayer to transmit information received from button/slider listeners into the player
    DJAudioPlayer* player;

    // A Component Class instance which stores buttons to auto fade-in/fade-out the music, as well as a fade-speed slider
    Fader fader{ player };

    // Stores the sliders for different reverb effects applied to the music
    ReverbEffects reverbEffects{ player };

    // Creates and stores an instance of the WaveformDisplay Class Component to show thumbnail of the track data
    WaveformDisplay waveformDisplay;

    // Stores the name of this DeckGUI (e.g. "D1" for DeckGUI 1)
    std::string deckTitle;

    // Stores the custom impact fantasy-font for the titles
    juce::Font techFont;

    // A macro which handles memory leaks
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
