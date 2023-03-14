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
            std::string _filePath
        );
        ~Track();

        // Getters for the data members
        unsigned __int64 getRowNumber();
        juce::URL getUrl();
        std::string getTitle();
        std::string getExtensionName();
        std::string getDuration();
        std::string getFilePath();

    private:
        unsigned __int64  rowNumber;
        juce::URL url;
        std::string title;
        std::string extensionName;
        std::string duration;
        std::string filePath;
};