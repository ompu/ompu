#include "ompu/plug/game/updater.hpp"
#include "ompu/ui/keyboard.hpp"

#include "ompu/game/game_data_snapshot.hpp"

#include <boost/core/ignore_unused.hpp>


namespace ompu { namespace plug { namespace game {

void Updater::partial(ompu::ui::Keyboard& c)
{
    for (auto const& msg : gd_->midi_msgs) {
        switch (msg->type) {
        case midi::MessageType::NoteOn:
            c.press_at_midi(msg->note_num, true);
            break;

        case midi::MessageType::NoteOff:
            c.press_at_midi(msg->note_num, false);
            break;

        default:
            break;
        }
    }
}

}}} // ompu
