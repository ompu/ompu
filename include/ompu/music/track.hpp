#pragma once

#include "ompu/music/music_fwd.hpp"
#include "ompu/music/music_traits.hpp"
#include "ompu/music/key.hpp"
#include "ompu/music/chord.hpp"
#include "ompu/music/symbol.hpp"


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
