#include "ompu/plug/host/host_app.hpp"
#include "ompu/plug/host/host_window.hpp"
#include "ompu/plug/host/filter_graph.hpp"
#include "ompu/plug/host/internal_filter.hpp"
#include "ompu/plug/host/graph_editor_panel.hpp"


namespace ompu { namespace plug { namespace host {

const int FilterGraph::midiChannelNumber = 0x1000;

FilterGraph::FilterGraph (juce::AudioPluginFormatManager& fm)
    : juce::FileBasedDocument (filenameSuffix,
                         filenameWildcard,
                         "Load a filter graph",
                         "Save a filter graph"),
      formatManager (fm)
{
    InternalPluginFormat internalFormat;

    addFilter (internalFormat.audioInDesc,  { 0.5,  0.1 });
    addFilter (internalFormat.midiInDesc,   { 0.25, 0.1 });
    addFilter (internalFormat.audioOutDesc, { 0.5,  0.9 });

    graph.addListener (this);

    setChangedFlag (false);
}

FilterGraph::~FilterGraph()
{
    graph.addListener (this);
    graph.clear();
}

juce::uint32 FilterGraph::getNextUID() noexcept
{
    return ++lastUID;
}

//==============================================================================
int FilterGraph::getNumFilters() const noexcept
{
    return graph.getNumNodes();
}

juce::AudioProcessorGraph::Node::Ptr FilterGraph::getNode (int index) const noexcept
{
    return graph.getNode (index);
}

juce::AudioProcessorGraph::Node::Ptr FilterGraph::getNodeForId (juce::uint32 uid) const
{
    return graph.getNodeForId (uid);
}

juce::AudioProcessorGraph::Node::Ptr FilterGraph::getNodeForName (const juce::String& name) const
{
    for (int i = 0; i < graph.getNumNodes(); i++)
        if (auto node = graph.getNode (i))
            if (auto p = node->getProcessor())
                if (p->getName().equalsIgnoreCase (name))
                    return node;

    return nullptr;
}

void FilterGraph::addFilter (const juce::PluginDescription& desc, juce::Point<double> p)
{
    struct AsyncCallback : juce::AudioPluginFormat::InstantiationCompletionCallback
    {
        explicit AsyncCallback (FilterGraph& g, juce::Point<double> pos)  : owner (g), position (pos)
        {}

        void completionCallback (juce::AudioPluginInstance* instance, const juce::String& error) override
        {
            owner.addFilterCallback (instance, error, position);
        }

        FilterGraph& owner;
        juce::Point<double> position;
    };

    formatManager.createPluginInstanceAsync (desc, graph.getSampleRate(), graph.getBlockSize(),
                                             new AsyncCallback (*this, p));
}

void FilterGraph::addFilterCallback (juce::AudioPluginInstance* instance, const juce::String& error, juce::Point<double> pos)
{
    if (instance == nullptr)
    {
        juce::AlertWindow::showMessageBox (juce::AlertWindow::WarningIcon,
                                     TRANS("Couldn't create filter"),
                                     error);
    }
    else
    {
        instance->enableAllBuses();

        if (auto* node = graph.addNode (instance))
        {
            node->properties.set ("x", pos.x);
            node->properties.set ("y", pos.y);
            changed();
        }
    }
}

void FilterGraph::removeFilter (const juce::uint32 id)
{
    PluginWindow::closeCurrentlyOpenWindowsFor (id);

    if (graph.removeNode (id))
        changed();
}

void FilterGraph::disconnectFilter (const juce::uint32 id)
{
    if (graph.disconnectNode (id))
        changed();
}

void FilterGraph::removeIllegalConnections()
{
    if (graph.removeIllegalConnections())
        changed();
}

void FilterGraph::setNodePosition (const juce::uint32 nodeId, double x, double y)
{
    if (juce::AudioProcessorGraph::Node::Ptr n = graph.getNodeForId (nodeId))
    {
        n->properties.set ("x", juce::jlimit (0.0, 1.0, x));
        n->properties.set ("y", juce::jlimit (0.0, 1.0, y));
    }
}

juce::Point<double> FilterGraph::getNodePosition (const juce::uint32 nodeId) const
{
    if (auto n = graph.getNodeForId (nodeId))
        return { static_cast<double> (n->properties ["x"]),
                 static_cast<double> (n->properties ["y"]) };

    return {};
}

//==============================================================================
int FilterGraph::getNumConnections() const noexcept
{
    return graph.getNumConnections();
}

const juce::AudioProcessorGraph::Connection* FilterGraph::getConnection (const int index) const noexcept
{
    return graph.getConnection (index);
}

const juce::AudioProcessorGraph::Connection* FilterGraph::getConnectionBetween (juce::uint32 sourceFilterUID, int sourceFilterChannel,
    juce::uint32 destFilterUID, int destFilterChannel) const noexcept
{
    return graph.getConnectionBetween (sourceFilterUID, sourceFilterChannel,
                                       destFilterUID, destFilterChannel);
}

bool FilterGraph::canConnect (juce::uint32 sourceFilterUID, int sourceFilterChannel,
    juce::uint32 destFilterUID, int destFilterChannel) const noexcept
{
    return graph.canConnect (sourceFilterUID, sourceFilterChannel,
                             destFilterUID, destFilterChannel);
}

bool FilterGraph::addConnection (juce::uint32 sourceFilterUID, int sourceFilterChannel,
    juce::uint32 destFilterUID, int destFilterChannel)
{
    const bool result = graph.addConnection (sourceFilterUID, sourceFilterChannel,
                                             destFilterUID, destFilterChannel);

    if (result)
        changed();

    return result;
}

void FilterGraph::removeConnection (const int index)
{
    graph.removeConnection (index);
    changed();
}

void FilterGraph::removeConnection (juce::uint32 sourceFilterUID, int sourceFilterChannel,
    juce::uint32 destFilterUID, int destFilterChannel)
{
    if (graph.removeConnection (sourceFilterUID, sourceFilterChannel,
                                destFilterUID, destFilterChannel))
        changed();
}

void FilterGraph::clear()
{
    PluginWindow::closeAllCurrentlyOpenWindows();

    graph.clear();
    changed();
}

//==============================================================================
juce::String FilterGraph::getDocumentTitle()
{
    if (! getFile().exists())
        return "Unnamed";

    return getFile().getFileNameWithoutExtension();
}

void FilterGraph::newDocument()
{
    clear();
    setFile ({});

    InternalPluginFormat internalFormat;

    addFilter (internalFormat.audioInDesc,  { 0.5,  0.1 });
    addFilter (internalFormat.midiInDesc,   { 0.25, 0.1 });
    addFilter (internalFormat.audioOutDesc, { 0.5,  0.9 });

    setChangedFlag (false);
}

juce::Result FilterGraph::loadDocument (const juce::File& file)
{
    juce::XmlDocument doc (file);
    std::unique_ptr<juce::XmlElement> xml(doc.getDocumentElement());

    if (!xml || !xml->hasTagName ("FILTERGRAPH")) {
        return juce::Result::fail("Not a valid filter graph file");
    }

    restoreFromXml (*xml);
    return juce::Result::ok();
}

juce::Result FilterGraph::saveDocument (const juce::File& file)
{
    std::unique_ptr<juce::XmlElement> xml(createXml());

    if (!xml->writeToFile (file, {})) {
        return juce::Result::fail ("Couldn't write to the file");
    }

    return juce::Result::ok();
}

juce::File FilterGraph::getLastDocumentOpened()
{
    juce::RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                        ->getValue ("recentFilterGraphFiles"));

