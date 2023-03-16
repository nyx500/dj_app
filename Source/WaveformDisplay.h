#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay : public juce::Component, // This inherits from general JUCE Component class
    public juce::ChangeListener // For the AudioThumbnail to draw automatically when a file is loaded
{
public:
    WaveformDisplay(
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse,
        juce::Font& _techFont
    );
    ~WaveformDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    /* Loads the URL selected by the user into the thumbnail as well as into the audio player*/
    void loadURL(juce::URL audioURL);

    /**Virtual method inherited from ChangeListener to implement automatic waveform drawing*/
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /** Sets the relative position of the playhead */
    void setPositionRelative(double pos);

    /** Sets the length of the track in seconds */
    void setTrackLengthInSeconds(double length);

private:
    // Create the AudioThumbnail class instance for the whole track
    juce::AudioThumbnail audioThumb;

    // Create the AudioThumbnail for the played area
    juce::AudioThumbnail audioThumbPlayed;

    // Checks if the file has successfully loaded
    bool fileLoaded;

    double position;

    double trackLength;

    /** Converts time in seconds to time in mins and seconds as a string */
    std::string displayTimeAsString(double timeInSeconds);

    // Custom font
    juce::Font techFont;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
