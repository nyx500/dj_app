#include <cmath>
#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse,
    juce::Font& _techFont) : // First argument (sourceSamplesPerThumbnailSample): use 1000 points to plot the waveform
    // (an audio file has millions of points) --> "downsampling"
    // This is a measure of the resolution of the image.
    // The lower the value, the higher the resolution.
    audioThumb(1000, formatManagerToUse, cacheToUse),
    audioThumbPlayed(1000, formatManagerToUse, cacheToUse),
    fileLoaded(false), // default value for fileLoaded has to be set to "false"
    position(0),
    trackLength(0),
    techFont(_techFont)
{
    // Register the ChangeListener with the audioThumbnail so it draws automatically
    audioThumb.addChangeListener(this);
    audioThumbPlayed.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

/**
 *The paint() method gets called when a region of a component needs redrawing,
 *either because the component's repaint() method has been called, or because something
 *has happened on the screen that means a section of a window needs to be redrawn
 */
void WaveformDisplay::paint(juce::Graphics& g)
{
    // Set black background
    g.fillAll(juce::Colours::black);

    // Outline component in blue
    g.setColour(juce::Colours::blue);
    g.drawRect(getLocalBounds(), 1.0); // draw an outline around the component

    g.setColour(juce::Colours::white);
    // Draw the waveform/audio thumb only if the file has loaded successfully
    if (fileLoaded)
    {
        // AudioThumbnail has a complicated function called ::drawChannel
        // Args: Graphics& g (basically the graphics object/canvas that we can call funcs on to draw on it)
        // The g canvas represents the bit of screen we are allowed to draw on (the bounds of this component)
        // We want to take up the whole of the component (2nd arg --> how much space to take up)
        audioThumb.drawChannel(
            g,
            getLocalBounds(),
            0,                           // Start time of the audio file
            audioThumb.getTotalLength(), // End of audio file (i.e. total length in seconds)
            0,                           // Channel number
            0.8                          // "Vertical-zoom factor" --> boosts up the sound if it's really quiet. If 1 = fills up the whole container.

        );

        // Set semi-opaque rectangle over section of audio thumbnail that has already been played to demonstrate the progress
        juce::Rectangle playedSectionRect = juce::Rectangle((int)(getWidth() * 0.1), (int)(getHeight() * 0.1), (int)(position * getWidth()), getHeight());
        g.setColour(juce::Colours::black.withAlpha(0.5f));
        g.fillRect(0, 0, position * getWidth(), getHeight());

        // Draws the playhead on the waveform in blue
        g.setColour(juce::Colours::blue);

        g.fillRect(getWidth() * position, 0, 3, getHeight());

        // Text stating how many minutes/seconds have elapseds
        g.setFont(20.0f);
        double timeInSeconds = position * trackLength;
        g.drawText(displayTimeAsString(timeInSeconds), 0, 0, 100, 100, 9, true);
    }
    else // File is not loaded
    {
        g.setColour(juce::Colours::blue);
        g.setFont(20.0f);
        g.setFont(techFont);
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true); // Draw some placeholder message when file not loaded
    }
}

/** Called when this component's size has been changed */
void WaveformDisplay::resized()
{
}

/* Loads the URL selected by the user into the thumbnail as well as into the audio player*/
void WaveformDisplay::loadURL(juce::URL audioURL)
{

    // Clears the (drawer) audioThumb in case it drew anything previously
    audioThumb.clear();
    audioThumbPlayed.clear();

    /**Passes the audioURL into the audioThumb for the display.
     * We need to "unpack" the URL and turn it into an
     * input source and call setSource on the thumbnail.
     * This function returns "true" if audioURL stream actually loads.
     */
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL)); // Class-scope variable
    fileLoaded = audioThumbPlayed.setSource(new juce::URLInputSource(audioURL));
}

/**Virtual method inherited from ChangeListener to implement automatic waveform drawing*/
void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    // Calls the paint() method again
    repaint();
}

/** Sets the relative position of the playhead */
void WaveformDisplay::setPositionRelative(double pos)
{
    // What to do when the waveform receives the position?
    // 1) We COULD call repaint() to update it
    // 2) We COULD just repaint() only if the position has changed
    // We need to store the position because when we call paint
    // it happens in a separate thread, so we are putting in a request
    // for sometime in the future for paint() to be called.
    // Therefore, the position needs to be stored locally, so that when
    // 'paint' gets called, it has a position to look at.
    // We need a local variable inside this object storing position.
    if (pos != position && pos > 0)
    {
        // Only repaint if the position has been updated
        position = pos;
        repaint();
    }
}

/** Sets the length of the track in seconds */
void WaveformDisplay::setTrackLengthInSeconds(double length)
{
    trackLength = length;
}

/** Converts time in seconds to time in mins and seconds as a string */
std::string WaveformDisplay::displayTimeAsString(double timeInSeconds)
{
    int minutes = (int)(timeInSeconds / 60);

    int seconds = std::floor(timeInSeconds);
    seconds = seconds % 60;

    std::string timeAsString = "";

    if (minutes < 10)
    {
        timeAsString += "0" + std::to_string(minutes) + ":";
    }
    else
    {
        timeAsString += std::to_string(minutes) + ":";
    }

    if (seconds < 10)
    {
        timeAsString += "0" + std::to_string(seconds);
    }
    else
    {
        timeAsString += std::to_string(seconds);
    }

    return timeAsString;
}
