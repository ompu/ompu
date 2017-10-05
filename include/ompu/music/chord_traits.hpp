#pragma once

#include "ompu/music/music_fwd.hpp"
#include "ompu/music/chord_note.hpp"
#include "ompu/music/chord_note_set.hpp"
#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

template<class FundSet, class TensionSet>
struct chord_traits
{
    using fund_set = FundSet;
    using tension_set = TensionSet;

    static constexpr bool has_tension = !TensionSet::is_empty;

    static constexpr bool is_triad = FundSet::unsafe_count == 3;
    static constexpr bool is_tetrad = FundSet::unsafe_count == 4;

    static_assert(
        is_power_chord_v<FundSet, TensionSet> || (is_triad || is_tetrad),
        "a chord must be either (1) power chord (2) triad (3) tetrad"
    );
};

}} // ompu
