#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.


    // getHeader() returns another kind of component called a Header (look up implementation!)
    // addColumn() function is really gnarly --> but many args are default apart from columnName, columnId, width (first 3 args)
    // ColumnId cannot be 0 in Juce 6
    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    // Add Column for track duration
    tableComponent.getHeader().addColumn("Track Duration", 2, 200);
    // Play button column
    tableComponent.getHeader().addColumn("", 3, 200);

    // Delete this for now until we add a track class
    //tableComponent.getHeader().addColumn("Artist", 2, 400);

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
    g.drawText(tracks[rowNumber].title,
        2, 0, width - 4, height,
        juce::Justification::centredLeft,
        true);
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
            juce::TextButton* btn = new juce::TextButton{ "Play" };

            juce::String id{ std::to_string(rowNumber) };
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
        DBG("add button was clicked!!!!!!");
        // this does work in 6.1 but the syntax is quite counter-intuitive
        // Link: https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
        // - configure the dialogue
        auto fileChooserFlags =
            juce::FileBrowserComponent::canSelectFiles;

        // - launch out of the main thread
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                auto chosenFile = chooser.getResult();

                // Stores the string showing duration of the track
                std::string trackLength = "";

                // Gets the duration of the audio track
                // Attribution: https://forum.juce.com/t/get-track-length-before-it-starts-playing/44838
                // Read the audio file using formatManager to get its length in seconds
                if (auto reader = formatManager.createReaderFor(chosenFile))
                {
                    double lengthInSeconds = reader->lengthInSamples / reader->sampleRate;

                    // Attribution for time conversion: https://stackoverflow.com/questions/25696992/converting-seconds-to-hours-and-minutes-and-seconds
                    int roundedSeconds = (int)std::round(lengthInSeconds);
                    int minutes = roundedSeconds / 60;
                    int secondsRemaining = roundedSeconds % 60;
                    int hours = minutes / 60;
                    minutes = minutes % 60;

                    // Logic to make hours into '00' instead of '0' if the value is 0, 1, 2 etc + do the same for mins and secs
                    // TODO: put all this in a separate function
                    std::string hoursString = std::to_string(hours);
                    std::string minutesString= std::to_string(minutes);
                    std::string secondsString = std::to_string(secondsRemaining);
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

                    trackLength = trackLength + hoursString + ":" + minutesString + ":" + secondsString;

                    DBG(trackLength);
                }

                
                // Add the new track to the list of tracks
                tracks.push_back(Track{
                juce::URL{ chosenFile },
                    chosenFile.getFileName().toStdString(),
                    "",
                    trackLength
                });

                // Updates and repaints the table component when a new track is added
                // Attribution: https://forum.juce.com/t/tablelistboxmodel-and-repaint/4915/2
                tableComponent.updateContent();
                tableComponent.repaint();

            });
    }
    else
    {
        // Converts JUCE String to std::string and then to integer
        int id = std::stoi(button->getComponentID().toStdString());
        DBG("PlaylistComponent::buttonClicked - trackTitle: ");
        DBG(tracks[id].title);
    }

}


// Drag and drop functions
bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("PlaylistComponent::isInterestedInFileDrag");
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    DBG("PlaylistComponent::filesDropped");

    // Make sure user only adds one file (at least for now)
    if (files.size() == 1)
    {
        // Convert a string filename into a file then into a URL
        juce::URL fileURL = juce::URL{ juce::File{files[0]} };
        // second argument: includeGetParameters --> if this is true and any parameters have been set with the withParameter() method, 
        // then the string will have these appended on the end and URL - encoded.
        juce::String fileURLJS = fileURL.toString(false);
        std::string fileURLString = fileURLJS.toStdString();

        DBG(fileURLString);
    }
}