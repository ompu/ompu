#pragma once

#include "ompu/plug/host_fwd.hpp"
#include "ompu/plug/host/filter_graph.hpp"


namespace ompu { namespace plug { namespace host {

class InternalPluginFormat
    : public juce::AudioPluginFormat
{
public:
    //==============================================================================
    InternalPluginFormat();
    ~InternalPluginFormat() {}

    //==============================================================================
    juce::PluginDescription audioInDesc, audioOutDesc, midiInDesc;

    void getAllTypes (juce::OwnedArray<juce::PluginDescription>&);

    //==============================================================================
    juce::String getName() const override                                                     { return "Internal"; }
    bool fileMightContainThisPluginType (const juce::String&) override                        { return true; }
    juce::FileSearchPath getDefaultLocationsToSearch() override                               { return {}; }
    bool canScanForPlugins() const override                                             { return false; }
    void findAllTypesForFile (juce::OwnedArray<juce::PluginDescription>&, const juce::String&) override  {}
    bool doesPluginStillExist (const juce::PluginDescription&) override                       { return true; }
    juce::String getNameOfPluginFromIdentifier (const juce::String& fileOrIdentifier) override      { return fileOrIdentifier; }
    bool pluginNeedsRescanning (const juce::PluginDescription&) override                      { return false; }
    juce::StringArray searchPathsForPlugins (const juce::FileSearchPath&, bool, bool) override      { return {}; }

private:
    //==============================================================================
    void createPluginInstance (const juce::PluginDescription&, double initialSampleRate, int initialBufferSize,
                               void* userData, void (*callback) (void*, juce::AudioPluginInstance*, const juce::String&)) override;

    bool requiresUnblockedMessageThreadDuringCreation (const juce::PluginDescription&) const noexcept override;
};

}}} // ompu
