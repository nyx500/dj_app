#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _gui1, DeckGUI* _gui2) :
    gui1(_gui1),
    gui2(_gui2)
{

    // Gets the URL a file storing the URLs of the songs (creates a file if one does not exist)
    // Stores the URL in private data member "storageFileURL"
    fullPathToFile = CreateOrLoadTrackURLsFile();

    readFile(fullPathToFile);

    //parseExistingXMLFile(fullPathToFile);

    // getHeader() returns another kind of component called a Header (look up implementation!)
    // addColumn() function is really gnarly --> but many args are default apart from columnName, columnId, width (first 3 args)
    // ColumnId cannot be 0 in Juce 6
    tableComponent.getHeader().addColumn("Track Title", 1, 300);
    // Add Column for track duration
    tableComponent.getHeader().addColumn("Track Duration", 2, 200);
    // Play in DeckGUI1 button column
    tableComponent.getHeader().addColumn("", 3, 100);
    // Play in DeckGUI2 button column
    tableComponent.getHeader().addColumn("", 4, 100);
    
    // Add the data (model) to the tableComponent
    // This class inherits from .: IS the model, so use "this"
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addButton);
    addButton.addListener(this);

    // I inserted a new formatManager to this class to be able to get
    // the duration of the track in seconds and store it inside the
    // track object
    formatManager.registerBasicFormats();
}

// Destructor: release memory and reset pointers
PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::darkgreen);
    g.fillRect(getLocalBounds());   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    // Put the add bottom above the playlist table
    addButton.setBounds(getWidth() * 0.25, 0, getWidth() * 0.5, getHeight() * 0.2);
    tableComponent.setBounds(0, getHeight() * 0.2, getWidth(), getHeight());

}

