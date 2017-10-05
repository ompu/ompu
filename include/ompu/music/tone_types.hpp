#pragma once

#include "ompu/music/music_fwd.hpp"


namespace ompu { namespace music {

template<unsigned Height>
struct tone_height : std::integral_constant<unsigned, Height>
{
    static_assert(Height <= 11, "0 <= tone height <= 11");
};

template<int UnsafeOffset>
struct relative_height // : std::integral_constant<int, Offset>
{
    // static_assert(Height <= 11, "0 <= relative tone height <= 11");
    static constexpr int unsafe_offset = UnsafeOffset;
    static constexpr int canonical_offset = UnsafeOffset < 0 ? -(-UnsafeOffset % 12) : (UnsafeOffset % 12);
    static constexpr std::size_t nth_octave = (UnsafeOffset < 0 ? (-UnsafeOffset) : (UnsafeOffset)) / 12;
};

template<int Offset>
struct tone_offset : std::integral_constant<int, Offset>
{};

}} // ompu
