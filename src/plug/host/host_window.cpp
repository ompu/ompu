#include "ompu/plug/host/host_window.hpp"
#include "ompu/plug/host/host_app.hpp"
#include "ompu/plug/host/internal_filter.hpp"

#include "ompu/plug/app.hpp"
#include "ompu/plug/util.hpp"

#include "ompu/util.hpp"

#include "config_window.h"

#include "JuceHeader.h"

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/variant.hpp>
#include <boost/core/ignore_unused.hpp>

#include <iostream>


namespace ompu { namespace plug { namespace host {

namespace gui_scenes {

class Visitor : public boost::static_visitor<void>
{
public:
    explicit Visitor(HostWindow* const self)
        : self_(self)
    {}

    void operator()(gui_scenes::config) const
    {
        {
            auto& c = self_->gui_->config;
            c.setVisible(!c.isVisible());
        }

        self_->gui_->mixer.setVisible(false);
    }

    void operator()(gui_scenes::game) const
    {
        self_->gui_->config.setVisible(false);
        self_->gui_->mixer.setVisible(false);
    }

    void operator()(gui_scenes::mixer) const
    {
        self_->gui_->config.setVisible(false);

        {
            auto& c = self_->gui_->mixer;
            c.setVisible(!c.isVisible());
        }
    }

private:
    HostWindow* const self_;
};

} // gui_scenes


class HostWindow::PluginListWindow  : public juce::DocumentWindow
{
public:
    PluginListWindow(HostWindow& owner_, juce::AudioPluginFormatManager& pluginFormatManager)
        : juce::DocumentWindow(
            "Available Plugins",
            juce::LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
            juce::DocumentWindow::minimiseButton | juce::DocumentWindow::closeButton
        )
        , owner(owner_)
    {
        juce::File const deadMansPedalFile(
            getAppProperties().getUserSettings()->getFile().getSiblingFile("RecentlyCrashedPluginsList")
        );

        content_ = std::make_unique<juce::PluginListComponent>(
            pluginFormatManager,
            owner.knownPluginList,
            deadMansPedalFile,
            getAppProperties().getUserSettings(),
            true
        );
        this->setContentNonOwned(content_.get(), true);

        this->setResizable(true, false);
        this->setResizeLimits(300, 400, 800, 1500);
        this->setTopLeftPosition(60, 60);

        this->restoreWindowStateFromString(getAppProperties().getUserSettings()->getValue("listWindowPos"));
        this->setVisible(true);
    }

    ~PluginListWindow()
    {
        getAppProperties().getUserSettings()->setValue("listWindowPos", getWindowStateAsString());
    }

