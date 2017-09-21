#pragma once

#include "ompu/midi/midi_fwd.hpp"

#include <array>


namespace ompu { namespace midi {

class Filter
{
public:
    bool is_filtered(Message const& msg) const noexcept;

    std::array<bool, OMPU_MIDI_CH_MAX> channel;

private:
};

class FilterSet
{
public:
    // TODO: implement

private:
};

}} // ompu
