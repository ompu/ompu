#include "ompu/plug/host/filter_io_config.hpp"
#include "ompu/plug/host/graph_editor_panel.hpp"
#include "ompu/plug/host/internal_filter.hpp"
#include "ompu/plug/host/host_window.hpp"


namespace ompu { namespace plug { namespace host {

class NumberedBoxes
    : public juce::TableListBox
    , juce::TableListBoxModel
    , juce::Button::Listener
{
public:
    struct Listener
    {
        virtual ~Listener() = default;

        virtual void addColumn()    = 0;
        virtual void removeColumn() = 0;
        virtual void columnSelected (int columnId) = 0;
    };

    enum
    {
        plusButtonColumnId  = 128,
        minusButtonColumnId = 129
    };

    //==============================================================================
    explicit NumberedBoxes(Listener& listenerToUse, bool canCurrentlyAddColumn, bool canCurrentlyRemoveColumn)
        : juce::TableListBox ("NumberedBoxes", this),
          listener (listenerToUse),
          canAddColumn (canCurrentlyAddColumn),
          canRemoveColumn (canCurrentlyRemoveColumn)
    {
        juce::TableHeaderComponent& tableHeader = getHeader();

        for (int i = 0; i < 16; ++i)
            tableHeader.addColumn (juce::String (i + 1), i + 1, 40);

        setHeaderHeight (0);
        setRowHeight (40);

        if (juce::ScrollBar* scrollbar = getHorizontalScrollBar())
            scrollbar->setAutoHide (false);
    }

    void setSelected (int columnId)
    {
        if (juce::TextButton* button = dynamic_cast<juce::TextButton*> (getCellComponent (columnId, 0))) {
            button->setToggleState (true, juce::NotificationType::dontSendNotification);
        }
    }

    void setCanAddColumn (bool canCurrentlyAdd)
    {
        if (canCurrentlyAdd != canAddColumn)
        {
            canAddColumn = canCurrentlyAdd;

            if (auto* const button = dynamic_cast<juce::TextButton*> (getCellComponent (plusButtonColumnId, 0))) {
                button->setEnabled (true);
            }
        }
    }

    void setCanRemoveColumn (bool canCurrentlyRemove)
    {
        if (canCurrentlyRemove != canRemoveColumn)
        {
            canRemoveColumn = canCurrentlyRemove;

            if (auto* const button = dynamic_cast<juce::TextButton*> (getCellComponent (minusButtonColumnId, 0))) {
                button->setEnabled (true);
            }
        }
    }

private:
    //==============================================================================
    int getNumRows() override                                             { return 1; }
    void paintCell (juce::Graphics&, int, int, int, int, bool) override         {}
    void paintRowBackground (juce::Graphics& g, int, int, int, bool) override   { g.fillAll (juce::Colours::grey); }

    Component* refreshComponentForCell (int, int columnId, bool,
                                        Component* existingComponentToUpdate) override
    {
        auto* textButton = dynamic_cast<juce::TextButton*> (existingComponentToUpdate);
        if (!textButton) {
            textButton = new juce::TextButton("");
        }

        textButton->setButtonText (getButtonName (columnId));
        textButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight |
            juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);

        const bool isPlusMinusButton = (columnId == plusButtonColumnId || columnId == minusButtonColumnId);

        if (isPlusMinusButton)
        {
            textButton->setEnabled (columnId == plusButtonColumnId ? canAddColumn : canRemoveColumn);
        }
        else
        {
            textButton->setRadioGroupId (1, juce::NotificationType::dontSendNotification);
            textButton->setClickingTogglesState (true);

            auto const busColour = juce::Colours::green.withRotatedHue (static_cast<float> (columnId) / 5.0f);
            textButton->setColour (juce::TextButton::buttonColourId, busColour);
            textButton->setColour (juce::TextButton::buttonOnColourId, busColour.withMultipliedBrightness (2.0f));
        }

        textButton->addListener (this);

        return textButton;
    }

    //==============================================================================
    juce::String getButtonName (int columnId)
    {
        if (columnId == plusButtonColumnId)  return "+";
        if (columnId == minusButtonColumnId) return "-";

        return juce::String(columnId);
    }

