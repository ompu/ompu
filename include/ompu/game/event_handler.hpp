#pragma once

#include "ompu/game/game_fwd.hpp"
#include "ompu/midi/midi_fwd.hpp"

#include <memory>


namespace ompu { namespace game {

class EventHandler
{
public:
    explicit EventHandler(Scheduler& sch, GameData& gd);
    ~EventHandler() = default;

    void in_midi(std::shared_ptr<midi::Message> msg);

#if 0
    void in_midi_sysex(std::unique_ptr<midi::Sysex> msg);
#endif


    EventHandler(EventHandler const&) = delete;
    EventHandler(EventHandler&&) = delete;

    EventHandler& operator=(EventHandler const&) = delete;
    EventHandler& operator=(EventHandler&&) = delete;

private:
    Scheduler& sch_;
    GameData& gd_;
};

}} // ompu
