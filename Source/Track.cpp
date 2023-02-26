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
    std::string _artist,
    std::string _duration
) : url(_url),
title(_title),
artist(_artist),
duration(_duration)
{
}

Track::~Track() {

}