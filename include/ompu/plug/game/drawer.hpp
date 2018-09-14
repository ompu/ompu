#pragma once

#include "ompu/plug/game/game_fwd.hpp"

#include "ompu/game/visitor.hpp"
#include "ompu/game/scene.hpp"

#include "ompu/ui/all.hpp"

#include "saya/logger.hpp"

#include "JuceHeader.h"


namespace ompu { namespace plug { namespace game {

class Drawer : public boost::static_visitor<ompu::game::drawer_return_type>
{
public:
    explicit Drawer(saya::logger_env_set envs, UISet* uis);

    constexpr static char const* name() noexcept { return "ompu-app"; }

    void game_data(ompu::game::drawer_game_data_type gd);


    void juce_component(juce::Component const* jc) noexcept { jc_ = jc; }
    void juce_draw_context(juce::Graphics* const g) noexcept { g_ = g; }

    ompu::game::drawer_return_type
    operator()(ompu::game::scenes::Noop const&) const;

    ompu::game::drawer_return_type
    operator()(ompu::game::scenes::Title const&) const;

    ompu::game::drawer_return_type
    operator()(ompu::game::scenes::Single const&) const;

private:
    void partial(ompu::ui::ModWheel const& v) const;
    void partial(ompu::ui::PitchWheel const& v) const;
    void partial(ompu::ui::Keyboard const& v) const;

    saya::logger l_;
    UISet* const uis_;
    ompu::game::drawer_game_data_type gd_{nullptr};
    juce::Component const* jc_{nullptr};
    juce::Graphics* g_{nullptr};
};

}}} // ompu
