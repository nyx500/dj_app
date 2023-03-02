/*
  ==============================================================================

    CSVHelper.h
    Created: 2 Mar 2023 7:52:15pm
    Author:  Ophelia

  ==============================================================================
*/

#pragma once
#include "Track.h"
#include <JuceHeader.h>
#include <string>
#include <vector>

class CSVHelper {
   
// Constructor: takes the path to the CSV as argument
public: 
    CSVHelper();


    /** Creates the CSV file to store track data if doesn't already exist,
     * and stores the file path in the private filePath property 
    */
    void getAndStoreCSVFile();

    /** 
        * Writes in the data for the tracks into the CSV File 
        * Pass in the track vector by reference for efficiency
    */
    void writeTracksDataIntoCSVFile(std::vector<Track>& tracks);

    /** 
    * Reads rows (1 track --> 1 row) from CSV file and outputs a 
    * vector of tracks (purpose: to store in the PlaylistComponent)
    */
    std::vector<Track> readTracksDataFromCSVFile();

private:

    /**
      * Converts a track into a comma-separated format as a juce::String data type
    */
    juce::String trackToCSV(Track& track);

    /**
        * Converts a CSV line into a track object
    */
    Track CSVToTrack(juce::StringRef& csvLine);

    // Stores the absolute path to the CSV File
    juce::String filePath;

};
