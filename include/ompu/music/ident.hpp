#pragma once

#include "ompu/music/mod.hpp"

#include <type_traits>


namespace ompu { namespace music {

namespace detail {

template<unsigned Root, class Mod>
struct calc_tone_height;

template<unsigned Root>
struct calc_tone_height<Root, mods::None> : std::integral_constant<unsigned, Root> {};
template<unsigned Root>
struct calc_tone_height<Root, mods::Sharp> : std::integral_constant<unsigned, Root + 1> {};
template<unsigned Root>
struct calc_tone_height<Root, mods::Flat> : std::integral_constant<unsigned, Root - 1> {};
template<unsigned Root>
struct calc_tone_height<Root, mods::DblSharp> : std::integral_constant<unsigned, Root + 2> {};
template<unsigned Root>
struct calc_tone_height<Root, mods::DblFlat> : std::integral_constant<unsigned, Root - 2> {};
template<unsigned Root>
struct calc_tone_height<Root, mods::Natural> : std::integral_constant<unsigned, Root> {};


template<unsigned Root, class Mod>
struct basic_tone_ident : std::integral_constant<unsigned, detail::calc_tone_height<Root, Mod>::value>
{
    using mod_type = Mod;
    static constexpr bool sharped = std::is_same_v<Mod, mods::Sharp>;
    static constexpr bool flatted = std::is_same_v<Mod, mods::Flat>;
};

} // detail

template<unsigned Root, class Mod>
using make_basic_tone_ident = detail::basic_tone_ident<Root % 12, Mod>;


namespace idents {

struct C : make_basic_tone_ident<0, mods::None> {};
struct Cs : make_basic_tone_ident<1, mods::Sharp> {};
struct Cb : make_basic_tone_ident<11, mods::Flat> {};
struct D : make_basic_tone_ident<2, mods::None> {};
struct Ds : make_basic_tone_ident<3, mods::Sharp> {};
struct Db : make_basic_tone_ident<1, mods::Flat> {};
struct E : make_basic_tone_ident<4, mods::None> {};
struct Es : make_basic_tone_ident<5, mods::Sharp> {};
struct Eb : make_basic_tone_ident<3, mods::Flat> {};
struct F : make_basic_tone_ident<5, mods::None> {};
struct Fs : make_basic_tone_ident<6, mods::Sharp> {};
struct Fb : make_basic_tone_ident<4, mods::Flat> {};
struct G : make_basic_tone_ident<7, mods::None> {};
struct Gs : make_basic_tone_ident<8, mods::Sharp> {};
struct Gb : make_basic_tone_ident<6, mods::Flat> {};
struct A : make_basic_tone_ident<9, mods::None> {};
struct As : make_basic_tone_ident<10, mods::Sharp> {};
struct Ab : make_basic_tone_ident<8, mods::Flat> {};
struct B : make_basic_tone_ident<11, mods::None> {};
struct Bs : make_basic_tone_ident<0, mods::Sharp> {};
struct Bb : make_basic_tone_ident<10, mods::Flat> {};

} // idents

namespace detail {

template<class Ident> struct sharp_counter : std::integral_constant<unsigned, 0> {};
template<unsigned Root> struct sharp_counter<make_basic_tone_ident<Root, mods::Sharp>> : std::integral_constant<unsigned, 1> {};
template<unsigned Root> struct sharp_counter<make_basic_tone_ident<Root, mods::DblSharp>> : std::integral_constant<unsigned, 2> {};

template<class Ident> struct flat_counter : std::integral_constant<unsigned, 0> {};
template<unsigned Root> struct flat_counter<make_basic_tone_ident<Root, mods::Flat>> : std::integral_constant<unsigned, 1> {};
template<unsigned Root> struct flat_counter<make_basic_tone_ident<Root, mods::DblFlat>> : std::integral_constant<unsigned, 2> {};

} // detail

}} // ompu
