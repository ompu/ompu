#pragma once

#include "ompu/music/music_fwd.hpp"


namespace ompu { namespace music {

template<unsigned A4_Freq = 440>
struct music_traits
{
    static constexpr unsigned A4_Freq = A4_Freq;
};

}} // ompu
