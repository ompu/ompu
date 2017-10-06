#pragma once

#include "ompu/music/chord.hpp"
#include "ompu/music/degree.hpp"


namespace ompu { namespace music {

template<class Degree, class Chord>
struct degreed_chord
{
    using degree_type = Degree;
    using chord_type = Chord;
};

}} // ompu
