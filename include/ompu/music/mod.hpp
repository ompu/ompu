#pragma once

#include "ompu/music/tone_types.hpp"
#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace mods {

struct none
{
    using offset_type = tone_offset<0>;
    static constexpr auto offset = offset_type::value;
    static constexpr auto const& name = detail::symbol_names::empty;
};
struct sharp
{
    using offset_type = tone_offset<1>;
    static constexpr auto offset = offset_type::value;
    static constexpr auto const& name = detail::symbol_names::sharp;
};
struct flat
{
    using offset_type = tone_offset<-1>;
    static constexpr auto offset = offset_type::value;
    static constexpr auto const& name = detail::symbol_names::flat;
};

struct dbl_sharp
{
    using offset_type = tone_offset<2>;
    static constexpr auto offset = offset_type::value;
    static constexpr auto const name = detail::symbol_names::sharp + detail::symbol_names::sharp;
};
struct dbl_flat
{
    using offset_type = tone_offset<-2>;
    static constexpr auto offset = offset_type::value;
    static constexpr auto const name = detail::symbol_names::flat + detail::symbol_names::flat;
};

struct natural
{
    using offset_type = tone_offset<0>;
    static constexpr auto offset = offset_type::value;
    static constexpr auto const& name = detail::symbol_names::natural;
};

} // mods


template<class... Mods>
struct mod_set
{
    static_assert(
        saya::zed::all_of_v<
            std::is_integral,
            saya::zed::template_<>,
            decltype(Mods::offset)...
        >,
        "all mods must have an integral offset value"
    );

    static constexpr std::size_t count = sizeof...(Mods);
    static constexpr std::size_t modded_count = saya::zed::fold_add_v<
        std::size_t, 0,
        std::conditional_t<std::is_same_v<Mods, mods::none>, std::integral_constant<std::size_t, 0>, std::integral_constant<std::size_t, 1>>::value...
    >;
};



namespace detail {

template<class Mod> struct mod_names;
template<> struct mod_names<mods::none> { static constexpr auto const& value = symbol_names::empty; };
template<> struct mod_names<mods::sharp> { static constexpr auto const& value = symbol_names::sharp; };
template<> struct mod_names<mods::flat> { static constexpr auto const& value = symbol_names::flat; };
template<> struct mod_names<mods::dbl_sharp> { static constexpr auto value = symbol_names::sharp + symbol_names::sharp; };
template<> struct mod_names<mods::dbl_flat> { static constexpr auto value = symbol_names::flat + symbol_names::flat; };
template<> struct mod_names<mods::natural> { static constexpr auto const& value = symbol_names::natural; };

} // detail

}} // ompu
