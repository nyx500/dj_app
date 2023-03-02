/*
  ==============================================================================

    CSVHelper.cpp
    Created: 2 Mar 2023 7:52:15pm
    Author:  Ophelia

  ==============================================================================
*/

#include "CSVHelper.h"

CSVHelper::CSVHelper()
{   
    // Gets/creates CSV track data file right away when CSVHelper instance is instantiated in another class
    getAndStoreCSVFile();
}

// Creates a the CSV file to store track data if doesn't already exist
void CSVHelper::getAndStoreCSVFile()
{
    // Sets the Current Working Directory to be the current (="SOURCE") folder
    juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" }).setAsCurrentWorkingDirectory();

    // This line either creates a new CSV file called "trackData.csv" if one does not exist, or gets the file if it does exist
    juce::File tracksFile = juce::File::getCurrentWorkingDirectory().getChildFile("trackData.csv");

    // Checks if the CSV File already exists
    if (!tracksFile.existsAsFile())
    {   
        // Create a new trackData.csv file in the current directory
        tracksFile.create();
    }

    // Stores the absolute filepath to the CSV file
    filePath = tracksFile.getFullPathName();
}



/** Writes in the data for the tracks into the CSV File
    * Pass in the track vector by reference for efficiency
*/
void CSVHelper::writeTracksDataIntoCSVFile(std::vector<Track>& tracks)
{   
    // Get the file object from the stored path to file
    juce::File targetFile = juce::File(filePath);

    // Create an output stream to write into the file
    juce::FileOutputStream stream(targetFile);
    
    // Write tracks into file only if stream has opened
    if (stream.openedOk())
    {   
        // Overwrites the previous data in the file with the help of these juce::FileOutputStream functions
        stream.setPosition(0);
        stream.truncate();

        // Iterates over the tracks vector and store data for each track in the file
        for (Track& track : tracks)
        {   
            // Uses trackToCSV helper method to output track as a comma-separated string, and writes it to the file
            stream.writeString(trackToCSV(track));
        }
    }
    else
    {
        DBG("CSVHelper::writeTracksDataIntoCSVFile - could not open file write stream!");
    }
}



/**
* Reads rows (1 track --> 1 row) from CSV file and outputs a
* vector of tracks (purpose: to store in the PlaylistComponent)
*/
std::vector<Track> CSVHelper::readTracksDataFromCSVFile()
{   
    // Create an empty tracks vector
    std::vector<Track> tracks;

    // Create file from stored path to CSV file
    juce::File targetFile = juce::File(filePath);

    // Verify the file exists
    if (targetFile.existsAsFile())
    {
        // Create a juce::FileInputStream object to read from the file
        juce::FileInputStream stream(targetFile);

        while (!stream.isExhausted())
        {   
            // Validate if the track can be parsed from the CSV line, if not catch
            // the exception generate by calling CSVToTrack and output an error message in the DBG console
            try {
                Track track = CSVToTrack(juce::StringRef(stream.readString()));

                // Add the track to the tracks vector to return
                tracks.push_back(track);
            }
            catch (std::exception& e)
            {
                DBG("CSVHelper::readTracksDataFromCSVFile - one of the CSV rows could not be read!");
            }
        }
    }

    return tracks;
}


/**
  * Converts a track into a comma-separated format as a juce::String data type
*/
juce::String CSVHelper::trackToCSV(Track& track)
{
    // Constructs a comma-separated string out of the different track properties
    // Converts the track's juce::URL property into a std::string data type
    std::string trackDataAsString = track.getUrl().toString(false).toStdString();
    // Add the track title, duration, filePath
    trackDataAsString = trackDataAsString + "," + track.getTitle() + "," + track.getDuration() + "," + track.getFilePath();

    // Converts the line to a juce::String in order to use the juce::WriteString/juce::readString methods to read from a file
    juce::String trackDataAsJuceString = juce::String(trackDataAsString);

    // Adds a newline to the end of the juce::String to separate the tracks (=rows in the CSV file)
    trackDataAsJuceString = trackDataAsJuceString.paddedRight('\n', trackDataAsJuceString.length() + 1);

    return trackDataAsJuceString;

}



/**
    * Converts a CSV line into a track object
*/
Track CSVHelper::CSVToTrack(juce::StringRef& csvLine)
{
    // Create a juce::StringArray object which comes with an "addTokens" feature
     // Each StringArray stores one track broken into comma-separated tokens
    juce::StringArray trackAsStrings;

    // Adds the tokens the track is made of to the trackAsStrings array
    trackAsStrings.addTokens(csvLine, juce::StringRef(","), juce::StringRef(","));

    // Throw an exception if the line cannot be converted into four tokens
    if (trackAsStrings.size() != 4)
    {   
        DBG("CSVHelper::CSVToTrack - bad CSV row!");
        throw std::exception();
    }
    else
    {
        // Reminder of order of the tokens from the CSV row: fileUrl (as string), title, duration, filepath
        // Turn the JUCE::Strings into std::strings!
        Track track{
            juce::URL(juce::String(trackAsStrings[0])),
            juce::String(trackAsStrings[1]).toStdString(),
            juce::String(trackAsStrings[2]).toStdString(),
            juce::String(trackAsStrings[3]).toStdString()
        };

        return track;
    }
}