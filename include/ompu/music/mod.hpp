#pragma once

#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace mods {

struct none : std::integral_constant<int, 0> { static constexpr auto const& name = detail::symbol_names::empty; };
struct sharp : std::integral_constant<int, 1> { static constexpr auto const& name = detail::symbol_names::sharp; };
struct flat : std::integral_constant<int, -1> { static constexpr auto const& name = detail::symbol_names::flat; };

struct dbl_sharp : std::integral_constant<int, 2> { static constexpr auto const name = detail::symbol_names::sharp + detail::symbol_names::sharp; };
struct dbl_flat : std::integral_constant<int, -2> { static constexpr auto const name = detail::symbol_names::flat + detail::symbol_names::flat; };

struct natural : std::integral_constant<int, 0> { static constexpr auto const& name = detail::symbol_names::natural; };

} // mods


template<class... Mods>
struct mod_set
{
    static_assert(
        saya::zed::all_of_v<
            std::is_integral,
            saya::zed::template_<>,
            Mods...
        >,
        "all mods must be an integral type"
    );
    static constexpr std::size_t count = sizeof...(Mods);
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
