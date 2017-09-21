#pragma once

#include "ompu/game/game_fwd.hpp"

#include <stdexcept>


namespace ompu { namespace game {

struct game_error : std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

}} // ompu
