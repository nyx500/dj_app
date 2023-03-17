#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================

/** Constructor: playlist must have access to the two deck GUIs and to the Main Component's custom font */
PlaylistComponent::PlaylistComponent(DeckGUI* _gui1, DeckGUI* _gui2, juce::Font _techFont) : gui1(_gui1),
gui2(_gui2),
techFont(_techFont)
{

    // Initializes the CSVHelper private data member, which either creates or loads the CSV file containing the Audio Tracks' data
    csvHelper = CSVHelper();

    // Reads the tracks stored in file (if any) and stores them in the "tracks" vector using a CSV helper method
    tracks = csvHelper.readTracksDataFromCSVFile();

    /** Sets the default tracksToDisplay vector to include all of the tracks
     *(later, the tracksToDisplay vector will be used to show ONLY the tracks that meet the user's
     *search criteria)
     */
    addTracksToDisplayToDisplayedVector();

    // Get the header (column names) from the juce::TableListBox table component
    // Remember: a ColumnId cannot be 0 in Juce 6, must begin counting with 1
    tableComponent.getHeader().setStretchToFitActive(true);

    //===========================================Add Columns to the TableListBox======================================
    // Adds (1) column header for track title
    tableComponent.getHeader().addColumn("Title", 1, 120, 100, getParentWidth() / 3);
    // Adds (2) column header for track duration
    tableComponent.getHeader().addColumn("Length", 2, 80, 60, getParentWidth() / 5);
    // Adds (3) column header for button which adds track to DeckGUI1 when clicked
    tableComponent.getHeader().addColumn("", 3, 50);
    // Adds (4) column header for button which adds track to DeckGUI2 when clicked
    tableComponent.getHeader().addColumn("", 4, 50);
    // Adds (5) column header for button which deletes track from Tracks vector/playlist
    tableComponent.getHeader().addColumn("", 5, 50);

    // Set background to black and outline to white for the table component
    tableComponent.setColour(juce::TableListBox::ColourIds::backgroundColourId, juce::Colours::black);
    tableComponent.setColour(juce::TableListBox::ColourIds::outlineColourId, juce::Colours::white);

    // Allows the tableComponent access to this component's data (this is the "model", due to inheritance from TableListBoxModel JUCE class)
    tableComponent.setModel(this);

    // Add the plus icon to the "Add Track" image button
    addButton.setImages(true, true, true, addIcon, 1.0, juce::Colour(), juce::Image(), 0.7, juce::Colour(255, 255, 255), juce::Image(), 1.0, juce::Colour(255, 255, 255), 0.0f);
    // Display the "Add Track" button
    addAndMakeVisible(addButton);
    // Add the component/listener to the Add Track button
    addButton.addListener(this);

    // Display the table
    addAndMakeVisible(tableComponent);

    //=============================Search Box Functionality=================================================

    // Set the earch box label
    addAndMakeVisible(searchBoxLabel);
    searchBoxLabel.setJustificationType(juce::Justification::topLeft);
    searchBoxLabel.setText("Press 'Enter' to submit the search term:", juce::dontSendNotification);

    // Add the TextEditor class "Search Box" and the TextEditor listener this component inherits from
    addAndMakeVisible(searchBox);
    searchBox.addListener(this);

    // Registers a listener for when mouse events occur in this component, such as user clicking inside/outside the search box
    searchBox.addMouseListener(this, true);

    // Restrict invalid input (for security programming purposes): no strings over 100 chars allowed, avoid buffer overflow
    searchBox.setInputRestrictions(100, {});

    // Sets the search box background colour to very dark blue and the outline to white
    searchBox.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colour(1, 2, 28));
    searchBox.setColour(juce::TextEditor::ColourIds::outlineColourId, juce::Colours::white);

    // Change colour of search box outline when in focus
    searchBox.setColour(juce::TextEditor::ColourIds::focusedOutlineColourId, juce::Colours::blue);
    searchBox.setHasFocusOutline(true);

    // Functionality for the "Clear Search" button to clear the results of the search and therefore to display all the tracks in the playlist
    addAndMakeVisible(clearButton);
    // Makes the "Clear Search" button bright blue with black text
    clearButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(14, 135, 250));
    clearButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::black);
    clearButton.addListener(this);

    // A formatManager to be able to get the duration of each track in seconds and to store this data inside the "Track" object
    formatManager.registerBasicFormats();
}

