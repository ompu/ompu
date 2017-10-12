#include "ompu/game/event_handler.hpp"
#include "ompu/game/scheduler.hpp"
#include "ompu/game/game_data.hpp"

#include "ompu/music/concrete/note.hpp"
#include "ompu/music/concrete/context.hpp"

#include "ompu/midi/message.hpp"
#include "ompu/midi/sysex.hpp"


namespace ompu { namespace game {

EventHandler::EventHandler(Scheduler& sch, GameData& gd)
    : sch_(sch)
    , gd_(gd)
{
    // ...
}

void EventHandler::in_midi(std::shared_ptr<midi::Message> msg)
{
    if (msg->is_note_on_or_off()) {
        sch_.enqueue([this, msg] {
            gd_.music_ctx().add_or_remove(msg->type == midi::MessageType::NoteOn, music::concrete::Note{*msg});
        });
    }

    auto* const p = msg.get();
    while (!gd_.in_midi_.push(p)) {}

    std::unique_lock<decltype(gd_.store_mtx_)>
    lock(gd_.store_mtx_, std::defer_lock);

    if (lock.try_lock()) {
        for (auto& kv : gd_.in_midi_store_sub_) {
            gd_.in_midi_store_[kv.first] = std::move(kv.second);
        }
        gd_.in_midi_store_sub_.clear();

        gd_.in_midi_store_[p] = msg;

    } else {
        gd_.in_midi_store_sub_[p] = msg;
    }
}

#if 0
void EventHandler::in_midi_sysex(std::unique_ptr<midi::Sysex> v)
{
    auto* const p = v.get();
    while (!gd_.in_midi_sysex_.push(p)) {}
    gd_.in_midi_sysex_store_[p] = std::move(v);
}
#endif

}} // ompu
