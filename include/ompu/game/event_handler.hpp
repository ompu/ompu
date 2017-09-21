#pragma once

#include "ompu/game/game_fwd.hpp"
#include "ompu/midi/midi_fwd.hpp"

#include <memory>


namespace ompu { namespace game {

class EventHandler
{
public:
    explicit EventHandler(GameData* gd);
    ~EventHandler() = default;

    void in_midi(std::unique_ptr<midi::Message> v);

#if 0
    void in_midi_sysex(std::unique_ptr<midi::Sysex> v);
#endif


    EventHandler(EventHandler const&) = delete;
    EventHandler(EventHandler&&) = delete;

    EventHandler& operator=(EventHandler const&) = delete;
    EventHandler& operator=(EventHandler&&) = delete;

private:
    GameData* const gd_;
};

}} // ompu
