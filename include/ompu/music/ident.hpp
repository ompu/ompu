#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/mod.hpp"


namespace ompu { namespace music {

namespace detail {

template<ident_height_type CanonicalHeight> struct ident_names;
template<> struct ident_names<0> { static constexpr auto value = sprout::to_string("C"); };
template<> struct ident_names<1> { static constexpr auto value = sprout::to_string("D"); };
template<> struct ident_names<2> { static constexpr auto value = sprout::to_string("E"); };
template<> struct ident_names<3> { static constexpr auto value = sprout::to_string("F"); };
template<> struct ident_names<4> { static constexpr auto value = sprout::to_string("G"); };
template<> struct ident_names<5> { static constexpr auto value = sprout::to_string("A"); };
template<> struct ident_names<6> { static constexpr auto value = sprout::to_string("B"); };

} // detail


template<ident_height_type Height, class Mod>
struct ident_traits
{
    // static constexpr auto real_height = Height;
    static constexpr auto canonical_height = cvt::detail::canonical_ident_height_v<
        Height, Mod
    >;

    static constexpr auto name =
          detail::ident_names<canonical_height>::value
        + detail::mod_names<Mod>::value
    ;
};


template<ident_height_type Height, class Mod>
struct basic_ident
{
    static constexpr auto height = Height;
    using mod_type = Mod;
    using traits_type = ident_traits<Height, Mod>;
    static constexpr auto canonical_height = traits_type::canonical_height;
    static constexpr auto const& name = traits_type::name;
    static constexpr bool sharped = std::is_same_v<Mod, mods::sharp>;
    static constexpr bool flatted = std::is_same_v<Mod, mods::flat>;

    static_assert(Height < 12, "ident: Height height must be less than 12");
};


namespace idents {

struct C : basic_ident<0, mods::none> {};
struct Cs : basic_ident<1, mods::sharp> {};
struct Cb : basic_ident<11, mods::flat> {};
struct D : basic_ident<2, mods::none> {};
struct Ds : basic_ident<3, mods::sharp> {};
struct Db : basic_ident<1, mods::flat> {};
struct E : basic_ident<4, mods::none> {};
struct Es : basic_ident<5, mods::sharp> {};
struct Eb : basic_ident<3, mods::flat> {};
struct F : basic_ident<5, mods::none> {};
struct Fs : basic_ident<6, mods::sharp> {};
struct Fb : basic_ident<4, mods::flat> {};
struct G : basic_ident<7, mods::none> {};
struct Gs : basic_ident<8, mods::sharp> {};
struct Gb : basic_ident<6, mods::flat> {};
struct A : basic_ident<9, mods::none> {};
struct As : basic_ident<10, mods::sharp> {};
struct Ab : basic_ident<8, mods::flat> {};
struct B : basic_ident<11, mods::none> {};
struct Bs : basic_ident<0, mods::sharp> {};
struct Bb : basic_ident<10, mods::flat> {};

} // idents

}} // ompu
