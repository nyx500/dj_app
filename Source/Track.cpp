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
    std::string _filePath
) : rowNumber(_rowNumber),
url(_url),
title(_title),
extensionName(_extensionName),
duration(_duration),
filePath(_filePath)
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
