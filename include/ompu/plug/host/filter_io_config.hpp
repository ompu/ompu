#pragma once

#include "ompu/plug/host_fwd.hpp"
#include "ompu/plug/host/filter_graph.hpp"

#include <memory>


namespace ompu { namespace plug { namespace host {

class FilterIOConfigurationWindow : public juce::AudioProcessorEditor
{
public:
    class InputOutputConfig;

    explicit FilterIOConfigurationWindow(juce::AudioProcessor* const p);
    ~FilterIOConfigurationWindow();

    void paint (juce::Graphics& g) override;
    void resized() override;

    InputOutputConfig* getConfig (bool isInput) noexcept { return isInput ? inConfig.get() : outConfig.get(); }
    void update();

private:
    HostWindow* getMainWindow() const;
    GraphDocumentComponent* getGraphEditor() const;
    juce::AudioProcessorGraph* getGraph() const;
    juce::int32 getNodeId() const;

    friend class InputOutputConfig;

    juce::AudioProcessor::BusesLayout currentLayout;
    juce::Label title;
    std::unique_ptr<InputOutputConfig> inConfig, outConfig;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterIOConfigurationWindow)
};

}}} // ompu
