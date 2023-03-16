#pragma once
#include <../JuceLibraryCode/JuceHeader.h>

// This is an audio source: it inherits from the JUCE AudioSource clas, so it has virtual functions to implement
class DJAudioPlayer : public juce::AudioSource,
    // Timer: for the "AutoFade" functionality --> checks Boolean variables to see if track should be autofading in or out
    public juce::Timer
{

public:

    /** Constructor: takes in the program-scope formatManager from Main Component*/
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);

    /** /Destructor */
    ~DJAudioPlayer();

    //====================Audio Source Virtual Functions Implementation=============
    /** Tells the audio source to prepare for playing */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    /** Called repeatedly to fetch subsequent blocks of audio data */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    /** Allows the audio source to release anything it no longer needs after playback has stopped. */
    void releaseResources() override;
    //==============================================================================

    /** Loads in the selected audio file's URL to play the selected audio track and loads up an audio stream from the URL to the transportSource */
    void loadURL(juce::URL audioURL);

    //================================Main Slider Controls==============================================
    /** Sets the volume of the audio to the double passed in as a parameter*/
    void setGain(double gain);

    /** Returns the current volume of the audio track */
    double getGain();

    /** Sets the speed of the audio to the double passed in as a parameter */
    void setSpeed(double ratio);

    /** Sets the position inside the audio track to the double passed in as a parameter */
    void setPosition(double posInSecs);

    /** 
     * Sets relative position, meaning a position between '0 and 1', instead of between 0 and "number of seconds".
     * This is because each track has a different length, so to keep slider the same for all the tracks, make position 0-1.s
    */
    void setPositionRelative(double pos);


    //====================================Reverb Effects applied to ReverbAudioSource instance=============================
    /** Sets the roomSize property of the reverb effect */
    void setReverbRoomSize(double roomSize);
    /** Sets the damping property of the reverb effect */
    void setReverbDamping(double dampingLevel);
    /** Sets the "wet level" property of the reverb effect */
    void setReverbWetLevel(double wetLevel);
    /** Sets the "dry level" property of the reverb effect */
    void setReverbDryLevel(double dryLevel);
    /** Sets the "width" property of the reverb effect */
    void setReverbWidth(double width);
    /** Set the FreezeMode (looping) property of the reverb effect */
    void setReverbFreezeMode(double freezeLevel);
    /** Resets reverb parameters to default values */
    void resetReverbParamsToDefault();

    //=====================================Auto-fader Functionality======================================================
    /** Sets the player's auto-fade speed by getting the Slider value from the Fader object in the DeckGUI */
    void setFadeSpeed(double fadeSpeed);
    /** Sets the isFadingIn Boolean private variable to "true" and sets the isFadingOut Boolean to false */
    void autoFadeIn();
    /** Sets the isFadingOut Boolean private variable to "true" and sets the isFadingIn Boolean to false */
    void autoFadeOut();
    /** Sets both isFadingIn and isFadingOut to false */
    void stopAutoFade();
    /**
     * Continuously checks if the Fader Booleans (isFadingIn/isFadingOut) are true, and if one is, 
     * then it either increases/decreases the volume of the audio track using the getGain() and setGain() functions
    */
    void timerCallback() override;


    //=====================================Basic Playback======================================================
    /** Starts playing the file */
    void play();
    /** Stops playing the file */
    void stop();

    /** Gets the relative position of the playhead*/
    double getPositionRelative();

    /** Gets the length of the track in seconds to pass into the WaveformDisplay to display position in seconds */
    double getTrackLengthInSeconds();

private:

    // The reference to the audio format manager passed in from MainComponent 
    juce::AudioFormatManager& formatManager;

    // Reader for the data stream from the audio file being played
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    
    // The three transport sources to play (transportSource) and manipulate (resample, reverb) the audio
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    /* This is a type of AudioSource used to apply a reverb effect to another AudioSource. */
    juce::ReverbAudioSource reverbAudioSource{ &resampleSource, false };

    // Stores if the audio is being played
    bool playing = false;


    //======================================Fader Data===================================================================
    // Variables storing the speed of the fade, as well as Booleans keeping track of whether to fade the track at 
    // the present moment

    /* This stores the speed to auto fade-in/fade-out with the slider in the Fader object */
    double autoFadeSpeed;

    // Default values of Booleans: as neither fading in or fading out on startup, "false" is the starting value
    double isFadingIn{ false };
    double isFadingOut{ false };
};