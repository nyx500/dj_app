#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Add some basic data to the table of tracks
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");
    trackTitles.push_back("Track 7");
    trackTitles.push_back("Track 8");
    trackTitles.push_back("Track 9");
    trackTitles.push_back("Track 10");


    // getHeader() returns another kind of component called a Header (look up implementation!)
    // addColumn() function is really gnarly --> but many args are default apart from columnName, columnId, width (first 3 args)
    // ColumnId cannot be 0 in Juce 6
    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    // Play button column
    tableComponent.getHeader().addColumn("", 2, 200);

    // Delete this for now until we add a track class
    //tableComponent.getHeader().addColumn("Artist", 2, 400);

    // Add the data (model) to the tableComponent
    // This class inherits from .: IS the model, so use "this"
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);

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

    tableComponent.setBounds(0, 0, getWidth(), getHeight());

}

int PlaylistComponent::getNumRows()
{
    // just return the number of tracks in the vector
    return trackTitles.size();
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
    g.drawText(trackTitles[rowNumber],
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
    if (columnId == 2)
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

    // Converts JUCE String to std::string and then to integer
    int id = std::stoi(button->getComponentID().toStdString());

    DBG("PlaylistComponent::buttonClicked - trackTitle: ");
    DBG(trackTitles[id]);
}