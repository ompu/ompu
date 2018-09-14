#pragma once

#include "ompu/plug/host_fwd.hpp"

namespace ompu { namespace plug { namespace host {

const char* const filenameSuffix = ".filtergraph";
const char* const filenameWildcard = "*.filtergraph";


class FilterGraph : public juce::FileBasedDocument, public juce::AudioProcessorListener
{
public:
    explicit FilterGraph(juce::AudioPluginFormatManager& formatManager);
    ~FilterGraph();

    juce::AudioProcessorGraph& getGraph() noexcept         { return graph; }

    int getNumFilters() const noexcept;
    juce::AudioProcessorGraph::Node::Ptr getNode (int index) const noexcept;

    juce::AudioProcessorGraph::Node::Ptr getNodeForId (juce::uint32 uid) const;
    juce::AudioProcessorGraph::Node::Ptr getNodeForName (const juce::String& name) const;

    void addFilter (const juce::PluginDescription&, juce::Point<double>);

    void addFilterCallback (juce::AudioPluginInstance*, const juce::String& error, juce::Point<double> pos);

    void removeFilter (const juce::uint32 filterUID);
    void disconnectFilter (const juce::uint32 filterUID);

    void removeIllegalConnections();

    void setNodePosition (juce::uint32 nodeId, double x, double y);
    juce::Point<double> getNodePosition (juce::uint32 nodeId) const;

    //==============================================================================
    int getNumConnections() const noexcept;
    const juce::AudioProcessorGraph::Connection* getConnection (const int index) const noexcept;

    const juce::AudioProcessorGraph::Connection* getConnectionBetween (juce::uint32 sourceFilterUID, int sourceFilterChannel,
        juce::uint32 destFilterUID, int destFilterChannel) const noexcept;

    bool canConnect (juce::uint32 sourceFilterUID, int sourceFilterChannel,
        juce::uint32 destFilterUID, int destFilterChannel) const noexcept;

    bool addConnection (juce::uint32 sourceFilterUID, int sourceFilterChannel,
        juce::uint32 destFilterUID, int destFilterChannel);

    void removeConnection (const int index);

    void removeConnection (juce::uint32 sourceFilterUID, int sourceFilterChannel,
        juce::uint32 destFilterUID, int destFilterChannel);

    void clear();


    //==============================================================================
    void audioProcessorParameterChanged (juce::AudioProcessor*, int, float) override {}
    void audioProcessorChanged (juce::AudioProcessor*) override { changed(); }

    //==============================================================================
    juce::XmlElement* createXml() const;
    void restoreFromXml (const juce::XmlElement& xml);

    //==============================================================================
    void newDocument();
    juce::String getDocumentTitle() override;
    juce::Result loadDocument (const juce::File& file) override;
    juce::Result saveDocument (const juce::File& file) override;
    juce::File getLastDocumentOpened() override;
    void setLastDocumentOpened (const juce::File& file) override;

    //==============================================================================


    /** The special channel index used to refer to a filter's midi channel.
    */
    static const int midiChannelNumber;

private:
    //==============================================================================
    juce::AudioPluginFormatManager& formatManager;
    juce::AudioProcessorGraph graph;

    juce::uint32 lastUID = 0;
    juce::uint32 getNextUID() noexcept;

    void createNodeFromXml (const juce::XmlElement& xml);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGraph)
};

}}} // ompu
