#pragma once

#include <cmath>

namespace ompu { namespace music {

template<unsigned A4_Freq = 440>
class Traits
{
public:
    static constexpr unsigned const A4_Freq = A4_Freq;
};

}} // ompu
