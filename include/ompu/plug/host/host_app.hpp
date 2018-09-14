#pragma once

#include "ompu/plug/host_fwd.hpp"
#include "ompu/plug/host/host_window.hpp"
#include "ompu/plug/host/internal_filter.hpp"

#include "ompu/plug/app_fwd.hpp"
#include "ompu/plug/logger.hpp"

#include "saya/logger.hpp"

#include <boost/filesystem/path.hpp>

#include <memory>
#include <fstream>


namespace ompu { namespace plug { namespace host {

class HostApp
    : public juce::JUCEApplication
    , public juce::ChangeBroadcaster
    , juce::AsyncUpdater
{
public:
    HostApp() = default;
    ~HostApp() = default;

    void initialise(juce::String const&) override;
    void handleAsyncUpdate() override;
    void shutdown() override;
    void systemRequestedQuit() override;

    juce::String const getApplicationName() override;
    juce::String const getApplicationVersion() override;
    bool moreThanOneInstanceAllowed() override;

    juce::ApplicationCommandManager& commandManager() { return cmd_mgr_; }
    juce::ApplicationProperties& appProperties() { return *app_props_; }

    boost::filesystem::path config_path() const;

private:
    boost::filesystem::path log_path() const;
    boost::filesystem::path temp_path() const { return config_path()/"temp"; }
    std::ofstream make_temp_file(std::string const& name);

    boost::filesystem::path log_file_path() const { return log_path() / "ompu.log"; }

    ompu::plug::App& app() noexcept;
    ompu::plug::App const& app() const noexcept;

    saya::logger& l() noexcept { return *l_; }

    // initializers
    void init_system();
    void init_logger();
    void save_states();

    juce::ApplicationCommandManager cmd_mgr_;
    std::unique_ptr<juce::ApplicationProperties> app_props_;

    // paths
    boost::filesystem::path config_path_;

    // logs
    std::unique_ptr<logger::DebugBuffer> l_dbg_buf_;
    std::ofstream l_file_;
    std::unique_ptr<saya::logger> l_;

    std::unique_ptr<ompu::plug::App> app_;
    std::unique_ptr<HostWindow> host_window_;
};

HostApp& getApp();
juce::ApplicationCommandManager& getCommandManager();
juce::ApplicationProperties& getAppProperties();

}}} // ompu
