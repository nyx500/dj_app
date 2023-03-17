#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "Track.h"
#include "DeckGUI.h"
#include "CSVHelper.h"

//==============================================================================
/*
 */
class PlaylistComponent : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener,
    // Enables adding of files from local disk through drag-and-drop
    public juce::FileDragAndDropTarget,
    // A listener for the text input box: search functionality
    public juce::TextEditor::Listener,
    // A mouse listener to toggle search box's caret when user clicks inside/outside it
    public juce::MouseListener
{
public:
    /** Constructor: playlist must have access to the two deck GUIs and to the Main Component's custom font */
    PlaylistComponent(
        DeckGUI* _gui1,
        DeckGUI* _gui2,
        // Pass in the font from Main Component,
        juce::Font _techFont);

    /** Destructor: sets table model to nullptr to avoid memory leak */
    ~PlaylistComponent() override;

    /**
     *The paint() method gets called when a region of a component needs redrawing,
     *either because the component's repaint() method has been called, or because something
     *has happened on the screen that means a section of a window needs to be redrawn
     */
    void paint(juce::Graphics&) override;

    /** Called when this component's size has been changed */
    void resized() override;

    //=====================================Implementation of Virtual Functions inherited from TableListBoxModel============================
    /**
     *Implementation of a pure virtual function in juce::TableListBoxModel
     * Returns the number of rows which should be stored in the TableListBox.
     */
    int getNumRows() override;

    /**
     *Implementation of a pure virtual function in juce::TableListBoxModel.
     * Draws the background behind a rows in the TableListBox.
     */
    void paintRowBackground(juce::Graphics& g,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;

    /**
     *Implementation of a pure virtual function in juce::TableListBoxModel.
     * Draws one of the rows in the TableListBox.
     */
    void paintCell(juce::Graphics& g,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;

    /**
     *Implementation of a pure virtual function in juce::TableListBoxModel.
     * This is used to create or update a custom component to go in a cell (i.e. the AddToDeckGui and Delete buttons)
     */
    juce::Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        juce::Component* existingComponentToUpdate) override;

    //=====================================Implementation of Listeners' Virtual Functions============================
    /**
     *Implementation of ButtonListener virtual function which is called when user clicks on a button
     *Functionality depends on which button is clicked
     */
    void buttonClicked(juce::Button* button) override;

    /**
     *Implementation of a TextEditorListener function which is called when user presses the Enter key.
     *Gets the user input from the TextEditor (Search Box) and loops through "tracks" to compare titles/durations to user input
     *Sets the Track's "isDisplayed" property to true if the track data contains the string the user typed into the search box
     */
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;

    /**
     *Implementation of a MouseListener function which is called when user releases a click somewhere on the component
     *Hides the search cursor ("caret") when user clicks outside the search box and shows it if user clicks on the search box
     */
    void mouseUp(const juce::MouseEvent& event) override;

    //============================================Drag and Drop functions====================================================

    /**
     * JUCE docs: callback to check whether this target is interested in the set of files being offered.
     * returns true if this component wants to receive the other callbacks regarding this type of object; if it returns false, no other callbacks will be made.
     */
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    /**
     * JUCE docs: callback to indicate that the user has dropped the files onto this component.
     * when the user drops the files, this get called, and you can use the files in whatever way is appropriate.
     */
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    //=========================================================================================================================

private:
    //==================================================Private Functions======================================================

    /** Adds a new track which the user selects from the local disk to the private tracks vector and CSV file */
    void addNewTrack();

    /**
     *A helper method converting the duration of the track length in seconds to a string in HH::MM::SS format
     * Returns the HH::MM::SS format string
     */
    std::string convertTimeInSecondsToString(double timeInSeconds);

    /*
     *A method which sets which tracks to display by getting the "isDisplayed" property of every track
     * and adding the desired tracks which have this property to the tracksToDisplay vector
     */
    void loopOverTracksAndDetermineIfToDisplay(juce::String userInput);

    /*
     Method which pushes certain tracks to the tracksToDisplay vector by getting the "isDisplayed" property of every track
     * and pushing only if this value is "true"
    */
    void addTracksToDisplayToDisplayedVector();

    //==================================================Private Data Members====================================================
    // The TableListBox for storing the track lsibrary
    juce::TableListBox tableComponent;

    // Makes a file chooser in order to add files to the playlist
    // Attribution: https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    juce::FileChooser fChooser{ "Select an audio file..." };

    // Creates a format manager for the PlaylistComponent to get the duration of each track in seconds
    juce::AudioFormatManager formatManager;

    // Stores all of the tracks loaded into the library
    std::vector<Track> tracks;

    // Stores only the tracks that should be displayed after user searches for specific terms.
    // The default when program is loaded should be to display all the tracks.
    std::vector<Track> tracksToDisplay;

    // Pointers to the 2 DeckGUI elements into the PlaylistComponent, to enable loading song from lib functionality
    DeckGUI* gui1;
    DeckGUI* gui2;

    // The full path to the csv file storing the track data
    std::string fullPathToFile;

    // A CSV Helper instance which stores, reads from, writes to and generally parses data to / from the CSV tracksData file
    CSVHelper csvHelper;

    // Custom font stored here for library title
    juce::Font techFont;

    // Stores a JUCE text editor component (i.e. a text input field for the "Search" functionality)
    juce::TextEditor searchBox;
    // Stores the label for the search box
    juce::Label searchBoxLabel;

    //============================================Text and Image Buttons======================================================
    // Stores the button to clear the search criteria
    juce::TextButton clearButton{ "Clear Search" };

    // Loads the "Add" button's png image from "Source" directory
    juce::File addButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("add.png");
    // Converts the image file with the "Add" icon to a juce Image format
    juce::Image addIcon = juce::PNGImageFormat::loadFrom(addButtonImageFile);
    // Sets the button to add a track to the playlist table
    juce::ImageButton addButton{ "ADD TRACK" };

    // Loads "Delete Track" (bin) icon from the "Source" directory
    juce::File deleteButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("delete.png");
    // Converts the file with the "Delete Track" (bin) icon to a juce Image format
    juce::Image deleteIcon = juce::PNGImageFormat::loadFrom(deleteButtonImageFile);

    // Loads "Add to Deck 1" (D1) icon from Source directory
    juce::File d1ButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("D1.png");
    // Converts the file with the "D1" icon to a juce Image format
    juce::Image d1Icon = juce::PNGImageFormat::loadFrom(d1ButtonImageFile);
    // Loads "Add to Deck 2" (D2) icon from Source directory
    juce::File d2ButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("D2.png");
    // Converts the file with the "D2" icon to a juce Image format
    juce::Image d2Icon = juce::PNGImageFormat::loadFrom(d2ButtonImageFile);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};