/** Destructor: sets table model to nullptr to avoid memory leak */
PlaylistComponent::~PlaylistComponent()
{
    // Attribution: FIXES MEMORY LEAK IN DEBUGGER ON SHUTDOWN https://forum.juce.com/t/issue-with-tablelistbox-and-accessibility/53866
    tableComponent.setModel(nullptr);
}

/**
 *The paint() method gets called when a region of a component needs redrawing,
 *either because the component's repaint() method has been called, or because something
 *has happened on the screen that means a section of a window needs to be redrawn
 */
void PlaylistComponent::paint(juce::Graphics& g)
{
    // Clear the background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Set background rectangle to black
    g.setColour(juce::Colours::black);
    g.fillRect(getLocalBounds());

    // Outlines the component with white
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds());

    // Draws the title ("Music Library") in custom robot font in white at the TOP LEFT of the PlaylistComponent
    g.setColour(juce::Colours::white);
    g.setFont(techFont);
    g.setFont(28.0f);
    g.drawText("Music Library", getWidth() * 0.1, getHeight() * 0.03, getWidth(), getHeight() * 0.3, juce::Justification::topLeft, true);
}

/** Called when this component's size has been changed */
void PlaylistComponent::resized()
{
    // Places search box TextEditor component and its label at the top of the Playlist/Music Library
    searchBoxLabel.setBounds(getWidth() * 0.05, getHeight() * 0.12, getWidth(), getHeight() * 0.08);
    searchBox.setBounds(getWidth() * 0.05, getHeight() * 0.20, getWidth() * 0.9, getHeight() * 0.05);

    // Places the clear button below the search box/label
    clearButton.setBounds(getWidth() * 0.1, getHeight() * 0.28, getWidth() * 0.8, getHeight() * 0.05);

    // Puts the add track button & the actual table storing tracks at the bottom of the Playlist Component, below the title and search box
    addButton.setBounds(getWidth() * 0.78, getHeight() * 0.35, getWidth() * 0.15, getHeight() * 0.15);
    tableComponent.setBounds(getWidth() * 0.02, getHeight() * 0.5, getWidth() * 0.96, getHeight() * 0.48);
}

//=====================================Implementation of Virtual Functions inherited from TableListBoxModel============================
/**
 *Implementation of a pure virtual function in juce::TableListBoxModel
 * Returns the number of rows which should be stored in the TableListBox
 */
int PlaylistComponent::getNumRows()
{
    // Simply returns the number of the tracks which should be displayed (are stored in the tracksToDisplay vector)
    return tracksToDisplay.size();
}

/**
 *Implementation of a pure virtual function in juce::TableListBoxModel.
 * Draws the background behind a rows in the TableListBox.
 */
void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    // When user clicks on ("selects") the row storing a particular track, paint it red
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::red);
    }
    else
    {
        // When tracks are not being clicked on ("selected") paint every other track darkest blue and some a lighter blue
        if (rowNumber % 2 == 0)
        {
            g.fillAll(juce::Colours::darkblue);
        }
        else
        {
            // Even darker blue, almost black
            g.fillAll(juce::Colour(1, 2, 28));
        }
    }
}

/**
 *Implementation of a pure virtual function in juce::TableListBoxModel.
 *Draws one of the rows in the TableListBox.
 */
