/*
  ==============================================================================

    Track.h
    Created: 26 Feb 2023 10:50:24am
    Author:  Ophelia

  ==============================================================================
  A track object which will store all the data for each added song to the playlistComponent
*/

#pragma once
#include <JuceHeader.h>

class Track {
    public:
        Track(
            unsigned __int64  _rowNumber,
            juce::URL _url,
            std::string _title,
            std::string _extensionName,
            std::string _duration,
            std::string _filePath,
            // Default: all tracks are displayed before user searches for something
            bool _isDisplayed = true
        );
        ~Track();

        // Getters for the data members
        unsigned __int64 getRowNumber();
        juce::URL getUrl();
        std::string getTitle();
        std::string getExtensionName();
        std::string getDuration();
        std::string getFilePath();
        bool getIsDisplayed();

        /** This function decides whether the track info should be displayed(after user searches for tracks)
            and sets the "isDisplayed" function to false if the track does not meet the search input criteria and
            true if it does. Takes in the search input as a parameter to check if the track meets the search criteria
            by seeing if the track data includes this substring. Calls the isDisplayed setter.
        */
        void determineIfShouldDisplay(const juce::String& searchInput);

        /* Setter function for the isDisplayed property */
        void setIsDisplayed(bool shouldTrackBeDisplayed);

    private:

        /** Reason why this is an 'unsigned __int64' type :
         * when creating the track when user selects + uploads a new file,
         * the tracks.size() property is used, which returns an unsigned __int64
         * If this is just an 'int', then the program does not compile, as an
         * error is thrown due to a "narrowing conversion" from a huge to ordinary-sized integer
        */
        unsigned __int64  rowNumber;

        juce::URL url;
        std::string title;
        std::string extensionName;
        std::string duration;
        std::string filePath;

        // This stores whether the track should be displayed or not depending on whether user has searched for it
        // The default is "true" --> before the user searches, all the tracks should be displayed
        bool isDisplayed;
};