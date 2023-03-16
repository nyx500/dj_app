#pragma once

#include <JuceHeader.h>
#include<vector>
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
    // Enables adding of files through drag-and-drop
    public juce::FileDragAndDropTarget,
    // A listener for the text input box
    public juce::TextEditor::Listener,
    // A mouse listener to hide search box's caret when user has clicked outside of it
    public juce::MouseListener
{
public:
    PlaylistComponent(
        DeckGUI* _gui1,
        DeckGUI* _gui2,
        // Pass in the font from Main Component,
        juce::Font _techFont
    );

    ~PlaylistComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;



    // Basic inheritance pattern
    // Minimum implementation of pure virtual functions in juce::TableListBoxModel
    int getNumRows() override;

    void paintRowBackground(juce::Graphics& g,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;

    void paintCell(juce::Graphics& g,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;

    // Virtual (not pure virtual) method to implement button in a cell
    juce::Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        juce::Component* existingComponentToUpdate) override;

    void buttonClicked(juce::Button* button) override;

    // Inherited method from TextEditor listener
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;

    // Inherited method from MouseListener (used to check if user has clicked outside search box)
    void mouseUp(const juce::MouseEvent& event) override;

    // Drag and drop functions
    bool isInterestedInFileDrag(const juce::StringArray& iles) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void readFile(std::string path);


private:

    /** Helper method converting the duration of the track length in seconds to a string in HH::MM::SS format
        * Returns the HH::MM::SS string
    */
    std::string convertTimeInSecondsToString(double timeInSeconds);

    juce::TableListBox tableComponent;

    // Make a file chooser in order to add files to the playlist
    // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    juce::FileChooser fChooser{ "Select an audio file..." };

    // Create a format manager for the PlaylistComponent (outside the main deckGUI component
    // as there is only one playlistComponent for an app and 2 DeckGUIs!) as it cannot access
    // the one inside the DJAudioPlayer in order to get the length/duration of a track
    // and therefore store this data in the track object
    juce::AudioFormatManager formatManager;

    // Stores all of the tracks loaded into the library
    std::vector<Track> tracks;

    // Stores only the tracks that should be displayed after user searches for specific terms
    // The default when program is loaded should be to display all the tracks
    std::vector<Track> tracksToDisplay;

    /**
     Method which iterates over all the stored tracks and calls the Track's method called determineIfShouldDisplay(inputtedText),
     * which sets the isDisplayed property to "true" only if the track data contains the input parameter string as a substring
    */
    void loopOverTracksAndDetermineIfToDisplay(juce::String userInput);

    /*
     Method which pushes certain tracks to the tracksToDisplay vector by getting the "isDisplayed" property of every track
     * and pushing only if this value is "true"
    */
    void addTracksToDisplayToDisplayedVector();

    // Pass pointers to the 2 DeckGUI elements into the PlaylistComponent, so that the chosen song is played
    // when the user clicks 'play' on it
    DeckGUI* gui1;
    DeckGUI* gui2;

    // Full path to the file storing track data
    std::string fullPathToFile;

    /** A CSV Helper instance which stores, reads from, writes to and generally parses data to / from a
    * CSV tracksData file that stores the data for the loaded tracks
    */
    CSVHelper csvHelper;

    /* Stores a JUCE text editor component (i.e. a text input field for the "Search" functionality)*/
    juce::TextEditor searchBox;
    /* Label for the search box*/
    juce::Label searchBoxLabel;
    /* Button to clear the search criteria */
    juce::TextButton clearButton{ "Clear Search" };

    // Loads Add icon from Source directory
    juce::File addButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("add.png");
    // Converts the file with the "add" icon to a juce Image format
    juce::Image addIcon = juce::PNGImageFormat::loadFrom(addButtonImageFile);
    // Button to add a track to the playlist table
    juce::ImageButton addButton{ "ADD TRACK" };
    // Loads Delete Track icon from Source directory
    juce::File deleteButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("delete.png");
    // Converts the file with the "delete" icon to a juce Image format
    juce::Image deleteIcon = juce::PNGImageFormat::loadFrom(deleteButtonImageFile);
    // Loads Add to Deck 1 icon from Source directory
    juce::File d1ButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("D1.png");
    // Converts the file with the "D1" icon to a juce Image format
    juce::Image d1Icon = juce::PNGImageFormat::loadFrom(d1ButtonImageFile);
    // Loads Add to Deck 2 icon from Source directory
    juce::File d2ButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("D2.png");
    // Converts the file with the "D2" icon to a juce Image format
    juce::Image d2Icon = juce::PNGImageFormat::loadFrom(d2ButtonImageFile);


    // Custom font
    juce::Font techFont;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};