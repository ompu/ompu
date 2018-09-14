#pragma once

#include "JuceHeader.h"

#include "ompu/plug/host_fwd.hpp"
#include "ompu/plug/host/filter_graph.hpp"
#include "ompu/plug/host/graph_editor_panel.hpp"

#include "ompu/plug/audio_callback.hpp"
#include "ompu/plug/device_manager.hpp"

#include "config_window.h"
#include "mixer.h"
#include "game_window.h"

#include <boost/variant/variant_fwd.hpp>

#include <memory>
#include <vector>
#include <utility>


namespace ompu { namespace plug { namespace host {

namespace gui_scenes {

struct config {};
struct mixer {};
struct game {};

class Visitor;

} // gui_scenes

namespace CommandIDs
{
    static const int open                   = 0x30000;
    static const int save                   = 0x30001;
    static const int saveAs                 = 0x30002;
    static const int newFile                = 0x30003;
    static const int showPluginListEditor   = 0x30100;
    static const int showAudioSettings      = 0x30200;
    static const int aboutBox               = 0x30300;
    static const int allWindowsForward      = 0x30400;
    static const int toggleDoublePrecision  = 0x30500;
}

class HostWindow
    : public juce::DocumentWindow
    , public juce::MenuBarModel
    , public juce::ApplicationCommandTarget
    , public juce::ChangeListener
    , public juce::FileDragAndDropTarget
    , public juce::KeyListener
{
    friend class gui_scenes::Visitor;
    constexpr static unsigned const WIDTH = 1600, HEIGHT = 1200;
    constexpr static char const* const CFG_XML_NAME = "ompu_cfg.xml";
    static juce::File const OMPU_ROOT, LOG_ROOT, LOG_FILE, CFG_ROOT, CFG_XML_FILE;

    static std::string const LOW_LATENCY_DEV_TYPE;

public:
    explicit HostWindow(ompu::plug::App& app_, saya::logger_env_set envs);
    ~HostWindow();

    void closeButtonPressed() override;
    void changeListenerCallback(juce::ChangeBroadcaster*) override;

    bool isInterestedInFileDrag (const juce::StringArray& files);
    void fileDragEnter (const juce::StringArray& files, int, int);
    void fileDragMove (const juce::StringArray& files, int, int);
    void fileDragExit (const juce::StringArray& files);
    void filesDropped (const juce::StringArray& files, int, int);

    void menuBarActivated (bool isActive);

    juce::StringArray getMenuBarNames();
    juce::PopupMenu getMenuForIndex (int topLevelMenuIndex, const juce::String& menuName);
    void menuItemSelected (int menuItemID, int topLevelMenuIndex);
    juce::ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (juce::Array<juce::CommandID>& commands);
    void getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);

    void tryToQuitApplication();

    void createPlugin (const juce::PluginDescription&, juce::Point<int> pos);

    void addPluginsToMenu (juce::PopupMenu&) const;
    const juce::PluginDescription* getChosenType (int menuID) const;

    GraphDocumentComponent* getGraphEditor() const;

    bool isDoublePrecisionProcessing();
    void updatePrecisionMenuItem (juce::ApplicationCommandInfo& info);

    void resized() override;

    virtual bool keyPressed(juce::KeyPress const& key, juce::Component* src) override;
    virtual bool keyStateChanged(bool is_down, juce::Component* src) override;

private:
    ompu::plug::App& app() noexcept { return app_; }
    ompu::plug::App const& app() const noexcept { return app_; }

    saya::logger& l() noexcept { return l_; }

    // app
    ompu::plug::App& app_;

    // logger
    saya::logger l_;

    // callbacks, listeners
    std::unique_ptr<AudioCallback> audio_cb_;

    struct GUI
    {
        ompu::plug::base::AppComponent<> holder;
        gui::ConfigWindow config;
        gui::Mixer mixer;

        explicit GUI(App& app)
            : holder(app)
            , config(app)
            , mixer(app)
        {}
    };

    std::unique_ptr<GUI> gui_;

    std::unique_ptr<GraphDocumentComponent> graph_document_;

    std::unique_ptr<gui::GameWindow> game_window_;

    juce::AudioPluginFormatManager formatManager;

    juce::OwnedArray<juce::PluginDescription> internalTypes;
    juce::KnownPluginList knownPluginList;
    juce::KnownPluginList::SortMethod pluginSortMethod;

    class PluginListWindow;
    std::unique_ptr<PluginListWindow> pluginListWindow;


    boost::variant<
        gui_scenes::config,
        gui_scenes::mixer,
        gui_scenes::game
    > gui_scene_;

    using resolution_set_type = std::vector<std::pair<int, int>>;
    static resolution_set_type const ResolutionSet;

    void showAudioSettings();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HostWindow)
};

}}} // ompu
