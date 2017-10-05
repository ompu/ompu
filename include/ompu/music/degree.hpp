#pragma once

#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace detail {

template<class RelativeHeight>
struct degree_names;

template<> struct degree_names<relative_tone_height<0> > { static constexpr auto name = sprout::to_string("I"); };
template<> struct degree_names<relative_tone_height<1> > { static constexpr auto name = symbol_names::flat + sprout::to_string("II"); };
template<> struct degree_names<relative_tone_height<2> > { static constexpr auto name = sprout::to_string("II"); };
template<> struct degree_names<relative_tone_height<3> > { static constexpr auto name = symbol_names::flat + sprout::to_string("III"); };
template<> struct degree_names<relative_tone_height<4> > { static constexpr auto name = sprout::to_string("III"); };
template<> struct degree_names<relative_tone_height<5> > { static constexpr auto name = sprout::to_string("IV"); };
template<> struct degree_names<relative_tone_height<6> > { static constexpr auto name = symbol_names::sharp + sprout::to_string("IV"); };
template<> struct degree_names<relative_tone_height<7> > { static constexpr auto name = sprout::to_string("V"); };
template<> struct degree_names<relative_tone_height<8> > { static constexpr auto name = symbol_names::flat + sprout::to_string("VI"); };
template<> struct degree_names<relative_tone_height<9> > { static constexpr auto name = sprout::to_string("VI"); };
template<> struct degree_names<relative_tone_height<10>> { static constexpr auto name = symbol_names::flat + sprout::to_string("VII"); };
template<> struct degree_names<relative_tone_height<11>> { static constexpr auto name = sprout::to_string("VII"); };

} // detail


template<class RelativeHeight>
struct basic_degree
{
    using height_type = RelativeHeight;
    static constexpr auto relative_height = RelativeHeight::value;
    static constexpr auto name = detail::degree_names<RelativeHeight>::name;
};

}} // ompu
