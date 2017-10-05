#pragma once

#include "ompu/music/music_fwd.hpp"
#include "ompu/music/chord_note.hpp"
#include "ompu/music/chord_note_set.hpp"
#include "ompu/music/chord_traits.hpp"
#include "ompu/music/key_feel.hpp"
#include "ompu/music/tone_types.hpp"
#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace detail {

template<class TensionSet, class Enabled = void>
struct chord_tension_name_impl;

template<class TensionSet>
struct chord_tension_name_impl<
    TensionSet,
    std::enable_if_t<TensionSet::is_empty>
>
{
    static constexpr auto const& name = symbol_names::empty;
};

template<class TensionSet>
struct chord_tension_name_impl<
    TensionSet,
    std::enable_if_t<!TensionSet::is_empty>
>
{
    static constexpr auto const name = sprout::to_string("");
};


template<class Traits, class Enabled = void>
struct chord_ornament_name_impl;

template<class Traits>
struct chord_ornament_name_impl<
    Traits,
    std::enable_if_t<
        is_power_chord_v<typename Traits::fund_set, typename Traits::tension_set>
    >
>
{
    static constexpr auto name = sprout::to_string("5");
};

template<class Traits>
struct chord_ornament_name_impl<
    Traits,
    std::enable_if_t<
        is_seventh_chord_v<typename Traits::fund_set>
    >
>
{
    static constexpr auto name = sprout::to_string("5");
};

} // detail


template<class FundSet, class TensionSet>
struct basic_chord
{
    using fund_set = FundSet;
    using tension_set = TensionSet;
    using traits_type = chord_traits<FundSet, TensionSet>;

    static constexpr auto const& ornament_name = detail::chord_ornament_name_impl<traits_type>::name;
};

template<>
struct basic_chord<void, void>
{
    static constexpr auto ornament_name = sprout::to_string("(non-chord)");
};

}} // ompu
