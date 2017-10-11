#pragma once

#include "ompu/music/mod.hpp"
#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace detail {

template<class CanonicalHeight> struct ident_names;
template<> struct ident_names<tone_height<0>> { static constexpr auto value = sprout::to_string("C"); };
template<> struct ident_names<tone_height<1>> { static constexpr auto value = sprout::to_string("D"); };
template<> struct ident_names<tone_height<2>> { static constexpr auto value = sprout::to_string("E"); };
template<> struct ident_names<tone_height<3>> { static constexpr auto value = sprout::to_string("F"); };
template<> struct ident_names<tone_height<4>> { static constexpr auto value = sprout::to_string("G"); };
template<> struct ident_names<tone_height<5>> { static constexpr auto value = sprout::to_string("A"); };
template<> struct ident_names<tone_height<6>> { static constexpr auto value = sprout::to_string("B"); };

} // detail


template<class Tone, class Mod>
struct ident_traits
{
    static constexpr auto name =
          detail::ident_names<cvt::detail::non_modded_tone_height_t<Tone, Mod>>::value
        + detail::mod_names<Mod>::value
    ;
};


template<class Tone, class Mod>
struct basic_ident
{
    using tone_type = Tone;
    using mod_type = Mod;
    using traits_type = ident_traits<Tone, Mod>;

    using height_type = typename Tone::height_type;
    static constexpr auto height = Tone::height;
    static constexpr auto const& name = traits_type::name;
    static constexpr bool is_sharped = std::is_same_v<Mod, mods::sharp>;
    static constexpr bool is_flatted = std::is_same_v<Mod, mods::flat>;
};

template<class... Idents>
struct ident_set
{
    static constexpr std::size_t count = sizeof...(Idents);
};

}} // ompu