    void closeButtonPressed()
    {
        owner.pluginListWindow = nullptr;
    }

private:
    HostWindow& owner;
    std::unique_ptr<juce::PluginListComponent> content_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginListWindow)
};

// -------------------------------------------

HostWindow::resolution_set_type const HostWindow::ResolutionSet = {
    {1600, 1200},
    {1600, 1000},
    {1600, 900},
    {1280, 720},
    {1200, 900},
    {800, 600},
};

HostWindow::HostWindow(ompu::plug::App& app_, saya::logger_env_set envs)
    : DocumentWindow(
        juce::JUCEApplication::getInstance()->getApplicationName(),
        juce::LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
        juce::DocumentWindow::allButtons
    )
    , app_(app_)
    , l_(std::move(envs), "HostWindow")
    , gui_scene_(gui_scenes::config{})
{
    auto& dm = app().dm();

    {
        l().info() << "setting audio callback..." << std::endl;
        audio_cb_ = std::make_unique<ompu::plug::AudioCallback>(app());
        dm.addAudioCallback(audio_cb_.get());
    }

    {
        l().info() << "fetching available devices" << std::endl;
        dm.createAudioDeviceTypes(app().dtypes);

        auto const dt_low_latency = boost::find_if(make_range(app().dtypes), [] (auto const& dt) {
            return dt->getTypeName() == App::LOW_LATENCY_DEV_TYPE;
        });

        if (dt_low_latency == std::end(make_range(app().dtypes))) {
            throw std::invalid_argument("low latency device not found on this environment");
        }

        auto* dt = *dt_low_latency;

        l().info() << "selecting low latency device type '" << dt->getTypeName() << "'" << std::endl;
        app().dtype = dt;
        app().dm().setCurrentAudioDeviceType(dt->getTypeName(), true);
    }

    //this->save_states();

    // ----------------------------------------------

    formatManager.addDefaultFormats();
    formatManager.addFormat (new InternalPluginFormat());

    std::unique_ptr<juce::XmlElement>
    savedAudioState(getAppProperties().getUserSettings()->getXmlValue("audioDeviceState"));

    //app().dm().initialise (2, 2, savedAudioState, true);

    this->centreWithSize(WIDTH, HEIGHT);
    this->setResizable(false, false);
    //this->setResizeLimits(500, 400, 10000, 10000);

    // setContentOwned (new GraphDocumentComponent (formatManager, app().dm()), false);
    graph_document_ = std::make_unique<GraphDocumentComponent>(formatManager, app().dm());

    // -----------------------------------------------------------------

    {
        l().info() << "setting up GUI..." << std::endl;
        gui_ = std::make_unique<GUI>(app());

        l().info() << "setting GUI font..." << std::endl;
        auto& lf = juce::LookAndFeel::getDefaultLookAndFeel();

#if BOOST_MSVC
        lf.setDefaultSansSerifTypefaceName("Meiryo UI"); // this should be OK on modern Windows
#else
#error sans serif font not specified for this platform (please set multibyte-aware font!)
#endif

        gui_->holder.setSize(WIDTH, HEIGHT);
        gui_->holder.addAndMakeVisible(gui_->config);
        gui_->holder.addChildComponent(gui_->mixer);

        gui_->mixer.setVisible(false);
        gui_->mixer.toFront(false);

        l().info() << "setting GUI callbacks..." << std::endl;
        this->addKeyListener(this);
    }


    l().info() << "application initialized" << std::endl;

    l().info() << "creating GameWindow..." << std::endl;
    game_window_ = std::make_unique<gui::GameWindow>(app());

    gui_->holder.addAndMakeVisible(game_window_.get());
    game_window_->toBack();
    l().info() << "GameWindow created" << std::endl;

    this->setContentNonOwned(&gui_->holder, false);

    // -------------------------------------
    // window resolution
    // -------------------------------------

    this->restoreWindowStateFromString(getAppProperties().getUserSettings()->getValue("window_state"));

    // detect unknown resolution
    if (!boost::algorithm::any_of_equal(ResolutionSet, std::make_pair(this->getWidth(), this->getHeight()))) {
        l().note() << "original resolution: (" << this->getWidth() << ", " << this->getHeight() << ")" << std::endl;
        l().warn() << "unknown resolution detected, falling back to default" << std::endl;
        this->setSize(ResolutionSet.front().first, ResolutionSet.front().second);
    }

    // -------------------------------------

    this->setVisible(true);

    InternalPluginFormat internalFormat;
    internalFormat.getAllTypes (internalTypes);

    std::unique_ptr<juce::XmlElement>
    savedPluginList(getAppProperties().getUserSettings()->getXmlValue("pluginList"));

    if (savedPluginList) {
        knownPluginList.recreateFromXml(*savedPluginList);
    }

    pluginSortMethod = static_cast<juce::KnownPluginList::SortMethod>(
        getAppProperties().getUserSettings()->getIntValue(
            "pluginSortMethod",
            juce::KnownPluginList::sortByManufacturer
        )
    );

    knownPluginList.addChangeListener(this);

    if (auto* filterGraph = getGraphEditor()->graph.get()) {
        filterGraph->addChangeListener(this);
    }

    this->addKeyListener(getCommandManager().getKeyMappings());

    juce::Process::setPriority(juce::Process::HighPriority);

#if JUCE_MAC
    setMacMainMenu(this);
#else
    setMenuBar(this);
#endif

    getCommandManager().setFirstCommandTarget(this);

    //this->resized();
}

HostWindow::~HostWindow()
{
    auto& dm = app().dm();
    dm.removeAudioCallback(audio_cb_.get());

    pluginListWindow = nullptr;
    knownPluginList.removeChangeListener (this);

    if (auto* filterGraph = getGraphEditor()->graph.get())
        filterGraph->removeChangeListener (this);

    getAppProperties().getUserSettings()->setValue ("window_state", getWindowStateAsString());
    clearContentComponent();
    graph_document_.reset();

   #if JUCE_MAC
    setMacMainMenu (nullptr);
   #else
    setMenuBar (nullptr);
   #endif
}

void HostWindow::resized()
{
    if (game_window_) {
        game_window_->setSize(this->getWidth(), this->getHeight());
    }

    if (gui_) {
        gui_->config.setSize(this->getWidth(), this->getHeight());
        gui_->mixer.setSize(this->getWidth(), static_cast<int>(this->getHeight() * .3f));
    }
}

bool HostWindow::keyPressed(juce::KeyPress const& key, juce::Component* const src)
{
    boost::ignore_unused(src);

    if (key.isKeyCurrentlyDown(juce::KeyPress::F2Key)) {
        gui_scene_ = gui_scenes::game{};

    } else if (key.isKeyCurrentlyDown(juce::KeyPress::F3Key)) {
        gui_scene_ = gui_scenes::mixer{};

    } else if (key.isKeyCurrentlyDown(juce::KeyPress::F6Key)) {
        gui_scene_ = gui_scenes::config{};

    } else {
        return true;
    }

    boost::apply_visitor(gui_scenes::Visitor{this}, gui_scene_);
    return true;
}

bool HostWindow::keyStateChanged(bool const is_down, juce::Component* const src)
{
    boost::ignore_unused(is_down, src);
    return false;
}

void HostWindow::closeButtonPressed()
{
    tryToQuitApplication();
}

struct AsyncQuitRetrier
    : juce::Timer
{
    AsyncQuitRetrier()   { startTimer (500); }

    void timerCallback() override
    {
        stopTimer();
        delete this;

        if (auto app = juce::JUCEApplicationBase::getInstance()) {
            app->systemRequestedQuit();
        }
    }
};

void HostWindow::tryToQuitApplication()
{
    PluginWindow::closeAllCurrentlyOpenWindows();

    if (juce::ModalComponentManager::getInstance()->cancelAllModalComponents())
    {
        new AsyncQuitRetrier();
    }
    else if (getGraphEditor() == nullptr
              || getGraphEditor()->graph->saveIfNeededAndUserAgrees() == juce::FileBasedDocument::savedOk)
    {
        // Some plug-ins do not want [NSApp stop] to be called
        // before the plug-ins are not deallocated.
        getGraphEditor()->releaseGraph();

        juce::JUCEApplication::quit();
    }
}

void HostWindow::changeListenerCallback (juce::ChangeBroadcaster* changed)
{
    if (changed == &knownPluginList)
    {
        menuItemsChanged();

        // save the plugin list every time it gets chnaged, so that if we're scanning
        // and it crashes, we've still saved the previous ones
        std::unique_ptr<juce::XmlElement> savedPluginList (knownPluginList.createXml());

        if (savedPluginList) {
            getAppProperties().getUserSettings()->setValue ("pluginList", savedPluginList.get());
            getAppProperties().saveIfNeeded();
        }
    }
    else if (changed == getGraphEditor()->graph.get())
    {
        auto title = juce::JUCEApplication::getInstance()->getApplicationName();

        juce::File f{getGraphEditor()->graph->getFile()};

        if (f.existsAsFile()) {
            title = f.getFileName() + " - " + title;
        }

        setName (title);
    }
}

juce::StringArray HostWindow::getMenuBarNames()
{
    juce::StringArray names;
    names.add ("File");
    names.add ("Plugins");
    names.add ("Options");
    names.add ("Windows");
    return names;
}

juce::PopupMenu HostWindow::getMenuForIndex (int topLevelMenuIndex, const juce::String& /*menuName*/)
{
    juce::PopupMenu menu;

    if (topLevelMenuIndex == 0)
    {
        // "File" menu
        menu.addCommandItem(&getCommandManager(), CommandIDs::newFile);
        menu.addCommandItem(&getCommandManager(), CommandIDs::open);

        juce::RecentlyOpenedFilesList recentFiles;
        recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                            ->getValue ("recentFilterGraphFiles"));

        juce::PopupMenu recentFilesMenu;
        recentFiles.createPopupMenuItems (recentFilesMenu, 100, true, true);
        menu.addSubMenu ("Open recent file", recentFilesMenu);

        menu.addCommandItem(&getCommandManager(), CommandIDs::save);
        menu.addCommandItem(&getCommandManager(), CommandIDs::saveAs);
        menu.addSeparator();
        menu.addCommandItem(&getCommandManager(), juce::StandardApplicationCommandIDs::quit);
    }
    else if (topLevelMenuIndex == 1)
    {
        // "Plugins" menu
        juce::PopupMenu pluginsMenu;
        addPluginsToMenu (pluginsMenu);
        menu.addSubMenu ("Create plugin", pluginsMenu);
        menu.addSeparator();
        menu.addItem(250, "Delete all plugins");
    }
    else if (topLevelMenuIndex == 2)
    {
        // "Options" menu

        menu.addCommandItem(&getCommandManager(), CommandIDs::showPluginListEditor);

        juce::PopupMenu sortTypeMenu;
        sortTypeMenu.addItem(200, "List plugins in default order",      true, pluginSortMethod == juce::KnownPluginList::defaultOrder);
        sortTypeMenu.addItem(201, "List plugins in alphabetical order", true, pluginSortMethod == juce::KnownPluginList::sortAlphabetically);
        sortTypeMenu.addItem(202, "List plugins by category",           true, pluginSortMethod == juce::KnownPluginList::sortByCategory);
        sortTypeMenu.addItem(203, "List plugins by manufacturer",       true, pluginSortMethod == juce::KnownPluginList::sortByManufacturer);
        sortTypeMenu.addItem(204, "List plugins based on the directory structure", true, pluginSortMethod == juce::KnownPluginList::sortByFileSystemLocation);
        menu.addSubMenu ("Plugin menu type", sortTypeMenu);

        menu.addSeparator();
        menu.addCommandItem(&getCommandManager(), CommandIDs::showAudioSettings);
        menu.addCommandItem(&getCommandManager(), CommandIDs::toggleDoublePrecision);

        menu.addSeparator();
        menu.addCommandItem(&getCommandManager(), CommandIDs::aboutBox);
    }
    else if (topLevelMenuIndex == 3)
    {
        menu.addCommandItem(&getCommandManager(), CommandIDs::allWindowsForward);
    }

