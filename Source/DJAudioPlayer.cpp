#include <../JuceLibraryCode/JuceHeader.h>
#include "DJAudioPlayer.h"

/** Constructor: takes in the program-scope formatManager from Main Component*/
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
    // Set up the reverb parameters
    juce::Reverb::Parameters params{};
    // Sets the reverb parameters to the defaults listed in the JUCE class reference
    // Attribution: https://docs.juce.com/master/structReverb_1_1Parameters.html
    params.roomSize = 0.5f;
    params.damping = 0.5f;
    params.wetLevel = 0.33f;
    params.dryLevel = 0.4f;
    params.width = 1.0f;
    params.freezeMode = 0.0f;
    reverbAudioSource.setParameters(params);

    // Call timerCallback every 10 milliseconds (one hundredth of a second)
    startTimer(10);
}

/** /Destructor */
DJAudioPlayer::~DJAudioPlayer()
{
}

//====================Audio Source Virtual Functions Implementation=============

// Birth
/** Tells the audio source to prepare for playing */
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // Reverb Audio Source for reverb effects, takes in the resampleSource, which takes in transportSource
    reverbAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

// Life
/** Called repeatedly to fetch subsequent blocks of audio data */
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{   
    // If no more data to get,or audio is paused, then clears the audio buffer
    if (readerSource.get() == nullptr || !playing)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    // Delegate responsibility to the reverbAudioSource which takes in the resampleSource as a parameter when created
    reverbAudioSource.getNextAudioBlock(bufferToFill);
}

// Death
/** Allows the audio source to release anything it no longer needs after playback has stopped. */
void DJAudioPlayer::releaseResources()
{   
    // Releases resources from all the three audio sources
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbAudioSource.releaseResources();
}
//==============================================================================

/** Loads in the selected audio file's URL to play the selected audio track and loads up an audio stream from the URL to the transportSource */
void DJAudioPlayer::loadURL(juce::URL audioURL)
{   
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr) // Good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

//================================Main Slider Controls==============================================
/** Sets the volume of the audio to the double passed in as a parameter*/
void DJAudioPlayer::setGain(double gain)
{
    // Makes sure volume is constrained between the bounds of 0.0 and 1.0
    if (getGain() < 0)
    {
        transportSource.setGain(0.0);
    }
    else if (getGain() > 1.0)
    {
        transportSource.setGain(1.0);
    }
    else
    {
        transportSource.setGain(gain);
    }
}


/** Returns the current volume of the audio track */
double DJAudioPlayer::getGain()
{
    return transportSource.getGain();
}


/** Sets the speed of the audio to the double passed in as a parameter */
void DJAudioPlayer::setSpeed(double ratio)
{   
    // Print out a DBG error message if the speed value is invalid (below 0 or over 100)
    if (ratio < 0 || ratio > 100.0)
    {
        DBG("DJAudioPlayer::setSpeed - Ratio should be between 0 and 100!");
    }
    else
    {   
        // Speed can only be changed on resampleSource and not the audio transportSource!
        resampleSource.setResamplingRatio(ratio);
    }
}

/** Sets the position inside the audio track to the double passed in as a parameter */
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

/**
 * Sets relative position, meaning a position between '0 and 1', instead of between 0 and "number of seconds".
 * This is because each track has a different length, so to keep slider the same for all the tracks, make position 0-1.s
*/
void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative - Pos should be between 0 and 1!");
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

//====================================Reverb Effects applied to ReverbAudioSource instance=============================
/** Sets the roomSize parameter of the reverb effect */
void DJAudioPlayer::setReverbRoomSize(double roomSize)
{
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();
    params.roomSize = roomSize;
    reverbAudioSource.setParameters(params);
}

/** Sets the damping property of the reverb effect */
void DJAudioPlayer::setReverbDamping(double dampingLevel)
{
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();
    params.damping = dampingLevel;
    reverbAudioSource.setParameters(params);
}