int PlaylistComponent::getNumRows()
{
    // just return the number of tracks in the vector
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    // Graphics 'g' --> just the row we are able to draw in
    // "selected" is when user clicks
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    // get the graphics object (the cell)
    // Documentation: 
    // void 	drawText (const String &text, int x, int y, int width, int height, Justification justificationType, bool useEllipsesIfTooBig=true) const
    // Draws a line of text within a specified rectangle.

    if (columnId == 1)
    {
        g.drawText(tracks[rowNumber].getTitle(),
            2, 0, width - 4, height,
            juce::Justification::centredLeft,
             true);
    }

    if (columnId == 2)
    {
        g.drawText(tracks[rowNumber].getDuration(),
            2, 0, width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
}

// Virtual (not pure virtual) method to implement button in a cell
juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    // If the columnId is that for the play button (equals 2)
    if (columnId == 3)
    {
        // If pointer to existingComponent is a nullptr (no component has been created yet)
        if (existingComponentToUpdate == nullptr)
        {
            // Create the component
            juce::TextButton* btn = new juce::TextButton{ "Load into Deck 1" };

            // Make the button id rowNumber:columnId, so for row 4, column 3, will be string 4:3
            juce::String id{ std::to_string(rowNumber) + ":" + std::to_string(columnId)};
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    // If the columnId is that for the play button (equals 2)
    if (columnId == 4)
    {
        // If pointer to existingComponent is a nullptr (no component has been created yet)
        if (existingComponentToUpdate == nullptr)
        {
            // Create the component
            juce::TextButton* btn = new juce::TextButton{ "Load into Deck 2" };

            juce::String id{ std::to_string(rowNumber) + ":" + std::to_string(columnId) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

// Normally, we need to check the Address of the Button --> but here we have nothing to compare it to
// as the buttons are not data members inside this class
void PlaylistComponent::buttonClicked(juce::Button* button)
{   
    // If add track button is clicked...
    if (button == &addButton)
    {
        // this does work in 6.1 but the syntax is quite counter-intuitive
        // Link: https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
        // - configure the dialogue
        auto fileChooserFlags =
            juce::FileBrowserComponent::canSelectFiles;

        // - launch out of the main thread
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                auto chosenFile = chooser.getResult();

                std::string trackLengthInHHMMSSFormat = "";

                // Gets the duration of the audio track
                // Attribution: https://forum.juce.com/t/get-track-length-before-it-starts-playing/44838
                // Read the audio file using formatManager to get its length in seconds
                if (auto reader = formatManager.createReaderFor(chosenFile))
                {
                    double lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
                    trackLengthInHHMMSSFormat = convertTimeInSecondsToString(lengthInSeconds);
                }

                // Get the absolute path of the file as a string
                // TODO: get just the name without the extension/format at the end
                std::string filePath = chosenFile.getFullPathName().toStdString();


                // Add the new track to the list of tracks
                tracks.push_back(Track{
                juce::URL{ chosenFile },
                    chosenFile.getFileName().toStdString(),
                     trackLengthInHHMMSSFormat,
                     filePath
                });

                juce::File targetFile = juce::File(juce::String(fullPathToFile));

                juce::FileOutputStream stream(targetFile);

                if (stream.openedOk())
                {   
                    // Overwrite the file by executing the next two commands
                    stream.setPosition(0);
                    stream.truncate();

                    for (int i = 0; i < tracks.size(); ++i)
                    {
                        stream.writeString(juce::String(tracks[i].getFilePath()));
                    }
                }

                // Updates and repaints the table component when a new track is added
                // Attribution: https://forum.juce.com/t/tablelistboxmodel-and-repaint/4915/2
                tableComponent.updateContent();
                tableComponent.repaint();

            });
    }
    else
    {
        // Converts JUCE String to std::string and then to integer
        std::string id = button->getComponentID().toStdString();

        // TODO: put this in a separate helper function
        // How to get substring before a certain char (the row-index/track-index in this case)
        // Attribution: https://stackoverflow.com/questions/15006269/c-get-substring-before-a-certain-char
        std::string trackId;
        std::string::size_type index = id.find(':');
        if (index != std::string::npos)
        {
            trackId = id.substr(0, index);
        }
        // Get the substring following the ':' (deck id)
        // Attribution: https://stackoverflow.com/questions/28163723/c-how-to-get-substring-after-a-character
        std::string deckId = id.substr(id.find(':') + 1);
        
        // Conver trackId to an integer, so that it can be used as an index
        int trackIndex = std::stoi(trackId);
        // Convert column/deckGui id to int and load url into that GUI's DJAudioPlayer
        if (std::stoi(deckId) == 3)
        {   
            int trackIndex = std::stoi(trackId);
            gui1->loadTrack(tracks[trackIndex].getUrl());
        }
        else
        {
            gui2->loadTrack(tracks[trackIndex].getUrl());
        }
    }

}


// Drag and drop functions
bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{

    // Make sure user only adds one file (at least for now)
    if (files.size() == 1)
    {
        // Convert a string filename into a file then into a URL
        juce::URL fileURL = juce::URL{ juce::File{files[0]} };
        // second argument: includeGetParameters --> if this is true and any parameters have been set with the withParameter() method, 
        // then the string will have these appended on the end and URL - encoded.
        juce::String fileURLJS = fileURL.toString(false);
        std::string fileURLString = fileURLJS.toStdString();

    }
}


/** Creates a file (if one does not already exist) in the Current Working Directory.
    * Returns the full path to that file.
*/
std::string PlaylistComponent::CreateOrLoadTrackURLsFile()
{   
    // Sets the Source folder this code is in to the Current Working Directory
    (juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getChildFile(juce::StringRef{ "Source" })).setAsCurrentWorkingDirectory();

    // Checks if file already exists, if so, load its path and return it
    if (!juce::File::getCurrentWorkingDirectory().getChildFile("songUrls.txt").existsAsFile())
    {   
        // Set the private member file called "urlsFile" to a new file in the Current Working Directory and create the file
        juce::File urlsFile = juce::File::getCurrentWorkingDirectory().getChildFile("songUrls.txt");
        urlsFile.create();
        return urlsFile.getFullPathName().toStdString();
    }
    else
    {   
        // If the file already exists because this application has already been opened, store the file in the "urlsFile" private data member
        DBG("PlaylistComponent::File already exists");
        juce::File urlsFile = juce::File::getCurrentWorkingDirectory().getChildFile("songUrls.txt");
        return urlsFile.getFullPathName().toStdString();
    }
}


/** Helper method converting the duration of the track length in seconds to a string in HH::MM::SS format
    * Returns the HH::MM::SS string
*/
std::string PlaylistComponent::convertTimeInSecondsToString(double lengthInSeconds)
{
    // Stores the string showing duration of the track
    std::string trackLength = "";

    // Attribution for time conversion: https://stackoverflow.com/questions/25696992/converting-seconds-to-hours-and-minutes-and-seconds
    // Converts seconds into HH:MM::SS format
    int roundedSeconds = (int)std::round(lengthInSeconds);
    int minutes = roundedSeconds / 60;
    int secondsRemaining = roundedSeconds % 60;
    int hours = minutes / 60;
    minutes = minutes % 60;

    // Logic to make hours into '00' instead of '0' if the value is 0, 1, 2 etc + do the same for mins and secs
    std::string hoursString = std::to_string(hours);
    std::string minutesString = std::to_string(minutes);
    std::string secondsString = std::to_string(secondsRemaining);

    // Prepend the hours/mins/secs with '0' if the time is just one digit (e.g. 7:3:56 get converted to 07:03:56)
    if (hours < 10)
    {
        hoursString = "0" + hoursString;
    }
    if (minutes < 10)
    {
        minutesString = "0" + minutesString;
    }
    if (secondsRemaining < 10)
    {
        secondsString = "0" + secondsString;
    }

    // Store track length as a string
    trackLength = trackLength + hoursString + ":" + minutesString + ":" + secondsString;

    return trackLength;
}


void PlaylistComponent::readFile(std::string path) {

    juce::File myFile = juce::File(juce::String(path));

    if (myFile.exists())
    {
        juce::FileInputStream stream(myFile);

        if (stream.openedOk())
        {
            while (!stream.isExhausted())
            {
                std::string myString = stream.readString().toStdString();
                DBG("readString: " + myString);
            }
        }
    }

}
