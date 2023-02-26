#pragma once

#include <JuceHeader.h>
#include<vector>
#include <string>
#include "Track.h"
#include "DeckGUI.h"


//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener,
    // Enables adding of files through drag-and-drop
    public juce::FileDragAndDropTarget
{
public:
    PlaylistComponent(
        DeckGUI* _gui1,
        DeckGUI* _gui2
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

    // Drag and drop functions
    bool isInterestedInFileDrag(const juce::StringArray& iles) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;


private:
    
    // Button to add a track to the playlist table
    juce::TextButton addButton{ "ADD NEW TRACK" };

    juce::TableListBox tableComponent;

    // Make a file chooser in order to add files to the playlist
    // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    juce::FileChooser fChooser{ "Select a file..." };

    // Create a format manager for the PlaylistComponent (outside the main deckGUI component
    // as there is only one playlistComponent for an app and 2 DeckGUIs!) as it cannot access
    // the one inside the DJAudioPlayer in order to get the length/duration of a track
    // and therefore store this data in the track object
    juce::AudioFormatManager formatManager;

    std::vector<Track> tracks;

    // Pass pointers to the 2 DeckGUI elements into the PlaylistComponent, so that the chosen song is played
    // when the user clicks 'play' on it
    DeckGUI* gui1;
    DeckGUI* gui2;

    // Stores the URLs of the songs
    juce::File urlsFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)

};
