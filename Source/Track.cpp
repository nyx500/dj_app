/*
  ==============================================================================

    Track.cpp
    Created: 26 Feb 2023 10:50:24am
    Author:  Ophelia
    Purpose: stores the data for a single audio track

  ==============================================================================
*/

#include "Track.h"


/** Constructor: defines private data members */
Track::Track(
    unsigned __int64 _rowNumber,
    juce::URL _url,
    std::string _title,
    std::string _extensionName,
    std::string _duration,
    std::string _filePath,
    bool _isDisplayed
) : rowNumber(_rowNumber),
url(_url),
title(_title),
extensionName(_extensionName),
duration(_duration),
filePath(_filePath),
isDisplayed(_isDisplayed)
{
}

Track::~Track() {

}

//========================================Getters for the Private Data Members==============================================

/** Returns track's index/row number */
unsigned __int64 Track::getRowNumber()
{
    return rowNumber;
}
/** Returns track's URL */
juce::URL Track::getUrl()
{
    return url;
}
/** Returns track's title */
std::string Track::getTitle()
{
    return title;
}
/** Returns track's file extension, e.g. "mp3" */
std::string Track::getExtensionName()
{
    return extensionName;
}
/** Returns track duration as a HH:MM:SS string */
std::string Track::getDuration()
{
    return duration;
}
/** Returns track's absolute file path */
std::string Track::getFilePath()
{
    return filePath;
}
/** Returns Boolean whihc stores whether the track should be displayed based on the user's searchbox input string */
bool Track::getIsDisplayed()
{
    return isDisplayed;
}

//========================================================================================================================

/**
 *Determines whether the track info should be displayed (after user searches for specific terms in the search box)
 *and sets the "isDisplayed" function to false if the track does not meet the search input criteria and
 *true if it does. This takes in the search input as a parameter to check if the track meets the search criteria
 *by seeing if the track data includes this substring, and then it calls the isDisplayed setter function.
*/
void Track::determineIfShouldDisplay(const juce::String& searchInput)
{   
    // Makes determination of whether track-info contains the entered string case-insensitive
    juce::String lowerCaseSearchInput = searchInput.toLowerCase();

    // Changes the input string to a stringRef JUCE data type because the .contains() method takes a StringRef argument
    juce::StringRef stringRefSearchInput = juce::StringRef(lowerCaseSearchInput);

    // Logic to check whether the track's title/duration match the user's search input
    if (juce::String(title).toLowerCase().contains(stringRefSearchInput)
        || juce::String(duration).toLowerCase().contains(stringRefSearchInput)
    )
    {   
        // If the track data contains the entered substring, set isDisplayed bool to "true"
        setIsDisplayed(true);
    }
    // Search box is empty --> display all the tracks
    else if (searchInput.isEmpty())
    {   
        setIsDisplayed(true);
    }
    else
    {   
        // If the track data does not contain the entered substring, set isDisplayed bool to "false"
        setIsDisplayed(false);
    }
}


/* Setter function for the isDisplayed property */
void Track::setIsDisplayed(bool shouldTrackBeDisplayed)
{
    isDisplayed = shouldTrackBeDisplayed;
}