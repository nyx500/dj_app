#include <../JuceLibraryCode/JuceHeader.h>
#include "DJAudioPlayer.h"

/** Constructor*/
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) :
    formatManager(_formatManager)
{   
    juce::Reverb::Parameters params{};
    params.roomSize = 0.5f;
    params.damping = 1.0f;
    params.wetLevel = 0.4f;
    params.width = 1.0f;
    params.freezeMode = 0.8f;
    reverbAudioSource.setParameters(params);
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
    // Delegates responsibility to transport source
    //transportSource.getNextAudioBlock(bufferToFill);

    if (readerSource.get() == nullptr || !playing)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    resampleSource.getNextAudioBlock(bufferToFill);
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
    if (gain < 0 || gain > 1)
    {
        DBG("DJAudioPlayer::setGain - Gain should be between 0 and 1!");
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