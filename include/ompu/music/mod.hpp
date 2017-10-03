#pragma once

#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace mods {

struct none : std::integral_constant<int, 0> {};
struct sharp : std::integral_constant<int, 1> {};
struct flat : std::integral_constant<int, -1> {};

struct dbl_sharp : std::integral_constant<int, 2> {};
struct dbl_flat : std::integral_constant<int, -2> {};

struct natural : std::integral_constant<int, 0> {};

} // mods

namespace detail {

template<class Mod> struct mod_names;
template<> struct mod_names<mods::none> { static constexpr auto value = sprout::to_string(""); };
template<> struct mod_names<mods::sharp> { static constexpr auto value = sprout::to_string(u8"\u266f"); };
template<> struct mod_names<mods::flat> { static constexpr auto value = sprout::to_string(u8"\u266d"); };
template<> struct mod_names<mods::dbl_sharp> { static constexpr auto value = mod_names<mods::sharp>::value + mod_names<mods::sharp>::value; };
template<> struct mod_names<mods::dbl_flat> { static constexpr auto value = mod_names<mods::flat>::value + mod_names<mods::flat>::value; };
template<> struct mod_names<mods::natural> { static constexpr auto value = sprout::to_string(u8"\u266e"); };

} // detail

}} // ompu
