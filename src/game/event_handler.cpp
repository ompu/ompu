#include "ompu/game/event_handler.hpp"
#include "ompu/game/game_data.hpp"

#include "ompu/midi/message.hpp"
#include "ompu/midi/sysex.hpp"


namespace ompu { namespace game {

EventHandler::EventHandler(GameData* const gd)
    : gd_(gd)
{
    // ...
}

void EventHandler::in_midi(std::unique_ptr<midi::Message> v)
{
    auto* const p = v.get();
    while (!gd_->in_midi_.push(p)) {}

    std::unique_lock<decltype(gd_->store_mtx_)>
    lock(gd_->store_mtx_, std::defer_lock);

    if (lock.try_lock()) {
        for (auto& kv : gd_->in_midi_store_sub_) {
            gd_->in_midi_store_[kv.first] = std::move(kv.second);
        }
        gd_->in_midi_store_sub_.clear();

        gd_->in_midi_store_[p] = std::move(v);

    } else {
        gd_->in_midi_store_sub_[p] = std::move(v);
    }
}

#if 0
void EventHandler::in_midi_sysex(std::unique_ptr<midi::Sysex> v)
{
    auto* const p = v.get();
    while (!gd_->in_midi_sysex_.push(p)) {}
    gd_->in_midi_sysex_store_[p] = std::move(v);
}
#endif

}} // ompu
