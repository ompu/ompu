#pragma once

#include "ompu/music/concrete/types.hpp"
#include "ompu/music/concrete/interval.hpp"

#include "ompu/midi/midi_fwd.hpp"

#include <boost/operators.hpp>


namespace ompu { namespace music { namespace concrete {

class Note : boost::partially_ordered<Note>
{
public:
    Note() noexcept = default;

    explicit Note(note_height_type nh, octave_type oct) noexcept;
    explicit Note(CanonicalNoteHeight nh, ModType mod, octave_type oct) noexcept;
    explicit Note(midi::Message const& mes) noexcept;

    Note(Note const&) = default;
    Note(Note&&) noexcept = default;
    ~Note() noexcept = default;

    Note& operator=(Note const&) = default;
    Note& operator=(Note&&) noexcept = default;

    note_height_type height() const noexcept { return nh_; }
    octave_type octave() const noexcept { return oct_; }

    bool operator==(Note const& rhs) const noexcept;
    bool operator<(Note const& rhs) const noexcept;
    bool operator<=(Note const& rhs) const noexcept;

private:
    note_height_type nh_;
    octave_type oct_;
};

std::ostream& operator<<(std::ostream& os, Note const& note);

}}} // ompu
