#pragma once

#include "ompu/music_fwd.hpp"


namespace ompu { namespace music {

namespace mods {

struct None : std::integral_constant<int, 0> {};
struct Sharp : std::integral_constant<int, 1> {};
struct Flat : std::integral_constant<int, -1> {};

struct DblSharp : std::integral_constant<int, 2> {};
struct DblFlat : std::integral_constant<int, -2> {};

struct Natural : std::integral_constant<int, 0> {};

} // mods

namespace detail {

template<unsigned Height, class Mod>
struct canonical_mod_shift;

template<unsigned Height>
struct canonical_mod_shift<Height, mods::Sharp>
    : std::conditional_t<
        Height == 11,
        std::integral_constant<unsigned, 0>,
        std::integral_constant<unsigned, Height + 1>
    >
{};

template<unsigned Height>
struct canonical_mod_shift<Height, mods::Flat>
    : std::conditional_t<
        Height == 0,
        std::integral_constant<unsigned, 11>,
        std::integral_constant<unsigned, Height - 1>
    >
{};

template<unsigned Height, class Mod>
using canonical_mod_shift_t = typename canonical_mod_shift<Height, Mod>::type;

} // detail

}} // ompu
