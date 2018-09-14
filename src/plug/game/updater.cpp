#include "ompu/plug/game/updater.hpp"
#include "ompu/plug/game/ui_set.hpp"

#include "ompu/game/game_data_snapshot.hpp"

#include <iostream>


namespace ompu { namespace plug { namespace game {

Updater::Updater(
    saya::logger_env_set envs,
    UISet* const uis
)
    : l_(std::move(envs), "Scene|U")
    , uis_(uis)
{}

void Updater::game_data(ompu::game::updater_game_data_type gd)
{
    gd_ = std::move(gd);
}

ompu::game::updater_return_type
Updater::operator()(ompu::game::scenes::Noop const&)
{
    // return gd_->scene;
    return ompu::game::scenes::Title{};
}

ompu::game::updater_return_type
Updater::operator()(ompu::game::scenes::Title const&)
{
    this->partial(uis_->keyboard);

    for (auto const& msg : gd_->midi_msgs) {
        l_.warn() << "incoming MIDI: " << msg->type << std::endl;

        switch (msg->type) {
        case midi::MessageType::NoteOn:
        case midi::MessageType::NoteOff:
            l_.warn() << "got MIDI note: " << msg->note_num << std::endl;
            break;
        }
    }

    return gd_->scene;
}

ompu::game::updater_return_type
Updater::operator()(ompu::game::scenes::Single const&)
{
    return gd_->scene;
}

}}} // ompu