    return recentFiles.getFile (0);
}

void FilterGraph::setLastDocumentOpened (const juce::File& file)
{
    juce::RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                        ->getValue ("recentFilterGraphFiles"));

    recentFiles.addFile (file);

    getAppProperties().getUserSettings()
        ->setValue ("recentFilterGraphFiles", recentFiles.toString());
}

//==============================================================================
static void readBusLayoutFromXml (juce::AudioProcessor::BusesLayout& busesLayout, juce::AudioProcessor* plugin, const juce::XmlElement& xml, const bool isInput)
{
    juce::Array<juce::AudioChannelSet>& targetBuses = (isInput ? busesLayout.inputBuses : busesLayout.outputBuses);
    int maxNumBuses = 0;

    if (auto* buses = xml.getChildByName (isInput ? "INPUTS" : "OUTPUTS"))
    {
        forEachXmlChildElementWithTagName (*buses, e, "BUS")
        {
            const int busIdx = e->getIntAttribute ("index");
            maxNumBuses = juce::jmax (maxNumBuses, busIdx + 1);

            // the number of buses on busesLayout may not be in sync with the plugin after adding buses
            // because adding an input bus could also add an output bus
            for (int actualIdx = plugin->getBusCount (isInput) - 1; actualIdx < busIdx; ++actualIdx)
                if (! plugin->addBus (isInput)) return;

            for (int actualIdx = targetBuses.size() - 1; actualIdx < busIdx; ++actualIdx)
                targetBuses.add (plugin->getChannelLayoutOfBus (isInput, busIdx));

            const juce::String& layout = e->getStringAttribute("layout");

            if (layout.isNotEmpty())
                targetBuses.getReference (busIdx) = juce::AudioChannelSet::fromAbbreviatedString (layout);
        }
    }

    // if the plugin has more buses than specified in the xml, then try to remove them!
    while (maxNumBuses < targetBuses.size())
    {
        if (! plugin->removeBus (isInput))
            return;

        targetBuses.removeLast();
    }
}

//==============================================================================
static juce::XmlElement* createBusLayoutXml (const juce::AudioProcessor::BusesLayout& layout, const bool isInput)
{
    const juce::Array<juce::AudioChannelSet>& buses = (isInput ? layout.inputBuses : layout.outputBuses);

    auto* xml = new juce::XmlElement(isInput ? "INPUTS" : "OUTPUTS");

    const int n = buses.size();
    for (int busIdx = 0; busIdx < n; ++busIdx)
    {
        auto* bus = new juce::XmlElement("BUS");
        bus->setAttribute ("index", busIdx);

        const juce::AudioChannelSet& set = buses.getReference (busIdx);
        const juce::String layoutName = set.isDisabled() ? "disabled" : set.getSpeakerArrangementAsString();

        bus->setAttribute ("layout", layoutName);

        xml->addChildElement (bus);
    }

    return xml;
}

static juce::XmlElement* createNodeXml (juce::AudioProcessorGraph::Node* const node) noexcept
{
    juce::AudioPluginInstance* plugin = dynamic_cast<juce::AudioPluginInstance*> (node->getProcessor());

    if (plugin == nullptr)
    {
        jassertfalse;
        return nullptr;
    }

    auto* e = new juce::XmlElement("FILTER");
    e->setAttribute ("uid", (int) node->nodeId);
    e->setAttribute ("x", node->properties ["x"].toString());
    e->setAttribute ("y", node->properties ["y"].toString());

    for (int i = 0; i < PluginWindow::NumTypes; ++i)
    {
        PluginWindow::WindowFormatType type = (PluginWindow::WindowFormatType) i;

        if (node->properties.contains (getOpenProp (type)))
        {
            e->setAttribute (getLastXProp (type), node->properties[getLastXProp (type)].toString());
            e->setAttribute (getLastYProp (type), node->properties[getLastYProp (type)].toString());
            e->setAttribute (getOpenProp (type),  node->properties[getOpenProp (type)].toString());
        }
    }

    juce::PluginDescription pd;
    plugin->fillInPluginDescription (pd);

    e->addChildElement (pd.createXml());

    auto* state = new juce::XmlElement("STATE");

    juce::MemoryBlock m;
    node->getProcessor()->getStateInformation (m);
    state->addTextElement (m.toBase64Encoding());
    e->addChildElement (state);

    auto* layouts = new juce::XmlElement("LAYOUT");
    const juce::AudioProcessor::BusesLayout layout = plugin->getBusesLayout();

    const bool isInputChoices[] = { true, false };
    for (bool isInput : isInputChoices)
        layouts->addChildElement (createBusLayoutXml (layout, isInput));

    e->addChildElement (layouts);

    return e;
}

void FilterGraph::createNodeFromXml (const juce::XmlElement& xml)
{
    juce::PluginDescription pd;

    forEachXmlChildElement (xml, e)
    {
        if (pd.loadFromXml (*e))
            break;
    }

    juce::String errorMessage;

    juce::AudioPluginInstance* instance = formatManager.createPluginInstance (pd, graph.getSampleRate(), graph.getBlockSize(), errorMessage);

    if (instance == nullptr)
        return;

    if (const juce::XmlElement* const layoutEntity = xml.getChildByName ("LAYOUT"))
    {
        juce::AudioProcessor::BusesLayout layout = instance->getBusesLayout();

        const bool isInputChoices[] = { true, false };
        for (bool isInput : isInputChoices)
            readBusLayoutFromXml (layout, instance, *layoutEntity, isInput);

        instance->setBusesLayout (layout);
    }

    juce::AudioProcessorGraph::Node::Ptr node (graph.addNode (instance, static_cast<juce::uint32>(xml.getIntAttribute("uid"))));

    if (auto* const state = xml.getChildByName ("STATE"))
    {
        juce::MemoryBlock m;
        m.fromBase64Encoding (state->getAllSubText());

        node->getProcessor()->setStateInformation (m.getData(), static_cast<int>(m.getSize()));
    }

    node->properties.set ("x", xml.getDoubleAttribute ("x"));
    node->properties.set ("y", xml.getDoubleAttribute ("y"));

    for (int i = 0; i < PluginWindow::NumTypes; ++i)
    {
        PluginWindow::WindowFormatType type = (PluginWindow::WindowFormatType) i;

        if (xml.hasAttribute (getOpenProp (type)))
        {
            node->properties.set (getLastXProp (type), xml.getIntAttribute (getLastXProp (type)));
            node->properties.set (getLastYProp (type), xml.getIntAttribute (getLastYProp (type)));
            node->properties.set (getOpenProp (type), xml.getIntAttribute (getOpenProp (type)));

            if (node->properties[getOpenProp (type)])
            {
                jassert (node->getProcessor());

                if (PluginWindow* const w = PluginWindow::getWindowFor (node, type))
                    w->toFront (true);
            }
        }
    }
}

juce::XmlElement* FilterGraph::createXml() const
{
    auto* xml = new juce::XmlElement("FILTERGRAPH");

    for (int i = 0; i < graph.getNumNodes(); ++i)
        xml->addChildElement (createNodeXml (graph.getNode (i)));

    for (int i = 0; i < graph.getNumConnections(); ++i)
    {
        const juce::AudioProcessorGraph::Connection* const fc = graph.getConnection(i);

        auto* e = new juce::XmlElement("CONNECTION");

        e->setAttribute ("srcFilter", (int) fc->sourceNodeId);
        e->setAttribute ("srcChannel", fc->sourceChannelIndex);
        e->setAttribute ("dstFilter", (int) fc->destNodeId);
        e->setAttribute ("dstChannel", fc->destChannelIndex);

        xml->addChildElement (e);
    }

    return xml;
}

void FilterGraph::restoreFromXml (const juce::XmlElement& xml)
{
    clear();

    forEachXmlChildElementWithTagName (xml, e, "FILTER")
    {
        createNodeFromXml (*e);
        changed();
    }

    forEachXmlChildElementWithTagName (xml, e, "CONNECTION")
    {
        addConnection (static_cast<juce::uint32>(e->getIntAttribute("srcFilter")),
                       e->getIntAttribute("srcChannel"),
                       static_cast<juce::uint32>(e->getIntAttribute("dstFilter")),
                       e->getIntAttribute("dstChannel"));
    }

    graph.removeIllegalConnections();
}

}}} // ompu
