#include "ompu/plug/host/host_app.hpp"
#include "ompu/plug/app.hpp"

#include "ompu/util.hpp"
#include "ompu/io.hpp"

#include "ompu/music/debug.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <codecvt>
#include <iostream>
#include <string>

#if BOOST_MSVC
#include <windows.h>
#endif


namespace ompu { namespace plug { namespace host {

void HostApp::initialise(juce::String const&)
{
    {
        saya::stream_lock::Init();
        this->init_system();
    }

    {
        juce::PropertiesFile::Options options;
        options.applicationName = this->getApplicationName();
        options.folderName = this->getApplicationName();
        options.filenameSuffix = "settings";
        options.osxLibrarySubFolder = "Preferences";

        app_props_ = std::make_unique<juce::ApplicationProperties>();
        app_props_->setStorageParameters(options);
    }

    // ---------------------------------
    namespace fs = boost::filesystem;
    {
        auto const& sp = app_props_->getStorageParameters();
        config_path_ = fs::path(sp.getDefaultFile().getFullPathName().toStdString()).parent_path();
    }

    // fs::create_directories(this->config_path());
    fs::create_directories(this->log_path());
    fs::create_directories(this->temp_path());

    // ---------------------------------
    l_dbg_buf_ = std::make_unique<logger::DebugBuffer>();
    l_ = std::make_unique<saya::logger>(saya::logger_env{l_dbg_buf_.get(), false}, app().name());

    this->init_logger();

    l().note() << app_props_->getStorageParameters().getDefaultFile().getFullPathName() << std::endl;
    l().info() << "using config file" << std::endl;

    // ompu::music debug code -------------------------------------
    {
        auto f = make_temp_file("ompu-music-debug.txt");
        ompu::music::debug_help(f);
    }

    // App context -------------------------------------
    app_ = std::make_unique<ompu::plug::App>(l().envs());

    // ---------------------------------------------------------
    // sync and upgrade versions if necessary

    {
        l().info() << "checking component version consistency..." << std::endl;
        auto* const s = app_props_->getCommonSettings(true);

        try {
            app().version_set().upgrade_from(
                App::version_set_type{
                    ompu::make_component_version<app_versions::ompu_app>(ompu::make_version(s->getValue("ompu_app_version").toStdString())),
                    ompu::make_component_version<app_versions::ompu_core>(ompu::make_version(s->getValue("ompu_core_version").toStdString()))
                },
                ompu::version_policies::upgrade_by_minor{}
            );

        } catch (std::invalid_argument const& e) {
            l().error() << e.what() << std::endl;
            l().warn() << "fallbacking to current version" << std::endl;
        }

        for (auto const& kv : app().version_set().components()) {
            auto const& last_run_version = kv.second;
            auto const& current_version = App::last_tested_versions().at(kv.first);

            if (current_version > last_run_version) {
                l().info()
                    << "upgraded: "
                    << boost::apply_visitor(app_versions::make_string{}, kv.first)
                    << " (" << last_run_version << ") ->"
                    << " (" << current_version << ")"
                    << std::endl
                ;
            }
        }
    }

    // ---------------------------------------------------------

    host_window_ = std::make_unique<HostWindow>(*app_, l().envs());
    host_window_->setUsingNativeTitleBar(true);

    // ---------------------------------

    cmd_mgr_.registerAllCommandsForTarget(this);
    cmd_mgr_.registerAllCommandsForTarget(host_window_.get());

    host_window_->menuItemsChanged();

    // notify all GUIs
    this->sendChangeMessage();

    // Important note! We're going to use an async update here so that if we need
    // to re-open a file and instantiate some plugins, it will happen AFTER this
    // initialisation method has returned.
    // On Windows this probably won't make a difference, but on OSX there's a subtle event loop
    // issue that can happen if a plugin runs one of those irritating modal dialogs while it's
    // being loaded. If that happens inside this method, the OSX event loop seems to be in some
    // kind of special "initialisation" mode and things get confused. But if we load the plugin
    // later when the normal event loop is running, everything's fine.
    triggerAsyncUpdate();
}


void HostApp::init_system()
{
#if BOOST_MSVC
    ::SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
#endif
}

void HostApp::init_logger()
{
    // tee to file...
    l_file_.open(log_file_path().string(), std::ios::out | std::ios::app);
    l().tee(saya::logger_env{l_file_.rdbuf(), false});

    l().info()
        << boost::format(
            "------------------------------\n"
            "[version info (last tested at)]\n"
            "%s\n"
            "[runtime info]\n"
            "timestamp: %s\n"
            "------------------------------"
        )
        % boost::algorithm::join(App::last_tested_versions().components() | boost::adaptors::transformed([] (auto const& kv) -> std::string {
            return boost::apply_visitor(app_versions::make_string{}, kv.first) + ": " + kv.second.str();
        }), ", ")
        % ompu::util::time_print{std::chrono::system_clock::now()}
        << std::endl
    ;
}

void HostApp::save_states()
{
    l().info() << "saving all states into config file..." << std::endl;
    l().error() << "FIXME: not implemented" << std::endl;
}

void HostApp::handleAsyncUpdate()
{
    juce::File fileToOpen;

    for (int i = 0; i < getCommandLineParameterArray().size(); ++i) {
        fileToOpen = juce::File::getCurrentWorkingDirectory().getChildFile (getCommandLineParameterArray()[i]);
        if (fileToOpen.existsAsFile()) break;
    }

    if (!fileToOpen.existsAsFile()) {
        juce::RecentlyOpenedFilesList recentFiles;
        recentFiles.restoreFromString (getAppProperties().getUserSettings()->getValue ("recentFilterGraphFiles"));

        if (recentFiles.getNumFiles() > 0)
            fileToOpen = recentFiles.getFile (0);
    }

    if (fileToOpen.existsAsFile()) {
        if (GraphDocumentComponent* graph = host_window_->getGraphEditor()) {
            if (FilterGraph* ioGraph = graph->graph.get()) {
                ioGraph->loadFrom(fileToOpen, true);
            }
        }
    }
}

boost::filesystem::path HostApp::config_path() const { return config_path_; }
boost::filesystem::path HostApp::log_path() const { return config_path_/"log"; }

std::ofstream HostApp::make_temp_file(std::string const& name)
{
    std::ofstream ofs{(this->temp_path()/name).string(), std::ios::binary};
    return ofs;
}

void HostApp::shutdown()
{
    host_window_.reset();
    app_.reset();

    app_props_.reset();
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);

    juce::Logger::setCurrentLogger(nullptr);
}

void HostApp::systemRequestedQuit()
{
    if (host_window_) {
        host_window_->tryToQuitApplication();
    } else {
        juce::JUCEApplicationBase::quit();
    }
}

juce::String const HostApp::getApplicationName() { return app().name(); }
juce::String const HostApp::getApplicationVersion() { return app().version_set().at(app_versions::ompu_app{}).str(); }
bool HostApp::moreThanOneInstanceAllowed() { return true; }

ompu::plug::App& HostApp::app() noexcept { return *app_; }
ompu::plug::App const& HostApp::app() const noexcept { return *app_; }


HostApp& getApp() { return *static_cast<HostApp*>(juce::JUCEApplication::getInstance()); }
juce::ApplicationCommandManager& getCommandManager() { return getApp().commandManager(); }
juce::ApplicationProperties& getAppProperties() { return getApp().appProperties(); }

START_JUCE_APPLICATION (HostApp)

}}} // ompu