    return menu;
}

void HostWindow::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
    if (menuItemID == 250)
    {
        if (auto* graphEditor = getGraphEditor())
            if (auto* filterGraph = graphEditor->graph.get())
                filterGraph->clear();
    }
    else if (menuItemID >= 100 && menuItemID < 200)
    {
        juce::RecentlyOpenedFilesList recentFiles;
        recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                            ->getValue ("recentFilterGraphFiles"));

        if (auto* graphEditor = getGraphEditor())
            if (graphEditor->graph != nullptr && graphEditor->graph->saveIfNeededAndUserAgrees() == juce::FileBasedDocument::savedOk)
                graphEditor->graph->loadFrom (recentFiles.getFile (menuItemID - 100), true);
    }
    else if (menuItemID >= 200 && menuItemID < 210)
    {
             if (menuItemID == 200)     pluginSortMethod = juce::KnownPluginList::defaultOrder;
        else if (menuItemID == 201)     pluginSortMethod = juce::KnownPluginList::sortAlphabetically;
        else if (menuItemID == 202)     pluginSortMethod = juce::KnownPluginList::sortByCategory;
        else if (menuItemID == 203)     pluginSortMethod = juce::KnownPluginList::sortByManufacturer;
        else if (menuItemID == 204)     pluginSortMethod = juce::KnownPluginList::sortByFileSystemLocation;

        getAppProperties().getUserSettings()->setValue ("pluginSortMethod", (int) pluginSortMethod);

        menuItemsChanged();
    }
    else
    {
        if (auto* desc = getChosenType (menuItemID))
            createPlugin (*desc,
                          { proportionOfWidth  (0.3f + juce::Random::getSystemRandom().nextFloat() * 0.6f),
                            proportionOfHeight (0.3f + juce::Random::getSystemRandom().nextFloat() * 0.6f) });
    }
}

