#include "ompu/music/concrete/note.hpp"

#include "ompu/midi/message.hpp"

#include <iostream>
#include <tuple>


namespace ompu { namespace music { namespace concrete {

Note::Note(CanonicalNoteHeight nh, ModType mod, octave_type oct) noexcept
    : nh_(static_cast<note_height_type>(nh) + mod_value(mod))
    , oct_(oct)
{}

Note::Note(midi::Message const& mes) noexcept
    : nh_(mes.note_num % 12)
    , oct_(mes.note_num / 12)
{}


bool Note::operator==(Note const& rhs) const noexcept
{
    return nh_ == rhs.nh_ && oct_ == rhs.oct_;
}

bool Note::operator<(Note const& rhs) const noexcept
{
    return std::forward_as_tuple(oct_, nh_) < std::forward_as_tuple(rhs.oct_, rhs.nh_);
}

bool Note::operator<=(Note const& rhs) const noexcept
{
    return std::forward_as_tuple(oct_, nh_) <= std::forward_as_tuple(rhs.oct_, rhs.nh_);
}


std::ostream& operator<<(std::ostream& os, Note const& note)
{
    return os << "Note { height: " << note.height() << ", octave: " << note.octave() << " }";
}

}}} // ompu
