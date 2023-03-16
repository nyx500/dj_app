/*
  ==============================================================================

    Track.h
    Created: 26 Feb 2023 10:50:24am
    Author:  Ophelia
    Purpose: stores the data for a single audio track

  ==============================================================================
  A track object which will store all the data for each added song to the playlistComponent
*/

#pragma once
#include <JuceHeader.h>

class Track {
    public:
        /** Constructor: defines private data members */
        Track(
            unsigned __int64  _rowNumber,
            juce::URL _url,
            std::string _title,
            std::string _extensionName,
            std::string _duration,
            std::string _filePath,
            // Default: all tracks are displayed before user searches for a specific term
            bool _isDisplayed = true
        );
        ~Track();

        //========================================Getters for the Private Data Members==============================================

        /** Returns track's index/row number */
        unsigned __int64 getRowNumber();
        /** Returns track's URL */
        juce::URL getUrl();
        /** Returns track's title */
        std::string getTitle();
        /** Returns track's file extension, e.g. "mp3" */
        std::string getExtensionName();
        /** Returns track duration as a HH:MM:SS string */
        std::string getDuration();
        /** Returns track's absolute file path */
        std::string getFilePath();
        /** Returns Boolean whihc stores whether the track should be displayed based on the user's searchbox input string */
        bool getIsDisplayed();

        //========================================================================================================================

        /** 
         *Determines whether the track info should be displayed (after user searches for specific terms in the search box)
         *and sets the "isDisplayed" function to false if the track does not meet the search input criteria and
         *true if it does. This takes in the search input as a parameter to check if the track meets the search criteria
         *by seeing if the track data includes this substring, and then it calls the isDisplayed setter function.
        */
        void determineIfShouldDisplay(const juce::String& searchInput);

        /* Setter function for the isDisplayed property */
        void setIsDisplayed(bool shouldTrackBeDisplayed);

    private:

        /** 
         *Reason why this is an 'unsigned __int64' type :
         * When creating the track when user selects + uploads a new file,
         * the tracks.size() property is used, which returns an unsigned __int64
         * If this is just an 'int', then the program does not compile, as an
         * error is thrown due to a "narrowing conversion" from a huge to ordinary-sized integer.
        */
        unsigned __int64  rowNumber;

        /** Private track data members storing info about this audio track */
        juce::URL url;
        std::string title;
        std::string extensionName;
        std::string duration;
        std::string filePath;

        // This stores whether the track should be displayed or not depending on whether user has searched for it (default --> true)
        bool isDisplayed;
};