#include "ompu/music/concrete/interval.hpp"

#include <boost/assert.hpp>


namespace ompu { namespace music { namespace concrete {

Interval::Interval(std::size_t semitones) noexcept
    : semitones_(semitones)
{
    BOOST_ASSERT(semitones <= 12);
}

Interval make_interval(note_height_type a, note_height_type b)
{
    return Interval{(std::max(a, b) - std::min(a, b)) % 12};
}

}}} // ompu