void HostWindow::menuBarActivated (bool isActivated)
{
    if (auto* graphEditor = getGraphEditor())
        if (isActivated)
            graphEditor->unfocusKeyboardComponent();
}

void HostWindow::createPlugin (const juce::PluginDescription& desc, juce::Point<int> pos)
{
    if (auto* graphEditor = getGraphEditor())
        graphEditor->createNewPlugin (desc, pos);
}

void HostWindow::addPluginsToMenu (juce::PopupMenu& m) const
{
    if (auto* graphEditor = getGraphEditor())
    {
        int i = 0;

        for (auto* t : internalTypes)
            m.addItem(++i, t->name + " (" + t->pluginFormatName + ")",
                       graphEditor->graph->getNodeForName (t->name) == nullptr);
    }

    m.addSeparator();

    knownPluginList.addToMenu (m, pluginSortMethod);
}

const juce::PluginDescription* HostWindow::getChosenType (const int menuID) const
{
    if (menuID >= 1 && menuID < 1 + internalTypes.size())
        return internalTypes [menuID - 1];

    return knownPluginList.getType (knownPluginList.getIndexChosenByMenu (menuID));
}

//==============================================================================
juce::ApplicationCommandTarget* HostWindow::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void HostWindow::getAllCommands (juce::Array<juce::CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const juce::CommandID ids[] = { CommandIDs::newFile,
                              CommandIDs::open,
                              CommandIDs::save,
                              CommandIDs::saveAs,
                              CommandIDs::showPluginListEditor,
                              CommandIDs::showAudioSettings,
                              CommandIDs::toggleDoublePrecision,
                              CommandIDs::aboutBox,
                              CommandIDs::allWindowsForward
                            };

    commands.addArray (ids, juce::numElementsInArray (ids));
}

