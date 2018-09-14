#pragma once

// Helpers for the JUCE library :(

#include "ompu/plug/error.hpp"
#include "ompu/component_id.hpp"

#include "JuceHeader.h"

#include <boost/range/iterator_range.hpp>
#include <boost/core/ignore_unused.hpp>


namespace ompu { namespace plug {

inline constexpr juce::uint8 operator""
_u8(unsigned long long v) noexcept
{
    return static_cast<juce::uint8>(v);
}

template<class T>
inline auto make_range(T& v) // must be a valid lvalue, because we are going to make a temporary
{
    using std::begin;
    using std::end;

    return boost::make_iterator_range(begin(v), end(v));
}

}} // ompu
