#include <../JuceLibraryCode/JuceHeader.h>
#include "DJAudioPlayer.h"

/** Constructor*/
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) :
    formatManager(_formatManager)
{   
    juce::Reverb::Parameters params{};

    // List of default reverb parameters
    params.roomSize = 0.5f;
    params.damping = 0.5f;
    params.wetLevel = 0.33f;
    params.dryLevel = 0.4f;
    params.width = 1.0f;
    params.freezeMode = 0.0f;
    reverbAudioSource.setParameters(params);

    // Argument: how often to call the timerCallback function
    startTimer(10); //'10' = 10 milliseconds (hundredth of a a second)
}
/** /Destructor */
DJAudioPlayer::~DJAudioPlayer()
{

}

//====================Audio Source Virtual Functions Implementation=============
// Birth
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);  
    reverbAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

// Life
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{

    if (readerSource.get() == nullptr || !playing)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    //resampleSource.getNextAudioBlock(bufferToFill);
    // Delegate responsibility to the reverbAudioSource which takes in the resampleSource as a parameter when created
    reverbAudioSource.getNextAudioBlock(bufferToFill);
}

// Death
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbAudioSource.releaseResources();
}
//==============================================================================

// Basic sound altering functions
void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain)
{
    // Make sure volume is between the bounds of 0.0 and 1.0
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

/** For the "fade" effect: returns the current volume of the audio*/
double DJAudioPlayer::getGain()
{
   return transportSource.getGain();
}


void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        DBG("DJAudioPlayer::setSpeed - Ratio should be between 0 and 100!");
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}


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


// Set the roomSize parameter for the reverb effect
void DJAudioPlayer::setReverbRoomSize(double roomSize)
{   
    DBG("calling DJAudioPlayer::setReverbRoomSize");
    DBG(std::to_string(roomSize));
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();

    params.roomSize = roomSize;
    reverbAudioSource.setParameters(params);
}

/** Sets the damping property of the reverb effect */
void DJAudioPlayer::setReverbDamping(double dampingLevel)
{
    DBG("calling DJAudioPlayer::setReverbDamping");
    DBG(std::to_string(dampingLevel));
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();

    params.damping = dampingLevel;
    reverbAudioSource.setParameters(params);
}
/** Sets the "wet level" property of the reverb effect */
void DJAudioPlayer::setReverbWetLevel(double wetLevel)
{
    DBG("calling DJAudioPlayer::setReverbWetLevel");
    DBG(std::to_string(wetLevel));
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();

    params.wetLevel = wetLevel;
    reverbAudioSource.setParameters(params);
}
/** Sets the "dry level" property of the reverb effect */
void DJAudioPlayer::setReverbDryLevel(double dryLevel)
{
    DBG("calling DJAudioPlayer::setReverbDryLevel");
    DBG(std::to_string(dryLevel));
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();

    params.roomSize = dryLevel;
    reverbAudioSource.setParameters(params);
}
/** Sets the "width" property of the reverb effect */
void DJAudioPlayer::setReverbWidth(double width)
{
    DBG("calling DJAudioPlayer::setReverbWidth");
    DBG(std::to_string(width));
    juce::Reverb::Parameters params = reverbAudioSource.getParameters();

    params.width = width;
    reverbAudioSource.setParameters(params);
}
/** Set the FreezeMode (looping) property of the reverb effect */
void DJAudioPlayer::setReverbFreezeMode(double freezeLevel)
{
    DBG("calling DJAudioPlayer::setReverbFreezeMode");
    DBG(std::to_string(freezeLevel));
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

/*****************************AUTO-FADE FUNCTIONALITY****************************************/
/** Sets the player's auto-fade speed by getting the slider value from the Fader object */
void DJAudioPlayer::setFadeSpeed(double fadeSpeed)
{
    autoFadeSpeed = fadeSpeed;
}
/** Automatically increases ("fades-in") the volume of the audio track at a speed dep. on value of autoFadeSpeed */
void DJAudioPlayer::autoFadeIn()
{   
    // If player is not fading in, set this value to true
    if (!isFadingIn)
    {
        isFadingIn = true;
        // Set isFadingOut to false when fadingIn is pressed
        isFadingOut = false;
    }
}
/** Automatically decreases ("fades-out") the volume of the audio track at a speed dep. on value of autoFadeSpeed */
void DJAudioPlayer::autoFadeOut()
{
    // If player is not fading out, set this value to true
    if (!isFadingOut)
    {
        isFadingOut = true;
        // Set isFadingIn to false when fadingOut is pressed
        isFadingIn = false;
    }
}
/** Stops any auto-fading that might be going on*/
void DJAudioPlayer::stopAutoFade()
{   
    isFadingIn = false;
    isFadingOut = false;
}


/** Implements juce::Timer's inherited pure virtual function to auto-fade consistently if fade Booleans are on
     * Checks if booleans (isFadingIn/isFadingOut) are true, and if one is, then it either increases/decreases the volume
     * of the audio track using the getGain() and setGain() functions
*/
void DJAudioPlayer::timerCallback()
{
    if (isFadingIn)
    {   
        DBG("DJAudioPlayer::timerCallback - isFadingIn");
        double currentVolume = getGain();
        DBG(std::to_string(currentVolume));
        if (currentVolume < 1.0)
        {
            setGain(currentVolume += autoFadeSpeed);
        }
        else
        {
            setGain(1.0);
        }
    }

    if (isFadingOut)
    {

        DBG("DJAudioPlayer::timerCallback - isFadingOut");
        double currentVolume = getGain();
        DBG(std::to_string(currentVolume));
        if (currentVolume > 0.0)
        {
            setGain(currentVolume -= autoFadeSpeed);
        }
        else
        {
            setGain(0.0);
        }
    }
}
/*****************************END OF AUTO-FADE FUNCTIONALITY****************************************/

// Basic sound stopping and starting functions
void DJAudioPlayer::play()
{
    playing = true;
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    playing = false;
    transportSource.stop();
}


double DJAudioPlayer::getPositionRelative()
{
    // .getCurrentPosition only returns the position back in seconds
    // BUT we need a relative (0 to 1) position...
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}


/** Get the length of the track in seconds to pass into the WaveformDisplay to display position in seconds */
double DJAudioPlayer::getTrackLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}