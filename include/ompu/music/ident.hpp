#pragma once

#include "ompu/music/mod.hpp"

#include <tuple>
#include <type_traits>


namespace ompu { namespace music {

namespace detail {

template<unsigned Height, class Mod>
struct calc_tone_height;

template<unsigned Height>
struct calc_tone_height<Height, mods::None> : std::integral_constant<unsigned, Height> {};
template<unsigned Height>
struct calc_tone_height<Height, mods::Sharp> : std::integral_constant<unsigned, Height + 1> {};
template<unsigned Height>
struct calc_tone_height<Height, mods::Flat> : std::integral_constant<unsigned, Height - 1> {};
template<unsigned Height>
struct calc_tone_height<Height, mods::DblSharp> : std::integral_constant<unsigned, Height + 2> {};
template<unsigned Height>
struct calc_tone_height<Height, mods::DblFlat> : std::integral_constant<unsigned, Height - 2> {};
template<unsigned Height>
struct calc_tone_height<Height, mods::Natural> : std::integral_constant<unsigned, Height> {};


template<unsigned Height, class Mod>
struct basic_tone_ident : std::integral_constant<unsigned, detail::calc_tone_height<Height, Mod>::value>
{
    using mod_type = Mod;
    static constexpr unsigned root = Height;
    static constexpr bool sharped = std::is_same_v<Mod, mods::Sharp>;
    static constexpr bool flatted = std::is_same_v<Mod, mods::Flat>;

    static_assert(Height < 12, "tone_ident: Height height must be less than 12");
};

} // detail

template<unsigned Height, class Mod>
using tone_ident_t = detail::basic_tone_ident<Height % 12, Mod>;


namespace idents {

struct C : tone_ident_t<0, mods::None> {};
struct Cs : tone_ident_t<1, mods::Sharp> {};
struct Cb : tone_ident_t<11, mods::Flat> {};
struct D : tone_ident_t<2, mods::None> {};
struct Ds : tone_ident_t<3, mods::Sharp> {};
struct Db : tone_ident_t<1, mods::Flat> {};
struct E : tone_ident_t<4, mods::None> {};
struct Es : tone_ident_t<5, mods::Sharp> {};
struct Eb : tone_ident_t<3, mods::Flat> {};
struct F : tone_ident_t<5, mods::None> {};
struct Fs : tone_ident_t<6, mods::Sharp> {};
struct Fb : tone_ident_t<4, mods::Flat> {};
struct G : tone_ident_t<7, mods::None> {};
struct Gs : tone_ident_t<8, mods::Sharp> {};
struct Gb : tone_ident_t<6, mods::Flat> {};
struct A : tone_ident_t<9, mods::None> {};
struct As : tone_ident_t<10, mods::Sharp> {};
struct Ab : tone_ident_t<8, mods::Flat> {};
struct B : tone_ident_t<11, mods::None> {};
struct Bs : tone_ident_t<0, mods::Sharp> {};
struct Bb : tone_ident_t<10, mods::Flat> {};

} // idents

using all_heights_seq = std::make_integer_sequence<unsigned, 12>;


namespace detail {

template<class Ident> struct canonical_ident;
template<class Mod> struct canonical_ident<tone_ident_t<0, Mod>> { using type = idents::C; };
template<class Mod> struct canonical_ident<tone_ident_t<1, Mod>> { using type = idents::Cs; };
template<class Mod> struct canonical_ident<tone_ident_t<2, Mod>> { using type = idents::D; };
template<class Mod> struct canonical_ident<tone_ident_t<3, Mod>> { using type = idents::Ds; };
template<class Mod> struct canonical_ident<tone_ident_t<4, Mod>> { using type = idents::E; };
template<class Mod> struct canonical_ident<tone_ident_t<5, Mod>> { using type = idents::F; };
template<class Mod> struct canonical_ident<tone_ident_t<6, Mod>> { using type = idents::Fs; };
template<class Mod> struct canonical_ident<tone_ident_t<7, Mod>> { using type = idents::G; };
template<class Mod> struct canonical_ident<tone_ident_t<8, Mod>> { using type = idents::Gs; };
template<class Mod> struct canonical_ident<tone_ident_t<9, Mod>> { using type = idents::A; };
template<class Mod> struct canonical_ident<tone_ident_t<10, Mod>> { using type = idents::As; };
template<class Mod> struct canonical_ident<tone_ident_t<11, Mod>> { using type = idents::B; };

template<class Ident>
using canonical_ident_t = typename canonical_ident<Ident>::type;


template<class Ident> struct sharp_counter : std::integral_constant<unsigned, 0> {};
template<unsigned Height> struct sharp_counter<tone_ident_t<Height, mods::Sharp>> : std::integral_constant<unsigned, 1> {};
template<unsigned Height> struct sharp_counter<tone_ident_t<Height, mods::DblSharp>> : std::integral_constant<unsigned, 2> {};

template<class Ident> struct flat_counter : std::integral_constant<unsigned, 0> {};
template<unsigned Height> struct flat_counter<tone_ident_t<Height, mods::Flat>> : std::integral_constant<unsigned, 1> {};
template<unsigned Height> struct flat_counter<tone_ident_t<Height, mods::DblFlat>> : std::integral_constant<unsigned, 2> {};

} // detail


namespace detail {

template<class Ident>
struct add_sharp;

template<unsigned Height>
struct add_sharp<detail::basic_tone_ident<Height, mods::None>>
{
    using type = tone_ident_t<Height + 1, mods::Sharp>;
};
template<unsigned Height>
struct add_sharp<detail::basic_tone_ident<Height, mods::Sharp>>
{
    using type = tone_ident_t<Height + 1, mods::DblSharp>;
};
template<unsigned Height>
struct add_sharp<detail::basic_tone_ident<Height, mods::Flat>>
{
    using type = tone_ident_t<Height + 1, mods::Natural>;
};
template<class Ident>
using add_sharp_t = typename add_sharp<Ident>::type;


template<class Ident>
struct add_flat;

template<unsigned Height>
struct add_flat<detail::basic_tone_ident<Height, mods::None>>
{
    using type = tone_ident_t<Height - 1, mods::Flat>;
};
template<unsigned Height>
struct add_flat<detail::basic_tone_ident<Height, mods::Sharp>>
{
    using type = tone_ident_t<Height - 1, mods::Natural>;
};
template<unsigned Height>
struct add_flat<detail::basic_tone_ident<Height, mods::Flat>>
{
    using type = tone_ident_t<Height - 1, mods::DblFlat>;
};
template<class Ident>
using add_flat_t = typename add_flat<Ident>::type;

} // detail


namespace detail {

template<class Ident, class Mod>
struct modded_ident;

template<class Ident>
struct modded_ident<Ident, mods::None>
{
    using type = Ident;
};

template<class Ident>
struct modded_ident<Ident, mods::Sharp>
{
    using type = detail::add_sharp_t<Ident>;
};

template<class Ident>
struct modded_ident<Ident, mods::Flat>
{
    using type = detail::add_flat_t<Ident>;
};

template<class Ident, class Mod>
using modded_ident_t = typename modded_ident<Ident, Mod>::type;

} // detail

}} // ompu
