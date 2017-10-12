#pragma once

#include "ompu/music/concrete/types.hpp"


namespace ompu { namespace music { namespace concrete {

class Interval
{
public:
    Interval() noexcept = default;
    Interval(Interval const&) = default;
    Interval(Interval&&) noexcept = default;

    explicit Interval(std::size_t semitones) noexcept;

    ~Interval() noexcept = default;

    Interval& operator=(Interval const&) = default;
    Interval& operator=(Interval&&) noexcept = default;


    std::size_t semitones() const noexcept { return semitones_; }

private:
    std::size_t semitones_;
};

Interval make_interval(note_height_type a, note_height_type b);


}}} // ompu
