/*
  ==============================================================================

    Track.cpp
    Created: 26 Feb 2023 10:50:24am
    Author:  Ophelia

  ==============================================================================
*/

#include "Track.h"
Track::Track(
    juce::URL _url,
    std::string _title,
    std::string _duration,
    std::string _filePath
) : url(_url),
title(_title),
duration(_duration),
filePath(_filePath)
{
}

Track::~Track() {

}

// Getters for the data members
juce::URL Track::getUrl()
{
    return url;
}
std::string Track::getTitle()
{
    return title;
}
std::string Track::getDuration()
{
    return duration;
}
std::string Track::getFilePath()
{
    return filePath;
}