void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    // 'g', the graphics object, is the cell in this context

    // Make the track data text white (on the dark blue background of the row)
    g.setColour(juce::Colours::white);

    // First column: enter track title data and position it on the left of the cell
    if (columnId == 1)
    {
        g.drawText(tracksToDisplay[rowNumber].getTitle(),
            2, 0, width - 4, height,
            juce::Justification::centredLeft,
            true);
    }

    // Second column: enter track duration data and position it on the left of the cell
    if (columnId == 2)
    {
        g.drawText(tracksToDisplay[rowNumber].getDuration(),
            2, 0, width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
}

/**
 *Implementation of a pure virtual function in juce::TableListBoxModel.
 * This is used to create or update a custom component to go in a cell (i.e. the AddToDeckGui and Delete buttons)
 */
juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    // Column with ID 3: the AddToDeckGui1 (D1) button should go here
    if (columnId == 3)
    {
        // If the pointer to the existingComponent is a nullptr, means no component has been created yet, so create the component!
        if (existingComponentToUpdate == nullptr)
        {
            // Creates the Load into Deck 1 ImageButton for the 3rd column for each Track (has a "D1" icon)
            juce::ImageButton* btn = new juce::ImageButton{ "Load into Deck 1" };

            // Sets the D1 icon for the ImageButton
            btn->setImages(true, true, true, d1Icon, 1.0, juce::Colour(),
                juce::Image(), 0.5, juce::Colour(255, 255, 255),
                juce::Image(), 1.0, juce::Colour(255, 255, 255), 0.0f);

            // Sets each button's ID to "rowNumber:columnId", so for row 4, column 3, this will be string 4:3
            juce::String id{ std::to_string(rowNumber) + ":" + std::to_string(columnId) };
            btn->setComponentID(id);

            // Adds a ButtonListener (this class) to every Load into Deck 1 button
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    // Column with ID 4: the AddToDeckGui1 (D2) button should go here
    if (columnId == 4)
    {
        // If the pointer to the existingComponent is a nullptr, means no component has been created yet, so create the component!
        if (existingComponentToUpdate == nullptr)
        {
            // Creates the Load into Deck 2 ImageButton for the 4th column for each Track (has a "D2" icon)
            juce::ImageButton* btn = new juce::ImageButton{ "Load into Deck 2" };

            // Sets the D2 icon for the ImageButton
            btn->setImages(true, true, true, d2Icon, 1.0, juce::Colour(),
                juce::Image(), 0.5, juce::Colour(255, 255, 255),
                juce::Image(), 1.0, juce::Colour(0, 0, 255), 0.0f);

            // Sets each button's ID to "rowNumber:columnId", so for row 4, column 4, this will be string 4:4
            juce::String id{ std::to_string(rowNumber) + ":" + std::to_string(columnId) };
            btn->setComponentID(id);

            // Adds a ButtonListener (this class) to every Load into Deck 2 button
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    // Column with ID 5: the "bin" icon button to delete the track in the current row should go here
    if (columnId == 5)
    {
        // If the pointer to the existingComponent is a nullptr, means no component has been created yet, so create the component!
        if (existingComponentToUpdate == nullptr)
        {
            // Creates the Delete ImageButton for the 5rd column for each Track (has a "Bin" icon)
            juce::ImageButton* btn = new juce::ImageButton{ "Delete" };

            // Sets the "Bin" icon for the ImageButton
            btn->setImages(true, true, true, deleteIcon, 1.0, juce::Colour(),
                juce::Image(), 0.5, juce::Colour(255, 255, 255),
                juce::Image(), 1.0, juce::Colour(255, 255, 255), 0.0f);

            // Sets each button's ID to "rowNumber:columnId", so for row 4, column 5, this will be string 4:5
            juce::String id{ std::to_string(rowNumber) + ":" + std::to_string(columnId) };
            btn->setComponentID(id);

            // Adds a ButtonListener (this class) to every Delete button
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    // Return the component (a button) which is to be added
    return existingComponentToUpdate;
}


//=====================================Implementation of Listeners' Virtual Functions============================
/**
 *Implementation of ButtonListener virtual function which is called when user clicks on a button
 *Functionality depends on which button is clicked
 */
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // If the "Add Track" button is clicked, adds a new track to the "tracks" vector and writes it to the CSV file
    if (button == &addButton)
    {
        addNewTrack();
    }
    else if (button == &clearButton)
    {
        /* Resets the user input to an empty string : this means that the function called loopOverTracksAndDetermineIfToDisplay will
         * set the "isDisplayed" property for each Track to true */
        loopOverTracksAndDetermineIfToDisplay(juce::String(""));
        // Adds all of the tracks which have "isDisplayed" set to true to the "tracksToDisplay" vector
        addTracksToDisplayToDisplayedVector();
        // Clears the search box of any user input
        searchBox.clear();

        // Redraws the table
        tableComponent.updateContent();
        tableComponent.repaint();
    }
    else
    {
        // Converts JUCE String ComponentID to std::string and then to an integer
        std::string id = button->getComponentID().toStdString();

        // Attribution: https://stackoverflow.com/questions/15006269/c-get-substring-before-a-certain-char
        // How to get a substring before a certain char (the row-index/track-index in this case)
        std::string trackId;

        std::string::size_type index = id.find(':');

        // Get the track (table ROW) id - before the ':' character in the Component's/Button's ID
        if (index != std::string::npos)
        {
            trackId = id.substr(0, index);
        }

        // Converts the trackId to an integer, so that it can be used as an index to get the correct track from the tracksToDisplay vector
        int trackIndex = std::stoi(trackId);

        // Get the substring AFTER the ':' to determine whether this button is to load into D1/D2 or to delete the Track in that row
        // Attribution: https://stackoverflow.com/questions/28163723/c-how-to-get-substring-after-a-character
        // How to get a substring after a certain char (the column-index/load/delete button in this case)
        int columnId = std::stoi(id.substr(id.find(':') + 1));

        // If the column ID is 3, load the track in the trackID/row index into DeckGUI1
        if (columnId == 3)
        {
            gui1->loadTrack(tracksToDisplay[trackIndex].getUrl());
        }

        // If the column ID is 4, load the track in the trackID/row index into DeckGUI2
        if (columnId == 4)
        {
            gui2->loadTrack(tracksToDisplay[trackIndex].getUrl());
        }

        // If the column ID is 5, then delete the track from the Music Library (the PlaylistComponent class)
        if (columnId == 5)
        {
            // IMPORTANT:
            // Attribution1 (deleting C++ vector elements): https://www.tutorialspoint.com/cplusplus-program-to-remove-items-from-a-given-vector
            // Attribution 2: https://iq.opengenus.org/ways-to-remove-elements-from-vector-cpp/
            tracksToDisplay.erase(tracksToDisplay.begin() + (trackIndex));

            // Also erase the track from the hidden track store (tracks which are not currently displayed)
            tracks.erase(tracks.begin() + (trackIndex));

            // Updates the CSV file after deleting the track
            csvHelper.writeTracksDataIntoCSVFile(tracks);

            // Updates and repaints the table component when the track is deleted from the PlaylsitComponent
            tableComponent.updateContent();
            tableComponent.repaint();
        }
    }
}

/**
 *Implementation of a TextEditorListener function which is called when user presses the Enter key.
 *Gets the user input from the TextEditor (Search Box) and loops through "tracks" to compare titles/durations to user input
 *Sets the Track's "isDisplayed" property to true if the track data contains the string the user typed into the search box
 */
void PlaylistComponent::textEditorReturnKeyPressed(juce::TextEditor& textEditor)
{
    // Compare the addresses of input and the searchBox member variable
    if (&textEditor == &searchBox)
    {
        // Get user input from search box
        juce::String inputtedText = textEditor.getText();

        // Determines which tracks should have isDisplayed set to true based on the user input string
        loopOverTracksAndDetermineIfToDisplay(inputtedText);

        // Adds the tracks which now have isDisplayed set to true to the "tracksToDisplay" vector
        addTracksToDisplayToDisplayedVector();

        // Updates table
        tableComponent.updateContent();
        tableComponent.repaint();
    }
}

/**
 *Implementation of a MouseListener function which is called when user releases a click somewhere on the component
 *Hides the search cursor ("caret") when user clicks outside the search box and shows it if user clicks on the search box
 */
void PlaylistComponent::mouseUp(const juce::MouseEvent& event)
{
    // event.eventComponent returns a pointer to the component where mouse was clicked
    // Check if the component clicked on was the searchBox
    if (event.eventComponent == &searchBox)
    {
        // Displays the caret (textbox cursor)
        searchBox.setCaretVisible(true);
    }
    else
    {
        // Hides the caret if user clicks outside the search box
        searchBox.setCaretVisible(false);
    }
}

//============================================= = Drag and Drop functions====================================================

/**
 * JUCE docs: callback to check whether this target is interested in the set of files being offered.
 * returns true if this component wants to receive the other callbacks regarding this type of object; if it returns false, no other callbacks will be made.
 */
bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}

/**
 * JUCE docs: callback to indicate that the user has dropped the files onto this component.
 * when the user drops the files, this get called, and you can use the files in whatever way is appropriate.
 */
void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{

    // Make sure user only adds one file
    if (files.size() == 1)
    {
        // Converts a string filename into a file then into a URL
        juce::URL fileURL = juce::URL{ juce::File{files[0]} };

        // 2nd parameter: "includeGetParameters" --> if this is true and any parameters have been set with the withParameter() method
        juce::String fileURLJS = fileURL.toString(false);

        // Converts the fileURLString to a standard string
        std::string fileURLString = fileURLJS.toStdString();
    }
}

/** Adds a new track which the user selects from the local disk to the private tracks vector and CSV file */
void PlaylistComponent::addNewTrack()
{
    // File drag-drop for JUCE 6:
    // Attribution: https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    auto fileChooserFlags =
        juce::FileBrowserComponent::canSelectFiles;

    // Launches out of the main thread
    fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            auto chosenFile = chooser.getResult();

            std::string trackLengthInHHMMSSFormat = "";

            // Gets the duration of the audio track
            // Attribution: https://forum.juce.com/t/get-track-length-before-it-starts-playing/44838
            // Read the audio file using the formatManager data member to store the track's length in seconds
            if (auto reader = formatManager.createReaderFor(chosenFile))
            {
                double lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
                // Call the custom-made convertTimeInSecondsToString() function to get the duration as HH:MM:SS
                trackLengthInHHMMSSFormat = convertTimeInSecondsToString(lengthInSeconds);

                // Free up the memory when done reading the length of the audio file
                delete reader;
                reader = nullptr;
            }

            // Gets the absolute path of each audio file as a string
            std::string filePath = chosenFile.getFullPathName().toStdString();

            // Adds the new Track's data to the "tracks" vector
            tracks.push_back(Track
                {
                    // The track index in the tracks vector
                    tracks.size(),
                    // Track URL
                    juce::URL{ chosenFile },
                    // Track title
                    chosenFile.getFileNameWithoutExtension().toStdString(),
                    // Track extension (e.g. mp3)
                    chosenFile.getFileExtension().toStdString(),
                    // Track duration in hrs, mins, seconds
                    trackLengthInHHMMSSFormat,
                    // Absolute path to the track
                    filePath
                });

            // Writes the track data for all the tracks after adding the new track into the CSV File
            csvHelper.writeTracksDataIntoCSVFile(tracks);

            // Displays all the tracks after a new track is added, thus clearing any previous search results
            loopOverTracksAndDetermineIfToDisplay(juce::String(""));
            addTracksToDisplayToDisplayedVector();
            // Clear any search criteria
            searchBox.clear();

            // Update and repaint the table component whenever a new track is added, so that it is displayed
            // Attribution: https://forum.juce.com/t/tablelistboxmodel-and-repaint/4915/2
            tableComponent.updateContent();
            tableComponent.repaint(); });
}

/**
 *A helper method converting the duration of the track length in seconds to a string in HH::MM::SS format
 * Returns the HH::MM::SS format string
 */
std::string PlaylistComponent::convertTimeInSecondsToString(double lengthInSeconds)
{
    // Stores the string showing duration of the track
    std::string trackLength = "";

    // Attribution: https://stackoverflow.com/questions/25696992/converting-seconds-to-hours-and-minutes-and-seconds
    // Converts seconds into HH:MM::SS format
    int roundedSeconds = (int)std::round(lengthInSeconds);
    int minutes = roundedSeconds / 60;
    int secondsRemaining = roundedSeconds % 60;
    int hours = minutes / 60;
    minutes = minutes % 60;

    // Decisions/logic to make hours into '00' instead of '0' if the value is 0, 1, 2 etc + do the same for mins and secs
    std::string hoursString = std::to_string(hours);
    std::string minutesString = std::to_string(minutes);
    std::string secondsString = std::to_string(secondsRemaining);

    // Prepends the hours/mins/secs with '0' if the time is just one digit (e.g. 7:3:56 get converted to 07:03:56)
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

    // Stores the track length as an HH:MM:SS string
    trackLength = trackLength + hoursString + ":" + minutesString + ":" + secondsString;

    // Returns the formatted time string
    return trackLength;
}

/**
 * A method which iterates over all the stored tracks and calls the Track's member function called determineIfShouldDisplay,
 * which sets the isDisplayed property to "true" only if the track title/duration contains the input parameter string as a substring
 */
void PlaylistComponent::loopOverTracksAndDetermineIfToDisplay(juce::String userInput)
{
    // Checks if each track contains the typed substring in its title or extension
    for (Track& t : tracks)
    {
        t.determineIfShouldDisplay(userInput);
    }
}

/**
 *A method which sets which tracks to display by getting the "isDisplayed" property of every track
 * and adding the desired tracks which have this property to the tracksToDisplay vector
 */
void PlaylistComponent::addTracksToDisplayToDisplayedVector()
{
    // Clears the current vector of tracks to display
    tracksToDisplay.clear();

    // Iterates over every Track stored in "tracks", and pushes only the tracks with isDisplayed set to true to the "tracksToDisplay" vector
    for (Track& t : tracks)
    {
        if (t.getIsDisplayed())
        {
            tracksToDisplay.push_back(t);
        }
    }
}
