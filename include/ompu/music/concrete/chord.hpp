#pragma once

#include "ompu/music/concrete/types.hpp"
#include "ompu/music/concrete/note.hpp"

#include <vector>
#include <algorithm>


namespace ompu { namespace music { namespace concrete {

class Chord
{
public:
    Chord() = default;

    template<class Notes>
    explicit Chord(Notes const& notes)
    {
        std::copy(std::begin(notes), std::end(notes), std::back_inserter(notes_));
    }

    Chord(Chord const&) = default;
    Chord(Chord&&) noexcept = default;

    ~Chord() = default;

    std::size_t note_count() const noexcept { return notes_.size(); }

private:
    std::vector<Note> notes_;
};

}}} // ompu
