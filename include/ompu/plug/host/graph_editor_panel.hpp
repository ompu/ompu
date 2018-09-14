#pragma once

#include "ompu/plug/host/filter_graph.hpp"


namespace ompu { namespace plug { namespace host {

struct FilterComponent;
struct ConnectorComponent;
struct PinComponent;

class GraphEditorPanel
    : public juce::Component
    , public juce::ChangeListener
{
public:
    GraphEditorPanel(FilterGraph& graph);
    ~GraphEditorPanel();

    void paint (juce::Graphics& g);
    void mouseDown (const juce::MouseEvent& e);

    void createNewPlugin (const juce::PluginDescription&, juce::Point<int> position);

    FilterComponent* getComponentForFilter (juce::uint32 filterID) const;
    ConnectorComponent* getComponentForConnection (const juce::AudioProcessorGraph::Connection& conn) const;
    PinComponent* findPinAt (juce::Point<float>) const;

    void resized();
    void changeListenerCallback (juce::ChangeBroadcaster*);
    void updateComponents();

    //==============================================================================
    void beginConnectorDrag (juce::uint32 sourceFilterID, int sourceFilterChannel,
                             juce::uint32 destFilterID, int destFilterChannel,
                             const juce::MouseEvent& e);
    void dragConnector (const juce::MouseEvent& e);
    void endDraggingConnector (const juce::MouseEvent& e);

    //==============================================================================
private:
    FilterGraph& graph;
    std::unique_ptr<ConnectorComponent> draggingConnector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphEditorPanel)
};


//==============================================================================
/**
    A panel that embeds a GraphEditorPanel with a midi keyboard at the bottom.

    It also manages the graph itself, and plays it.
*/
class GraphDocumentComponent
    : public juce::Component
{
public:
    GraphDocumentComponent(
        juce::AudioPluginFormatManager& formatManager,
        juce::AudioDeviceManager& deviceManager
    );
    ~GraphDocumentComponent();

    void createNewPlugin (const juce::PluginDescription&, juce::Point<int> position);
    void setDoublePrecision (bool doublePrecision);

    std::unique_ptr<FilterGraph> graph;

    void resized() override;
    void unfocusKeyboardComponent();

    void releaseGraph();

private:
    juce::AudioDeviceManager& deviceManager;
    juce::AudioProcessorPlayer graphPlayer;
    juce::MidiKeyboardState keyState;

public:
    GraphEditorPanel* graphPanel;

private:
    juce::Component* keyboardComp;
    juce::Component* statusBar;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphDocumentComponent)
};



// A desktop window containing a plugin's UI

class PluginWindow
    : public juce::DocumentWindow
{
public:
    enum WindowFormatType
    {
        Normal = 0,
        Generic,
        Programs,
        Parameters,
        AudioIO,
        NumTypes
    };

    PluginWindow(juce::AudioProcessorEditor*, juce::AudioProcessorGraph::Node*, WindowFormatType);
    ~PluginWindow();

    static PluginWindow* getWindowFor (juce::AudioProcessorGraph::Node*, WindowFormatType);

    static void closeCurrentlyOpenWindowsFor (const juce::uint32 nodeId);
    static void closeAllCurrentlyOpenWindows();

    void moved() override;
    void closeButtonPressed() override;

private:
    juce::AudioProcessorGraph::Node* owner;
    WindowFormatType type;

    float getDesktopScaleFactor() const override     { return 1.0f; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginWindow)
};

inline juce::String toString (PluginWindow::WindowFormatType type)
{
    switch (type)
    {
        case PluginWindow::Normal:     return "Normal";
        case PluginWindow::Generic:    return "Generic";
        case PluginWindow::Programs:   return "Programs";
        case PluginWindow::Parameters: return "Parameters";
        default:                       return {};
    }
}

inline juce::String getLastXProp (PluginWindow::WindowFormatType type)    { return "uiLastX_" + toString (type); }
inline juce::String getLastYProp (PluginWindow::WindowFormatType type)    { return "uiLastY_" + toString (type); }
inline juce::String getOpenProp  (PluginWindow::WindowFormatType type)    { return "uiopen_"  + toString (type); }

}}} // ompu