void HostWindow::getCommandInfo (const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    const juce::String category ("General");

    switch (commandID)
    {
    case CommandIDs::newFile:
        result.setInfo ("New", "Creates a new filter graph file", category, 0);
        result.defaultKeypresses.add(juce::KeyPress('n', juce::ModifierKeys::commandModifier, 0));
        break;

    case CommandIDs::open:
        result.setInfo ("Open...", "Opens a filter graph file", category, 0);
        result.defaultKeypresses.add (juce::KeyPress ('o', juce::ModifierKeys::commandModifier, 0));
        break;

    case CommandIDs::save:
        result.setInfo ("Save", "Saves the current graph to a file", category, 0);
        result.defaultKeypresses.add (juce::KeyPress ('s', juce::ModifierKeys::commandModifier, 0));
        break;

    case CommandIDs::saveAs:
        result.setInfo ("Save As...",
                        "Saves a copy of the current graph to a file",
                        category, 0);
        result.defaultKeypresses.add (juce::KeyPress ('s', juce::ModifierKeys::shiftModifier | juce::ModifierKeys::commandModifier, 0));
        break;

    case CommandIDs::showPluginListEditor:
        result.setInfo ("Edit the list of available plug-Ins...", {}, category, 0);
        result.addDefaultKeypress ('p', juce::ModifierKeys::commandModifier);
        break;

    case CommandIDs::showAudioSettings:
        result.setInfo ("Change the audio device settings", {}, category, 0);
        result.addDefaultKeypress ('a', juce::ModifierKeys::commandModifier);
        break;

    case CommandIDs::toggleDoublePrecision:
        updatePrecisionMenuItem(result);
        break;

    case CommandIDs::aboutBox:
        result.setInfo ("About...", {}, category, 0);
        break;

    case CommandIDs::allWindowsForward:
        result.setInfo ("All Windows Forward", "Bring all plug-in windows forward", category, 0);
        result.addDefaultKeypress ('w', juce::ModifierKeys::commandModifier);
        break;

    default:
        break;
    }
}