/** Sets the "wet level" property of the reverb effect */
void DJAudioPlayer::setReverbWetLevel(double wetLevel)
{
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();
    params.wetLevel = wetLevel;
    reverbAudioSource.setParameters(params);
}

/** Sets the "dry level" property of the reverb effect */
void DJAudioPlayer::setReverbDryLevel(double dryLevel)
{
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();
    params.roomSize = dryLevel;
    reverbAudioSource.setParameters(params);
}


/** Sets the "width" property of the reverb effect */
void DJAudioPlayer::setReverbWidth(double width)
{
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();
    params.width = width;
    reverbAudioSource.setParameters(params);
}

/** Set the FreezeMode (looping) property of the reverb effect */
void DJAudioPlayer::setReverbFreezeMode(double freezeLevel)
{
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();
    params.freezeMode = freezeLevel;
    reverbAudioSource.setParameters(params);
}

/** Resets reverb parameters to default values */
void DJAudioPlayer::resetReverbParamsToDefault()
{
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();
    // List of default reverb parameters
    params.roomSize = 0.5f;
    params.damping = 0.5f;
    params.wetLevel = 0.33f;
    params.dryLevel = 0.4f;
    params.width = 1.0f;
    params.freezeMode = 0.0f;
    reverbAudioSource.setParameters(params);
}


//=====================================Auto-fader Functionality===================================================

/** Sets the player's auto-fade speed by getting the Slider value from the Fader object in the DeckGUI */
void DJAudioPlayer::setFadeSpeed(double fadeSpeed)
{
    autoFadeSpeed = fadeSpeed;
}

/** Sets the isFadingIn Boolean private variable to "true" and sets the isFadingOut Boolean to false */
void DJAudioPlayer::autoFadeIn()
{
    // If player is not fading in, then set this value to true
    if (!isFadingIn)
    {
        isFadingIn = true;

        // Set isFadingOut to false when fadeIn button is pressed
        isFadingOut = false;
    }
}


/** Sets the isFadingOut Boolean private variable to "true" and sets the isFadingIn Boolean to false */
void DJAudioPlayer::autoFadeOut()
{
    // If player is not fading out, then set this value to true
    if (!isFadingOut)
    {
        isFadingOut = true;

        // Set isFadingIn to false when fadeOut button is pressed
        isFadingIn = false;
    }
}

/** Sets both isFadingIn and isFadingOut to false */
void DJAudioPlayer::stopAutoFade()
{
    isFadingIn = false;
    isFadingOut = false;
}

/** 
 *Continuously checks if the Fader Booleans (isFadingIn/isFadingOut) are true, and if one is,
 *then it either increases/decreases the volume of the audio track using the getGain() and setGain() functions
*/
void DJAudioPlayer::timerCallback()
{   
    // Gets the audio's current volume
    double currentVol = getGain();

    // If the private member variable called "isFadingIn" has been set to TRUE, then increase the volume
    if (isFadingIn)
    {
        // Only increase the volume when fading in up to 1.0
        if (currentVol < 1.0)
        {
            setGain(currentVol += autoFadeSpeed);
        }
        else
        {
            setGain(1.0);
        }
    }


    // If the private member variable called "isFadingOut" has been set to TRUE, then increase the volume
    if (isFadingOut)
    {   
        // Only decrease the volume when fading out down to 0.0
        if (currentVol > 0.0)
        {
            setGain(currentVol -= autoFadeSpeed);
        }
        else
        {
            setGain(0.0);
        }
    }
    
}


//=====================================Basic Playback======================================================
  /** Starts playing the file */
void DJAudioPlayer::play()
{
    playing = true;
    transportSource.start();
}

/** Stops playing the file */
void DJAudioPlayer::stop()
{
    playing = false;
    transportSource.stop();
}


/** Gets the relative position of the playhead */
double DJAudioPlayer::getPositionRelative()
{
    // .getCurrentPosition only returns the position back in seconds, so calculates a ratio between 0-1 through dividing by total length
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

/** Gets the length of the track in seconds to pass into the WaveformDisplay to display the position in seconds */
double DJAudioPlayer::getTrackLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}