#pragma once

#include "ompu/music/music_fwd.hpp"


namespace ompu { namespace music {

template<unsigned Height>
struct tone_height : std::integral_constant<unsigned, Height>
{
    static_assert(Height <= 11, "0 <= tone height <= 11");
};

template<unsigned Height>
struct relative_tone_height : std::integral_constant<unsigned, Height>
{
    static_assert(Height <= 11, "0 <= relative tone height <= 11");
};

template<int Offset>
struct tone_offset : std::integral_constant<int, Offset>
{};

}} // ompu
