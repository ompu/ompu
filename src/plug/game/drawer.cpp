#include "ompu/plug/game/drawer.hpp"
#include "ompu/plug/game/ui_set.hpp"

#include "ompu/game/game_data_snapshot.hpp"

#include <iostream>


namespace ompu { namespace plug { namespace game {

Drawer::Drawer(
    saya::logger_env_set envs,
    UISet* const uis
)
    : l_(std::move(envs), "Scene|D")
    , uis_(uis)
{}

void Drawer::game_data(ompu::game::drawer_game_data_type gd)
{
    gd_ = std::move(gd);
}

ompu::game::drawer_return_type
Drawer::operator()(ompu::game::scenes::Noop const&) const
{
    // always noop
}

ompu::game::drawer_return_type
Drawer::operator()(ompu::game::scenes::Title const&) const
{
    auto const origin = jc_->getLocalBounds();

    g_->setColour(juce::Colour::fromRGB(240, 240, 255));
    g_->fillRect(origin);

    this->partial(uis_->keyboard);
}

ompu::game::drawer_return_type
Drawer::operator()(ompu::game::scenes::Single const&) const
{
    // FIXME
}

}}} // ompu
