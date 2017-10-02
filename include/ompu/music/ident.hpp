#pragma once

#include "ompu/music/mod.hpp"

#include <tuple>
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
    static constexpr unsigned root = Root;
    static constexpr bool sharped = std::is_same_v<Mod, mods::Sharp>;
    static constexpr bool flatted = std::is_same_v<Mod, mods::Flat>;

    static_assert(Root < 12, "tone_ident: Root height must be less than 12");
};

} // detail

template<unsigned Root, class Mod>
using make_tone_ident = detail::basic_tone_ident<Root % 12, Mod>;


namespace idents {

struct C : make_tone_ident<0, mods::None> {};
struct Cs : make_tone_ident<1, mods::Sharp> {};
struct Cb : make_tone_ident<11, mods::Flat> {};
struct D : make_tone_ident<2, mods::None> {};
struct Ds : make_tone_ident<3, mods::Sharp> {};
struct Db : make_tone_ident<1, mods::Flat> {};
struct E : make_tone_ident<4, mods::None> {};
struct Es : make_tone_ident<5, mods::Sharp> {};
struct Eb : make_tone_ident<3, mods::Flat> {};
struct F : make_tone_ident<5, mods::None> {};
struct Fs : make_tone_ident<6, mods::Sharp> {};
struct Fb : make_tone_ident<4, mods::Flat> {};
struct G : make_tone_ident<7, mods::None> {};
struct Gs : make_tone_ident<8, mods::Sharp> {};
struct Gb : make_tone_ident<6, mods::Flat> {};
struct A : make_tone_ident<9, mods::None> {};
struct As : make_tone_ident<10, mods::Sharp> {};
struct Ab : make_tone_ident<8, mods::Flat> {};
struct B : make_tone_ident<11, mods::None> {};
struct Bs : make_tone_ident<0, mods::Sharp> {};
struct Bb : make_tone_ident<10, mods::Flat> {};

} // idents

using all_heights_seq = std::make_integer_sequence<unsigned, 12>;


namespace detail {

template<class Ident> struct canonical_ident;
template<class Mod> struct canonical_ident<basic_tone_ident<0, Mod>> { using type = idents::C; };
template<class Mod> struct canonical_ident<basic_tone_ident<1, Mod>> { using type = idents::Cs; };
template<class Mod> struct canonical_ident<basic_tone_ident<2, Mod>> { using type = idents::D; };
template<class Mod> struct canonical_ident<basic_tone_ident<3, Mod>> { using type = idents::Ds; };
template<class Mod> struct canonical_ident<basic_tone_ident<4, Mod>> { using type = idents::E; };
template<class Mod> struct canonical_ident<basic_tone_ident<5, Mod>> { using type = idents::F; };
template<class Mod> struct canonical_ident<basic_tone_ident<6, Mod>> { using type = idents::Fs; };
template<class Mod> struct canonical_ident<basic_tone_ident<7, Mod>> { using type = idents::G; };
template<class Mod> struct canonical_ident<basic_tone_ident<8, Mod>> { using type = idents::Gs; };
template<class Mod> struct canonical_ident<basic_tone_ident<9, Mod>> { using type = idents::A; };
template<class Mod> struct canonical_ident<basic_tone_ident<10, Mod>> { using type = idents::As; };
template<class Mod> struct canonical_ident<basic_tone_ident<11, Mod>> { using type = idents::B; };

template<class Ident>
using canonical_ident_t = typename canonical_ident<Ident>::type;


template<class Ident> struct sharp_counter : std::integral_constant<unsigned, 0> {};
template<unsigned Root> struct sharp_counter<make_tone_ident<Root, mods::Sharp>> : std::integral_constant<unsigned, 1> {};
template<unsigned Root> struct sharp_counter<make_tone_ident<Root, mods::DblSharp>> : std::integral_constant<unsigned, 2> {};

template<class Ident> struct flat_counter : std::integral_constant<unsigned, 0> {};
template<unsigned Root> struct flat_counter<make_tone_ident<Root, mods::Flat>> : std::integral_constant<unsigned, 1> {};
template<unsigned Root> struct flat_counter<make_tone_ident<Root, mods::DblFlat>> : std::integral_constant<unsigned, 2> {};

} // detail

}} // ompu
