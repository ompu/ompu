#pragma once

#include "ompu/music/music_fwd.hpp"
#include "ompu/music/key.hpp"
#include "ompu/music/type_traits.hpp"

#include <boost/variant.hpp>


namespace ompu { namespace music {

namespace concrete {

using any_mods = saya::zed::rewrap_t<
    std::tuple,
    boost::variant,
    all_mods
>;

using any_tones = saya::zed::rewrap_t<
    std::tuple,
    boost::variant,
    all_tones
>;

using any_keys = saya::zed::rewrap_t<
    std::tuple,
    boost::variant,
    all_keys
>;

enum struct note_names : unsigned
{
    C, D, E, F, G, A, B,
};

enum struct mod_names : int
{
    None,
    Sharp, DblSharp,
    Flat, DblFlat,
    Natural,
};

struct note
{
    note_names name;
    mod_names mod;
};

struct context
{
    any_keys key;
};

} // concrete

}} // ompu
