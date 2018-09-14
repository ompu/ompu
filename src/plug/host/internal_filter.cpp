#include "ompu/plug/host/internal_filter.hpp"
#include "ompu/plug/host/filter_graph.hpp"


namespace ompu { namespace plug { namespace host {

InternalPluginFormat::InternalPluginFormat()
{
    {
        juce::AudioProcessorGraph::AudioGraphIOProcessor p (juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
        p.fillInPluginDescription (audioOutDesc);
    }

    {
        juce::AudioProcessorGraph::AudioGraphIOProcessor p (juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
        p.fillInPluginDescription (audioInDesc);
    }

    {
        juce::AudioProcessorGraph::AudioGraphIOProcessor p (juce::AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);
        p.fillInPluginDescription (midiInDesc);
    }
}

void InternalPluginFormat::createPluginInstance (const juce::PluginDescription& desc,
                                                 double /*initialSampleRate*/,
                                                 int /*initialBufferSize*/,
                                                 void* userData,
                                                 void (*callback) (void*, juce::AudioPluginInstance*, const juce::String&))
{
    juce::AudioPluginInstance* p = nullptr;

    if (desc.name == audioOutDesc.name) p = new juce::AudioProcessorGraph::AudioGraphIOProcessor (juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    if (desc.name == audioInDesc.name)  p = new juce::AudioProcessorGraph::AudioGraphIOProcessor (juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
    if (desc.name == midiInDesc.name)   p = new juce::AudioProcessorGraph::AudioGraphIOProcessor (juce::AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);

    callback (userData, p, p == nullptr ? NEEDS_TRANS ("Invalid internal filter name") : juce::String());
}

bool InternalPluginFormat::requiresUnblockedMessageThreadDuringCreation (const juce::PluginDescription&) const noexcept
{
    return false;
}

void InternalPluginFormat::getAllTypes (juce::OwnedArray<juce::PluginDescription>& results)
{
    results.add (new juce::PluginDescription (audioInDesc));
    results.add (new juce::PluginDescription (audioOutDesc));
    results.add (new juce::PluginDescription (midiInDesc));
}

}}} // ompu
