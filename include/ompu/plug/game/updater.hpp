#pragma once

#include "ompu/plug/game/game_fwd.hpp"

#include "ompu/game/visitor.hpp"
#include "ompu/game/scene.hpp"

#include "ompu/ui/all.hpp"

#include "saya/logger.hpp"


namespace ompu { namespace plug { namespace game {

class Updater : public boost::static_visitor<ompu::game::updater_return_type>
{
public:
    explicit Updater(saya::logger_env_set envs, UISet* uis);

    constexpr static char const* name() noexcept { return "ompu-app"; }

    void game_data(ompu::game::updater_game_data_type gd);

    ompu::game::updater_return_type
    operator()(ompu::game::scenes::Noop const&);

    ompu::game::updater_return_type
    operator()(ompu::game::scenes::Title const&);

    ompu::game::updater_return_type
    operator()(ompu::game::scenes::Single const&);

private:
    void partial(ompu::ui::ModWheel& c);
    void partial(ompu::ui::PitchWheel& c);
    void partial(ompu::ui::Keyboard& c);


    saya::logger l_;
    UISet* const uis_;
    ompu::game::updater_game_data_type gd_{nullptr};
};

}}} // ompu
