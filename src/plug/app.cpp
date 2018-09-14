#include "ompu/plug/app.hpp"
#include "ompu/plug/game/event_handler.hpp"
#include "ompu/plug/game/updater.hpp"
#include "ompu/plug/game/drawer.hpp"
#include "ompu/plug/game/ui_set.hpp"

#include "ompu/game/game.hpp"
#include "ompu/game/game_data.hpp"
#include "ompu/game/game_data_snapshot.hpp"

#include "ompu/midi.hpp"
#include "ompu/ompu_lib.hpp"

#include <boost/config.hpp>

#include <string>


namespace ompu { namespace plug {

std::string const App::NAME{"ompu"};

std::string const App::LOW_LATENCY_DEV_TYPE =
#if BOOST_MSVC
    "ASIO"
#else
#error low-latency device type not specified for this platform
#endif
;


namespace detail {

game_type* game_access::get_game(App& app) noexcept
{
    return app.game_.get();
}

} // detail

App::version_set_type const App::LAST_TESTED_VERSIONS{
    ompu::make_component_version<app_versions::ompu_app>(ompu::Version{1, 0, 0, "alpha"}),
    ompu::make_component_version<app_versions::ompu_core>(ompu::Version{1, 0, 0, "alpha"})
};


App::App(saya::logger_env_set l_envs)
    : version_set_(LAST_TESTED_VERSIONS)
    , l_(std::move(l_envs), "App")
    , eh_(std::make_unique<game::EventHandler>(game_->eh()))
    , uis_(std::make_unique<game::UISet>())
    , suv_(std::make_unique<game::Updater>(l_.envs(), uis_.get()))
    , sdv_(std::make_unique<game::Drawer>(l_.envs(), uis_.get()))
    , game_(std::make_unique<game_type>(l_.envs(), suv_.get(), sdv_.get()))
{
    uis_->init(this);
    game_->start();

#if 0
    game_thread_ = std::thread([this] {
        game_->run();
    });
#endif
}

App::~App()
{
    game_->stop();

#if 0
    if (game_thread_.joinable()) {
        game_thread_.join();
    }
#endif
}

void App::juce_component(juce::Component const* const jc) noexcept
{
    sdv_->juce_component(jc);
}

void App::juce_draw_context(juce::Graphics* const g) noexcept
{
    sdv_->juce_draw_context(g);
}

void App::set_midi_input(bool enable, ComponentID const& device_id, ompu::midi::Filter filter)
{
    std::lock_guard<std::mutex> lock{midi_in_mtx_};

    if (enable) {
        if (midi_ins_.count(device_id)) {
            l_.note() << "device id: " << device_id << std::endl;
            l_.error() << "attempt to enable an already open MIDI input device" << std::endl;
            return;
        }

        auto& in = midi_ins_[device_id];

        // TODO: assigning in inverse order; is this okay?
        in.reset(juce::MidiInput::openDevice(device_id.index(), eh_.get()));
        eh_->add_midi_in_dev(in.get(), device_id, std::move(filter));

        in->start();

    } else {
        if (!midi_ins_.count(device_id)) {
            l_.note() << "device id: " << device_id << std::endl;
            l_.error() << "attempt to disable an unmanaged MIDI input device" << std::endl;
            return;
        }

        auto& in = midi_ins_.at(device_id);
        in->stop();
        eh_->remove_midi_in_dev(in.get());
        in.reset();
    }
}

}} // ompu

