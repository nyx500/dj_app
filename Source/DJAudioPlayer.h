#pragma once
#include <../JuceLibraryCode/JuceHeader.h>// Need this header to use Juce library

// Want this to be an audio source, so inherit from the audio source (has virtual functions to implement)
class DJAudioPlayer : public juce::AudioSource,
    // Timer's purpose here: for the "AutoFade" functionality --> checks Boolean variables to see if track should be autofading in/out
    public juce::Timer
{

public:
    /** Constructor*/
    DJAudioPlayer(juce::AudioFormatManager& _formatManager); // Now takes in the program-scope formatManager used in all classes
    /** /Destructor */
    ~DJAudioPlayer();

    //====================Audio Source Virtual Functions Implementation=============
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    //==============================================================================

    // Basic sound altering functions
    void loadURL(juce::URL audioURL);
    void setGain(double gain);

    /** For the "fade" effect: returns the current volume of the audio*/
    double getGain();

    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    // Relative means between '0 and 1' instead of between 0 and "number of seconds"
    void setPositionRelative(double pos);
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


    /************************SECTION FOR AUTO-FADER FUNCTIONALITY***************************/
    /** Sets the player's auto-fade speed by getting the slider value from the Fader object */
    void setFadeSpeed(double fadeSpeed);
    /** Automatically increases ("fades-in") the volume of the audio track at a speed dep. on value of autoFadeSpeed */
    void autoFadeIn();
    /** Automatically decreases ("fades-out") the volume of the audio track at a speed dep. on value of autoFadeSpeed */
    void autoFadeOut();
    /** Stops any auto-fading that might be going on*/
    void stopAutoFade();
    /** Implements juce::Timer's inherited pure virtual function to auto-fade consistently if fade Booleans are on
     * Checks if booleans (isFadingIn/isFadingOut) are true, and if one is, then it either increases/decreases the volume
     * of the audio track using the getGain() and setGain() functions
    */
    void timerCallback() override;

    // Basic stopping and starting functions
    /** Start playing the file */
    void play();
    /** Stop playing the file */
    void stop();

    /** Gets the relative position of the playhead*/
    double getPositionRelative();

    /** Get the length of the track in seconds to pass into the WaveformDisplay to display position in seconds */
    double getTrackLengthInSeconds();

private:

    // Need a global AudioFormatManager instead (Week 18) --> type should be reference
    juce::AudioFormatManager& formatManager;

    std::unique_ptr <juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    /* An AudioSource that uses the Reverb class to apply a reverb to another AudioSource. */
    juce::ReverbAudioSource reverbAudioSource{ &resampleSource, false};
    bool playing = false;

    /********************************FADER FUNCTIONALITY*************************************/
    /* Stores the speed to auto fade-in/fade-out with the Fader object */
    double autoFadeSpeed;
    // Default values of bools: neither fading in or fading out, both have to be false on startup
    double isFadingIn{ false };
    double isFadingOut{ false };
};