bool HostWindow::perform (const InvocationInfo& info)
{
    auto* graphEditor = getGraphEditor();

    switch (info.commandID)
    {
    case CommandIDs::newFile:
        if (graphEditor != nullptr && graphEditor->graph != nullptr && graphEditor->graph->saveIfNeededAndUserAgrees() == juce::FileBasedDocument::savedOk)
            graphEditor->graph->newDocument();
        break;

    case CommandIDs::open:
        if (graphEditor != nullptr && graphEditor->graph != nullptr && graphEditor->graph->saveIfNeededAndUserAgrees() == juce::FileBasedDocument::savedOk)
            graphEditor->graph->loadFromUserSpecifiedFile (true);
        break;

    case CommandIDs::save:
        if (graphEditor != nullptr && graphEditor->graph != nullptr)
            graphEditor->graph->save(true, true);
        break;

    case CommandIDs::saveAs:
        if (graphEditor && graphEditor->graph) {
            graphEditor->graph->saveAs (juce::File{}, true, true, true);
        }
        break;

    case CommandIDs::showPluginListEditor:
        if (!pluginListWindow) {
            pluginListWindow = std::make_unique<PluginListWindow>(*this, formatManager);
        }

        pluginListWindow->toFront(true);
        break;

    case CommandIDs::showAudioSettings:
        showAudioSettings();
        break;

    case CommandIDs::toggleDoublePrecision:
        if (auto* props = getAppProperties().getUserSettings())
        {
            bool newIsDoublePrecision = ! isDoublePrecisionProcessing();
            props->setValue ("doublePrecisionProcessing", juce::var (newIsDoublePrecision));

            {
                juce::ApplicationCommandInfo cmdInfo (info.commandID);
                updatePrecisionMenuItem(cmdInfo);
                menuItemsChanged();
            }

            if (graphEditor != nullptr)
                graphEditor->setDoublePrecision (newIsDoublePrecision);
        }
        break;

    case CommandIDs::aboutBox:
        // TODO
        break;

    case CommandIDs::allWindowsForward:
    {
        auto& desktop = juce::Desktop::getInstance();

        for (int i = 0; i < desktop.getNumComponents(); ++i)
            desktop.getComponent (i)->toBehind (this);

        break;
    }

    default:
        return false;
    }

    return true;
}

void HostWindow::showAudioSettings()
{
    juce::AudioDeviceSelectorComponent audioSettingsComp(app().dm(),
                                                    0, 256,
                                                    0, 256,
                                                    true, true, true, false);

    audioSettingsComp.setSize (500, 450);

    juce::DialogWindow::LaunchOptions o;
    o.content.setNonOwned (&audioSettingsComp);
    o.dialogTitle                   = "Audio Settings";
    o.componentToCentreAround       = this;
    o.dialogBackgroundColour        = getLookAndFeel().findColour (ResizableWindow::backgroundColourId);
    o.escapeKeyTriggersCloseButton  = true;
    o.useNativeTitleBar             = false;
    o.resizable                     = false;

    o.runModal();

    std::unique_ptr<juce::XmlElement> audioState(app().dm().createStateXml());

    getAppProperties().getUserSettings()->setValue ("audioDeviceState", audioState.get());
    getAppProperties().getUserSettings()->saveIfNeeded();

    if (auto* graphEditor = getGraphEditor()) {
        if (graphEditor->graph) {
            graphEditor->graph->removeIllegalConnections();
        }
    }
}

bool HostWindow::isInterestedInFileDrag (const juce::StringArray&)
{
    return true;
}

void HostWindow::fileDragEnter (const juce::StringArray&, int, int)
{
}

void HostWindow::fileDragMove (const juce::StringArray&, int, int)
{
}

void HostWindow::fileDragExit (const juce::StringArray&)
{
}

void HostWindow::filesDropped (const juce::StringArray& files, int x, int y)
{
    if (auto* graphEditor = getGraphEditor())
    {
        if (files.size() == 1 && juce::File{files[0]}.hasFileExtension(filenameSuffix))
        {
            if (auto* filterGraph = graphEditor->graph.get()) {
                if (filterGraph->saveIfNeededAndUserAgrees() == juce::FileBasedDocument::savedOk) {
                    filterGraph->loadFrom (juce::File{files[0]}, true);
                }
            }

        } else {
            juce::OwnedArray<juce::PluginDescription> typesFound;
            knownPluginList.scanAndAddDragAndDroppedFiles (formatManager, files, typesFound);

            auto pos = graphEditor->getLocalPoint (this, juce::Point<int> (x, y));

            for (int i = 0; i < juce::jmin (5, typesFound.size()); ++i) {
                if (auto* desc = typesFound.getUnchecked(i)) {
                    createPlugin (*desc, pos);
                }
            }
        }
    }
}

GraphDocumentComponent* HostWindow::getGraphEditor() const
{
    return graph_document_.get();
}

bool HostWindow::isDoublePrecisionProcessing()
{
    if (auto* props = getAppProperties().getUserSettings())
        return props->getBoolValue ("doublePrecisionProcessing", false);

    return false;
}

void HostWindow::updatePrecisionMenuItem(juce::ApplicationCommandInfo& info)
{
    info.setInfo ("Double floating point precision rendering", juce::String(), "General", 0);
    info.setTicked (isDoublePrecisionProcessing());
}

}}} // ompu
