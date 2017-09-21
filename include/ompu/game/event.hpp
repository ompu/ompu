#pragma once

#include "ompu/game/game_fwd.hpp"

namespace ompu { namespace game {

class Event
{
public:
    Event() = default;
    virtual ~Event() = default;

    Event(Event const&) = delete;
    Event(Event&&) noexcept = default;

    Event& operator=(Event const&) = delete;
    Event& operator=(Event&&) noexcept = default;

    virtual char const* name() const noexcept = 0;
};

}} // ompu
