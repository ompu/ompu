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

template<class... Names>
struct chord_note_gather_names
{
private:
    template<class T1>
    static constexpr auto plus(T1&& v1) { return v1; }

    template<class T1, class T2, class... Rest>
    static constexpr auto plus(T1&& v1, T2&& v2, Rest&&... rests)
    { return plus(v1 + v2, std::forward<Rest>(rests)...); }

    template<class T1, class... Rest>
    static constexpr auto plus(T1&& v1, T1&& v2, Rest&&... rests) { return plus(v1 + v2, std::forward<Rest>(rests)...); }

public:
    static_assert(sizeof...(Names) != 0, "gathered names must not be empty");
    static constexpr auto name = plus(Names::name...);
};

template<class Tension>
struct chord_tension_single_name
{
    static constexpr auto name = number_names<Tension::tension_id>::name + Tension::mod_type::name;
};

template<class TensionSet>
struct chord_tension_name_impl;

template<class... Tensions>
struct chord_tension_name_impl<
    chord_tension_set<Tensions...>
>
{
    static_assert(!chord_tension_set<Tensions...>::is_empty, "tension set must not be empty to fetch names");
    static constexpr auto name = chord_note_gather_names<
        chord_tension_single_name<Tensions>...
    >::name;
};

template<class TensionSet, class Enabled = void>
struct chord_tension_name;

template<class TensionSet>
struct chord_tension_name<
    TensionSet,
    std::enable_if_t<!TensionSet::is_empty>
>
{
    static constexpr auto name =
        sprout::to_string("(") +
        chord_tension_name_impl<TensionSet>::name +
        sprout::to_string(")")
    ;
};

template<class TensionSet>
struct chord_tension_name<
    TensionSet,
    std::enable_if_t<TensionSet::is_empty>
>
{
    static constexpr auto const& name = symbol_names::empty;
};

template<class CN5, class CN7>
struct chord_ornament_5th_name { static constexpr auto const& name = symbol_names::empty; };
template<class CN5>
struct chord_ornament_5th_name<CN5, void> { static constexpr auto const& name = chord_note_name<CN5>::name; };

template<class CN5, class CN7>
struct chord_ornament_7th_name { static constexpr auto const& name = chord_note_name<CN7>::name; };

template<class CN7>
struct chord_ornament_7th_name<
    chord_notes::b5,
    CN7
>
{
    static constexpr auto const& name = std::conditional_t<
        std::is_same_v<CN7, chord_notes::dim7>,
        chord_note_gather_names<chord_note_name<chord_notes::dim7>>,
        chord_note_gather_names<chord_note_name<CN7>, chord_note_name<chord_notes::b5>>
    >::name;
};

template<
    class CN3,
    class CN5,
    class CN6,
    class CN7
>
struct chord_ornament_name_impl
{
    static constexpr auto const& name = std::conditional_t<
        std::is_void<CN6>::value,
        chord_note_gather_names<
            std::conditional_t<
                std::is_same<typename CN7::mod_type, mods::dbl_flat>::value,
                detail::symbol_name_empty,
                chord_note_name<CN3>
            >,
            chord_ornament_5th_name<CN5, CN7>,
            chord_ornament_7th_name<CN5, CN7>
        >,
        chord_note_gather_names<
            chord_note_name<CN3>,
            chord_note_name<CN6>
        >
    >::name;
};

template<class Traits>
struct chord_ornament_name
{
    static constexpr auto name =
        chord_ornament_name_impl<
            typename Traits::fund_set::cn3, typename Traits::fund_set::cn5, typename Traits::fund_set::cn6, typename Traits::fund_set::cn7
        >::name +
        chord_tension_name<
            typename Traits::tension_set
        >::name
    ;
};

} // detail


template<class FundSet, class TensionSet>
struct basic_chord
{
    using fund_set = FundSet;
    using tension_set = TensionSet;
    using traits_type = chord_traits<FundSet, TensionSet>;

    static constexpr auto const& ornament_name = std::conditional_t<
        is_power_chord_v<FundSet, TensionSet>,
        detail::number_names<5>,
        detail::chord_ornament_name<traits_type>
    >::name;
};

template<>
struct basic_chord<void, void>
{
    static constexpr auto ornament_name = sprout::to_string("(non-chord)");
};

}} // ompu
