#pragma once

#include "ompu/music/tone_types.hpp"
#include "ompu/music/mod.hpp"
#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace detail {

template<class DegreeHeight>
struct degree_names;

template<> struct degree_names<degree_height<1>> { static constexpr auto name = sprout::to_string("I"); };
template<> struct degree_names<degree_height<2>> { static constexpr auto name = sprout::to_string("II"); };
template<> struct degree_names<degree_height<3>> { static constexpr auto name = sprout::to_string("III"); };
template<> struct degree_names<degree_height<4>> { static constexpr auto name = sprout::to_string("IV"); };
template<> struct degree_names<degree_height<5>> { static constexpr auto name = sprout::to_string("V"); };
template<> struct degree_names<degree_height<6>> { static constexpr auto name = sprout::to_string("VI"); };
template<> struct degree_names<degree_height<7>> { static constexpr auto name = sprout::to_string("VII"); };

} // detail


template<unsigned N>
struct degree_height
{
    static_assert(1 <= N && N <= 7, "1 <= degree height <= 7; octaves must be canonicalized");
    static constexpr auto N = N;
};

template<class DegreeHeight, class Mod>
struct basic_degree
{
    using height_type = DegreeHeight;
    using mod_type = Mod;
    static constexpr auto N = DegreeHeight::N;
    static constexpr auto name = detail::degree_names<DegreeHeight>::name;
};

}} // ompu