    void buttonClicked (juce::Button* btn) override
    {
        auto const& text = btn->getButtonText();

        if      (text == "+") listener.addColumn();
        else if (text == "-") listener.removeColumn();
    }

    void buttonStateChanged (juce::Button* btn) override
    {
        auto const& text = btn->getButtonText();

        if (text == "+" || text == "-") return;

        if (btn->getToggleState()) {
            listener.columnSelected(text.getIntValue());
        }
    }

    //==============================================================================
    Listener& listener;
    bool canAddColumn, canRemoveColumn;
};

//==============================================================================
class FilterIOConfigurationWindow::InputOutputConfig
    : public juce::Component
    , juce::ComboBox::Listener
    , juce::Button::Listener
    , NumberedBoxes::Listener
{
public:
    explicit InputOutputConfig(FilterIOConfigurationWindow& parent, bool direction)
        : owner (parent),
          ioTitle ("ioLabel", direction ? "Input Configuration" : "Output Configuration"),
          nameLabel ("nameLabel", "Bus Name:"),
          layoutLabel ("layoutLabel", "Channel Layout:"),
          enabledToggle ("Enabled"),
          ioBuses (*this, false, false),
          isInput (direction),
          currentBus (0)
    {
        ioTitle.setFont (ioTitle.getFont().withStyle (juce::Font::bold));
        nameLabel.setFont (nameLabel.getFont().withStyle (juce::Font::bold));
        layoutLabel.setFont (layoutLabel.getFont().withStyle (juce::Font::bold));
        enabledToggle.setClickingTogglesState (true);

        layouts.addListener (this);
        enabledToggle.addListener (this);

        addAndMakeVisible (layoutLabel);
        addAndMakeVisible (layouts);
        addAndMakeVisible (enabledToggle);
        addAndMakeVisible (ioTitle);
        addAndMakeVisible (nameLabel);
        addAndMakeVisible (name);
        addAndMakeVisible (ioBuses);

        updateBusButtons();
        updateBusLayout();
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        juce::Rectangle<int> r = getLocalBounds().reduced (10);

        ioTitle.setBounds (r.removeFromTop (14));
        r.reduce (10, 0);
        r.removeFromTop (16);

        ioBuses.setBounds (r.removeFromTop (60));

        {
            juce::Rectangle<int> label = r.removeFromTop (24);

            nameLabel.setBounds (label.removeFromLeft (100));
            enabledToggle.setBounds (label.removeFromRight (80));
            name.setBounds (label);
        }

        {
            juce::Rectangle<int> label = r.removeFromTop (24);

            layoutLabel.setBounds (label.removeFromLeft (100));
            layouts.setBounds (label);
        }
    }

private:
    void updateBusButtons()
    {
        if (juce::AudioProcessor* filter = owner.getAudioProcessor())
        {
            juce::TableHeaderComponent& header = ioBuses.getHeader();
            header.removeAllColumns();

            const int n = filter->getBusCount (isInput);
            for (int i = 0; i < n; ++i)
                header.addColumn ("", i + 1, 40);

            header.addColumn ("+", NumberedBoxes::plusButtonColumnId,  20);
            header.addColumn ("-", NumberedBoxes::minusButtonColumnId, 20);

            ioBuses.setCanAddColumn    (filter->canAddBus    (isInput));
            ioBuses.setCanRemoveColumn (filter->canRemoveBus (isInput));
        }

        ioBuses.setSelected (currentBus + 1);
    }

    void updateBusLayout()
    {
        if (juce::AudioProcessor* filter = owner.getAudioProcessor())
        {
            if (juce::AudioProcessor::Bus* bus = filter->getBus (isInput, currentBus))
            {
                name.setText (bus->getName(), juce::NotificationType::dontSendNotification);

                int i;

                for (i = 1; i < juce::AudioChannelSet::maxChannelsOfNamedLayout; ++i)
                    if ((layouts.indexOfItemId(i) == -1) != bus->supportedLayoutWithChannels (i).isDisabled())
                        break;

                // supported layouts have changed
                if (i < juce::AudioChannelSet::maxChannelsOfNamedLayout)
                {
                    layouts.clear();

                    for (i = 1; i < juce::AudioChannelSet::maxChannelsOfNamedLayout; ++i)
                    {
                        juce::AudioChannelSet set = bus->supportedLayoutWithChannels (i);
                        if (! set.isDisabled())
                            layouts.addItem (set.getDescription(), i);
                    }
                }

                layouts.setSelectedId (bus->getLastEnabledLayout().size());

                const bool canBeDisabled = bus->isNumberOfChannelsSupported (0);

                if (canBeDisabled != enabledToggle.isEnabled())
                    enabledToggle.setEnabled (canBeDisabled);

                enabledToggle.setToggleState (bus->isEnabled(), juce::NotificationType::dontSendNotification);
            }
        }
    }

    //==============================================================================
    void comboBoxChanged (juce::ComboBox* combo) override
    {
        if (combo == &layouts)
        {
            if (juce::AudioProcessor* filter = owner.getAudioProcessor())
            {
                if (juce::AudioProcessor::Bus* bus = filter->getBus (isInput, currentBus))
                {
                    const int selectedNumChannels = layouts.getSelectedId();

                    if (selectedNumChannels != bus->getLastEnabledLayout().size())
                    {
                        if (juce::isPositiveAndBelow (selectedNumChannels, static_cast<int>(juce::AudioChannelSet::maxChannelsOfNamedLayout))
                         && bus->setCurrentLayoutWithoutEnabling (bus->supportedLayoutWithChannels (selectedNumChannels)))
                        {
                            if (auto* const config = owner.getConfig(!isInput)) {
                                config->updateBusLayout();
                            }

                            owner.update();
                        }
                    }
                }
            }
        }
    }

    void buttonClicked (juce::Button*) override {}

    void buttonStateChanged (juce::Button* btn) override
    {
        if (btn == &enabledToggle && enabledToggle.isEnabled())
        {
            if (juce::AudioProcessor* filter = owner.getAudioProcessor())
            {
                if (juce::AudioProcessor::Bus* bus = filter->getBus (isInput, currentBus))
                {
                    if (bus->isEnabled() != enabledToggle.getToggleState())
                    {
                        bool success;

                        if (enabledToggle.getToggleState())
                            success = bus->enable();
                        else
                            success = bus->setCurrentLayout (juce::AudioChannelSet::disabled());

                        if (success)
                        {
                            updateBusLayout();

                            if (InputOutputConfig* config = owner.getConfig (! isInput))
                                config->updateBusLayout();

                            owner.update();
                        }
                        else
                        {
                            enabledToggle.setToggleState (! enabledToggle.getToggleState(),
                                juce::NotificationType::dontSendNotification);
                        }
                    }
                }
            }
        }
    }

    //==============================================================================
    void addColumn() override
    {
        if (juce::AudioProcessor* filter = owner.getAudioProcessor())
        {
            if (filter->canAddBus (isInput))
            {
                if (filter->addBus (isInput))
                {
                    updateBusButtons();
                    updateBusLayout();

                    if (InputOutputConfig* config = owner.getConfig (! isInput))
                    {
                        config->updateBusButtons();
                        config->updateBusLayout();
                    }

                    owner.update();
                }
            }
        }
    }

    void removeColumn() override
    {
        if (juce::AudioProcessor* filter = owner.getAudioProcessor())
        {
            if (filter->getBusCount (isInput) > 1 && filter->canRemoveBus (isInput))
            {
                if (filter->removeBus (isInput))
                {
                    currentBus = juce::jmin (filter->getBusCount (isInput) - 1, currentBus);

                    updateBusButtons();
                    updateBusLayout();

                    if (auto* const config = owner.getConfig (! isInput)) {
                        config->updateBusButtons();
                        config->updateBusLayout();
                    }

                    owner.update();
                }
            }
        }
    }

    void columnSelected (int columnId) override
    {
        const int newBus = columnId - 1;
        if (currentBus != newBus)
        {
            currentBus = newBus;
            ioBuses.setSelected (currentBus + 1);
            updateBusLayout();
        }
    }

    //==============================================================================
    FilterIOConfigurationWindow& owner;
    juce::Label ioTitle, nameLabel, name, layoutLabel;
    juce::ToggleButton enabledToggle;
    juce::ComboBox layouts;
    NumberedBoxes ioBuses;
    bool isInput;
    int currentBus;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputOutputConfig)
};


FilterIOConfigurationWindow::FilterIOConfigurationWindow (juce::AudioProcessor* const p)
    : AudioProcessorEditor (p),
      title ("title", p->getName())
{
    jassert (p != nullptr);
    setOpaque (true);

    title.setFont (title.getFont().withStyle (juce::Font::bold));
    addAndMakeVisible (title);

    {
        juce::ScopedLock renderLock (p->getCallbackLock());
        p->suspendProcessing (true);
        p->releaseResources();
    }

    if (p->getBusCount (true)  > 0 || p->canAddBus (true)) {
        inConfig = std::make_unique<InputOutputConfig>(*this, true);
        addAndMakeVisible(*inConfig);
    }

    if (p->getBusCount (false) > 0 || p->canAddBus (false)) {
        outConfig = std::make_unique<InputOutputConfig>(*this, false);
        addAndMakeVisible(*outConfig);
    }

    currentLayout = p->getBusesLayout();
    setSize (400, (inConfig != nullptr && outConfig != nullptr ? 160 : 0) + 200);
}

FilterIOConfigurationWindow::~FilterIOConfigurationWindow()
{
    if (juce::AudioProcessorGraph* graph = getGraph())
    {
        if (juce::AudioProcessor* p = getAudioProcessor())
        {
            juce::ScopedLock renderLock (graph->getCallbackLock());

            graph->suspendProcessing (true);
            graph->releaseResources();

            p->prepareToPlay (graph->getSampleRate(), graph->getBlockSize());
            p->suspendProcessing (false);

            graph->prepareToPlay (graph->getSampleRate(), graph->getBlockSize());
            graph->suspendProcessing (false);
        }
    }
}

void FilterIOConfigurationWindow::paint (juce::Graphics& g)
{
     g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void FilterIOConfigurationWindow::resized()
{
    juce::Rectangle<int> r = getLocalBounds().reduced (10);

    title.setBounds (r.removeFromTop (14));
    r.reduce (10, 0);

    if (inConfig != nullptr)
        inConfig->setBounds (r.removeFromTop (160));

    if (outConfig != nullptr)
        outConfig->setBounds (r.removeFromTop (160));
}

void FilterIOConfigurationWindow::update()
{
    const juce::int32 nodeId = getNodeId();

    if (juce::AudioProcessorGraph* graph = getGraph())
        if (nodeId != -1)
            graph->disconnectNode (static_cast<juce::uint32> (nodeId));

    if (GraphDocumentComponent* graphEditor = getGraphEditor())
        if (GraphEditorPanel* panel = graphEditor->graphPanel)
            panel->updateComponents();
}

juce::int32 FilterIOConfigurationWindow::getNodeId() const
{
    if (juce::AudioProcessorGraph* graph = getGraph())
    {
        const int n = graph->getNumNodes();

        for (int i = 0; i < n; ++i)
            if (juce::AudioProcessorGraph::Node* node = graph->getNode (i))
                if (node->getProcessor() == getAudioProcessor())
                    return static_cast<juce::int32> (node->nodeId);
    }

    return -1;
}

HostWindow* FilterIOConfigurationWindow::getMainWindow() const
{
    Component* comp;

    for (int idx = 0; (comp = juce::Desktop::getInstance().getComponent(idx)) != nullptr; ++idx)
        if (HostWindow* mainWindow = dynamic_cast<HostWindow*> (comp))
            return mainWindow;

    return nullptr;
}

GraphDocumentComponent* FilterIOConfigurationWindow::getGraphEditor() const
{
    if (HostWindow* mainWindow = getMainWindow())
    {
        if (GraphDocumentComponent* graphEditor = mainWindow->getGraphEditor())
            return graphEditor;
    }

    return nullptr;
}

juce::AudioProcessorGraph* FilterIOConfigurationWindow::getGraph() const
{
    if (auto* const graphEditor = getGraphEditor()) {
        if (auto* const filterGraph = graphEditor->graph.get()) {
            return &filterGraph->getGraph();
        }
    }

    return nullptr;
}

}}} // ompu
