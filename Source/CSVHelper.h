/*
  =======================================================================================

    CSVHelper.h
    Created: 2 Mar 2023 7:52:15pm
    Author:  Ophelia
    Purpose: A set of helper methods to help read and write "Track" data from a CSV file
  =======================================================================================
*/

#pragma once
#include "Track.h"
#include <JuceHeader.h>
#include <string>
#include <vector>

class CSVHelper
{
public:
    /** Either opens or creates a CSV trackData file when CSVHelper is instantiated */
    CSVHelper();

    /** 
     *Creates the CSV file to store the track data if doesn't already exist.
     *Stores the file path to the track data in this class's private 
     "filePath" property
    */
    void getAndStoreCSVFile();

    /**
     * Writes the track data into the CSV File.
    */
    void writeTracksDataIntoCSVFile(std::vector<Track>& tracks);

    /**
     * Reads rows (1 track per row) from the trackdata CSV file and
     * outputs a vector of tracks to store in the Playlist
     */
    std::vector<Track> readTracksDataFromCSVFile();

private:
    /**
     * Converts a track into a comma-separated juce::String
    */
    juce::String trackToCSV(Track& track);

    /**
     * Converts a CSV line/row into a "Track" object to store in the
     * "Tracks" vector in the Playlist
    */
    Track CSVToTrack(juce::StringRef& csvLine);

    // Stores the absolute path to the trackData CSV File
    juce::String filePath;
};