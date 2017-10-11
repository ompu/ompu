#pragma once

#include "ompu/music/music_fwd.hpp"
#include "ompu/music/chord_note.hpp"

#include <tuple>


namespace ompu { namespace music {

template<class CN3, class CN5, class CN6, class CN7>
struct chord_fund_set
{
    using cn3 = CN3; using cn5 = CN5; using cn6 = CN6; using cn7 = CN7;
    static_assert(!std::is_void_v<cn5>, "a chord must (at least) have the 5th note");

    static constexpr std::size_t unsafe_count = saya::zed::fold_add_v<
        std::size_t,
        1, // <-- the root note
        !std::is_void_v<cn3>, !std::is_void_v<cn5>, !std::is_void_v<cn6>, !std::is_void_v<cn7>
    >;

    static_assert(unsafe_count == 3 || unsafe_count == 4, "a chord must be at least triad or tetrad");
    // using notes_type = saya::zed::compact_t<std::tuple, cn3, cn5, cn6, cn7>;
};

template<class... Tensions>
struct chord_tension_set
{
    static_assert(saya::zed::any_of<std::is_void, saya::zed::template_<>, Tensions...>::value, "tension note cannot be void");
    static constexpr bool is_empty = false;
    static constexpr std::size_t count = sizeof...(Tensions);
    static_assert(count <= 2, "a chord cannot have more than 2 tension notes");
};

template<>
struct chord_tension_set<>
{
    static constexpr bool is_empty = true;
    static constexpr std::size_t count = 0;
};

}} // ompu
