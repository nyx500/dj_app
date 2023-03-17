/*
  =======================================================================================

    CSVHelper.h
    Created: 2 Mar 2023 7:52:15pm
    Author:  Ophelia
    Purpose: A set of helper methods to help read and write "Track" data from a CSV file
  =======================================================================================
*/

#include "CSVHelper.h"


/** Either opens or creates a CSV trackData file when CSVHelper is instantiated */
CSVHelper::CSVHelper()
{
    getAndStoreCSVFile();
}

/**
 *Creates the CSV file to store the track data if doesn't already exist.
 *Stores the file path to the track data in this class's private
 *"filePath" property
*/
void CSVHelper::getAndStoreCSVFile()
{
    // Sets the Current Working Directory to be the current (="SOURCE") folder
    juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" }).setAsCurrentWorkingDirectory();

    // This either creates a new CSV file called "trackData.csv" if one does not already exist, or it gets the file if it does exist
    juce::File tracksFile = juce::File::getCurrentWorkingDirectory().getChildFile("trackData.csv");

    // Checks if the CSV File exists
    if (!tracksFile.existsAsFile())
    {
        // Create a new trackData.csv file in the current directory if it does not exist yet
        tracksFile.create();
    }

    // Stores the absolute filepath to the CSV file in the "filePath" private variable
    filePath = tracksFile.getFullPathName();
}

/**
 * Writes the track data into the CSV File.
*/
void CSVHelper::writeTracksDataIntoCSVFile(std::vector<Track>& tracks)
{
    // Gets the file object from the stored path to file
    juce::File targetFile = juce::File(filePath);

    // Creates an output stream to write into the file
    juce::FileOutputStream stream(targetFile);

    // Writes tracks into file only if stream successfully opens
    if (stream.openedOk())
    {
        // Overwrites the previous data which was stored in the file with data from the the inputted Tracks vector
        stream.setPosition(0);
        stream.truncate();

        // Iterates over the Tracks vector and writes the data for each Track in the vector into the csv
        for (Track& track : tracks)
        {
            // Calls trackToCSV method to output Track information as a comma-separated string, and writes this to the file
            stream.writeString(trackToCSV(track));
        }
    }
    // Print DBG error message if stream could not be opened
    else
    {
        DBG("CSVHelper::writeTracksDataIntoCSVFile - could not open file write stream!");
    }
}

/**
 * Reads rows (1 track per row) from the trackdata CSV file and
 * outputs a vector of Tracks to store in the Playlist
*/
std::vector<Track> CSVHelper::readTracksDataFromCSVFile()
{
    // Creates an empty tracks vector
    std::vector<Track> tracks;

    // Creates a JUCE File object from the stored path to CSV file
    juce::File targetFile = juce::File(filePath);

    // Verifies the file exists
    if (targetFile.existsAsFile())
    {
        // Create a juce::FileInputStream object to read from the file
        juce::FileInputStream stream(targetFile);

        while (!stream.isExhausted())
        {
            // Try-catch block for CSVToTrack method (throws up an error if the CSV row cannot be parsed)
            try
            {
                // This called-function throws an error if the tokens cannot be parsed from the CSV
                // E.g. if the rowIndex token cannot be converted from a string to an integer
                Track track = CSVToTrack(juce::StringRef(stream.readString()));

                // Adds the track to the tracks vector
                tracks.push_back(track);
            }
            // Prints out the message below with DBG if CSVToTrack threw an error
            catch (std::exception& e)
            {
                DBG("CSVHelper::readTracksDataFromCSVFile - one of the CSV rows could not be read!");
            }
        }
    }

    return tracks;
}

/**
  * Converts a track into a comma-separated juce::String
 */
juce::String CSVHelper::trackToCSV(Track& track)
{
    // Converts the track's juce::URL property into a std::string
    std::string trackUrlAsString = track.getUrl().toString(false).toStdString();

    // Creates a string storing the track's URL, title, extension, duration, filePath
    std::string trackDataAsString = std::to_string(track.getRowNumber()) + "," + trackUrlAsString + "," + track.getTitle() +
        "," + track.getExtensionName() + "," + track.getDuration() + "," + track.getFilePath();

    // Converts the above line to a juce::String in order to use the juce::WriteString/juce::readString methods for file management
    juce::String trackDataAsJuceString = juce::String(trackDataAsString);

    // Add a newline to the end of the juce::String to separate the Tracks from each other (=rows in the CSV file)
    trackDataAsJuceString = trackDataAsJuceString.paddedRight('\n', trackDataAsJuceString.length() + 1);

    // Return the comma-separated string ready to store in the CSV file
    return trackDataAsJuceString;
}

/**
 * Converts a CSV line/row into a "Track" object to store in the
 * "Tracks" vector in the Playlist
 */
Track CSVHelper::CSVToTrack(juce::StringRef& csvLine)
{
    // Creates a juce::StringArray object which comes with an "addTokens" feature
    juce::StringArray trackAsStrings;

    // Breaks the CSV string/row into the tokens that make up the data for one track
    trackAsStrings.addTokens(csvLine, juce::StringRef(","), juce::StringRef(","));

    // Throws an exception if the row/line cannot be converted into precisely six tokens
    if (trackAsStrings.size() != 6)
    {
        DBG("CSVHelper::CSVToTrack - bad CSV row! Does not have six tokens!");
        throw std::exception();
    }
    // Token size is good: convert the CSV line to a Track and return it
    // Order of data items from the CSV row: row index, fileUrl (as string), title, file extension, duration, filepath
    else
    {
        unsigned __int64 rowIndex;

        // Throws the error up to the caller (readTracksDataFromCSVFile()) if the row-index token cannot be converted to a string
        try
        {
            rowIndex = std::stoi(trackAsStrings[0].toStdString());
        }
        catch (const std::exception& e)
        {
            throw;
        }

        // Create a Track if an error was not thrown when converting the row index token from string to int
        Track track{
            rowIndex,
            juce::URL(juce::String(trackAsStrings[1])),
            juce::String(trackAsStrings[2]).toStdString(),
            juce::String(trackAsStrings[3]).toStdString(),
            juce::String(trackAsStrings[4]).toStdString(),
            juce::String(trackAsStrings[5]).toStdString() };

        return track;
    }
}