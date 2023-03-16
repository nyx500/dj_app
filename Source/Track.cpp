/*
  ==============================================================================

    Track.cpp
    Created: 26 Feb 2023 10:50:24am
    Author:  Ophelia

  ==============================================================================
*/

#include "Track.h"
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

// Getters for the data members
unsigned __int64 Track::getRowNumber()
{
    return rowNumber;
}
juce::URL Track::getUrl()
{
    return url;
}
std::string Track::getTitle()
{
    return title;
}
std::string Track::getExtensionName()
{
    return extensionName;
}
std::string Track::getDuration()
{
    return duration;
}
std::string Track::getFilePath()
{
    return filePath;
}

bool Track::getIsDisplayed()
{
    return isDisplayed;
}

/** This function decides whether the track info should be displayed(after user searches for tracks)
    and sets the "isDisplayed" function to false if the track does not meet the search input criteria and
    true if it does. Takes in the search input as a parameter to check if the track meets the search criteria
    by seeing if the track data (title, extension) includes this substring. Calls the isDisplayed setter.
*/
void Track::determineIfShouldDisplay(const juce::String& searchInput)
{   
    // Make determination of whether track-info contains the entered string case-insensitive
    juce::String lowerCaseSearchInput = searchInput.toLowerCase();
    // Change the input string to a stringRef JUCE data type because the .contains() method takes a StringRef argument
    juce::StringRef stringRefSearchInput = juce::StringRef(lowerCaseSearchInput);

    // Logic to check whether the track's title/extension match the user's search input
    if (juce::String(title).toLowerCase().contains(stringRefSearchInput)
        || juce::String(duration).toLowerCase().contains(stringRefSearchInput)
    )
    {   

        // If the track data contains the entered substring, set isDisplayed bool to "true"
        DBG("Track::determineIfShouldDisplay - true");
        setIsDisplayed(true);
    }
    // Search box is empty --> display all the tracks
    else if (searchInput.isEmpty())
    {   
        // If the track data contains the entered substring, set isDisplayed bool to "true"
        DBG("Track::determineIfShouldDisplay - true");
        setIsDisplayed(true);
    }
    else
    {   

        // If the track data does not contain the entered substring, set isDisplayed bool to "false"
        DBG("Track::determineIfShouldDisplay - false");
        setIsDisplayed(false);
    }
}


/* Setter function for the isDisplayed property */
void Track::setIsDisplayed(bool shouldTrackBeDisplayed)
{
    isDisplayed = shouldTrackBeDisplayed;
}