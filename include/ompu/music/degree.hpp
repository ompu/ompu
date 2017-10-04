#pragma once

#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace detail {

template<ident_height_type Offset>
struct degree_names;

template<> struct degree_names<0> { static constexpr auto name = sprout::to_string("I"); };
template<> struct degree_names<1> { static constexpr auto name = symbol_names::flat + sprout::to_string("II"); };
template<> struct degree_names<2> { static constexpr auto name = sprout::to_string("II"); };
template<> struct degree_names<3> { static constexpr auto name = symbol_names::flat + sprout::to_string("III"); };
template<> struct degree_names<4> { static constexpr auto name = sprout::to_string("III"); };
template<> struct degree_names<5> { static constexpr auto name = sprout::to_string("IV"); };
template<> struct degree_names<6> { static constexpr auto name = symbol_names::sharp + sprout::to_string("IV"); };
template<> struct degree_names<7> { static constexpr auto name = sprout::to_string("V"); };
template<> struct degree_names<8> { static constexpr auto name = symbol_names::flat + sprout::to_string("VI"); };
template<> struct degree_names<9> { static constexpr auto name = sprout::to_string("VI"); };
template<> struct degree_names<10> { static constexpr auto name = symbol_names::flat + sprout::to_string("VII"); };
template<> struct degree_names<11> { static constexpr auto name = sprout::to_string("VII"); };

} // detail


template<ident_height_type Offset>
struct basic_degree
{
    static constexpr auto offset = Offset;
    static constexpr auto name = detail::degree_names<Offset>::name;
};

}} // ompu
