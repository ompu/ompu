#pragma once

#include "ompu/plug/device_manager.hpp"
#include "ompu/plug/io.hpp"

#include "ompu/plug/game/game_fwd.hpp"

#include "ompu/game/game.hpp"

#include "ompu/io.hpp"
#include "ompu/component_id.hpp"
#include "ompu/version.hpp"

#include "saya/logger.hpp"

#include <boost/variant/static_visitor.hpp>
#include <boost/assert.hpp>

#include "JuceHeader.h"

#include <thread>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <type_traits>


namespace ompu { namespace plug {

namespace app_versions {

struct version_tag {};

template<class Tag, std::enable_if_t<std::is_base_of_v<version_tag, Tag>, int> = 0>
inline bool operator==(Tag, Tag) noexcept { return true; }

template<class Tag1, class Tag2, std::enable_if_t<std::is_base_of_v<version_tag, Tag1> && std::is_base_of_v<version_tag, Tag2>, int> = 0>
inline bool operator==(Tag1, Tag2) noexcept { return false; }

template<class Tag, std::enable_if_t<std::is_base_of_v<version_tag, Tag>, int> = 0>
inline std::size_t
hash_value(Tag) { return 42; /* arbitrary value */ }

struct ompu_app : version_tag {};
struct ompu_core : version_tag {};

struct make_string : boost::static_visitor<std::string>
{
    std::string operator()(ompu_app) const { return "ompu-app"; }
    std::string operator()(ompu_core) const { return "ompu-core"; }
};

} // app_versions


class App;

using game_type = ompu::game::Game<
      game::Updater
    , game::Drawer
>;

namespace detail {

struct game_access
{
protected:
    static game_type* get_game(App& app) noexcept;
};

} // detail


class App
{
public:
    using version_set_type = ompu::VersionSet<
        app_versions::ompu_app,
        app_versions::ompu_core
    >;

private:
    static std::string const NAME;

public:
    friend struct detail::game_access;
    static std::string const LOW_LATENCY_DEV_TYPE;

    explicit App(saya::logger_env_set l_envs);
    ~App();

    saya::logger const& l() const { return l_; }
    saya::logger& l() { return l_; }

    juce::AudioDeviceManager const& dm() const { return dm_; }
    juce::AudioDeviceManager& dm() { return dm_; }

    juce::OwnedArray<juce::AudioIODeviceType> dtypes;
    juce::AudioIODeviceType* dtype{nullptr};

    std::string const& name() const noexcept { return NAME; }

    static version_set_type const& last_tested_versions() { return LAST_TESTED_VERSIONS; }
    version_set_type& version_set() noexcept { return version_set_; }
    version_set_type const& version_set() const noexcept { return version_set_; }

    int desired_fps() const noexcept { return 120; }

    game_type* const game() { return game_.get(); }
    game_type const* const game() const { return game_.get(); }

    // juce related --------------------
    void juce_component(juce::Component const* jc) noexcept;
    void juce_draw_context(juce::Graphics* g) noexcept;
    void set_midi_input(bool enable, ComponentID const& device_id, ompu::midi::Filter filter);

private:
    static version_set_type const LAST_TESTED_VERSIONS;
    version_set_type version_set_;

    saya::logger l_;
    DeviceManager dm_;

    std::unordered_map<ComponentID, std::unique_ptr<juce::MidiInput>>
    midi_ins_;

    std::mutex midi_in_mtx_;

    std::unique_ptr<game::UISet> uis_;
    std::unique_ptr<game::Updater> suv_;
    std::unique_ptr<game::Drawer> sdv_;

    std::unique_ptr<game_type> game_;
    std::unique_ptr<game::EventHandler> eh_;

#if 0
    std::thread game_thread_;
#endif
};

}} // ompu
