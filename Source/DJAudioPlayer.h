#pragma once
#include <../JuceLibraryCode/JuceHeader.h>// Need this header to use Juce library

// Want this to be an audio source, so inherit from the audio source (has virtual functions to implement)
class DJAudioPlayer : public juce::AudioSource {

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
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    // Relative means between '0 and 1' instead of between 0 and "number of seconds"
    void setPositionRelative(double pos);

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
    bool playing = false;
};