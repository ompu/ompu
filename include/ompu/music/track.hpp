#pragma once

#include "ompu/music/traits.hpp"
#include "ompu/music/key.hpp"
#include "ompu/music/chord.hpp"


namespace ompu { namespace music {

template<class Traits = music_traits<>>
class basic_track
{
public:
    using traits_type = Traits;

private:
};

using Track = basic_track<>;

}} // ompu
