#pragma once

#include "ompu/music/concrete/types.hpp"
#include "ompu/music/concrete/note.hpp"

#include <tuple>
#include <array>
#include <vector>
#include <set>
#include <algorithm>


namespace ompu { namespace music { namespace concrete {

struct invalid_chord_error : concrete_error
{
    using concrete_error::concrete_error;
};

struct too_many_notes : invalid_chord_error
{
    using invalid_chord_error::invalid_chord_error;
    too_many_notes() : invalid_chord_error("too many notes") {}
};

struct too_many_tensions : too_many_notes
{
    too_many_tensions() : too_many_notes("too many tension notes") {}
};

struct disharmony_error : invalid_chord_error
{
    using invalid_chord_error::invalid_chord_error;
    disharmony_error() : invalid_chord_error("specified notes yields inevitable disharmony") {}
};


class Chord
{
public:
    // using tensions_type = std::array<Note const*, 3>;
    using tensions_type = std::vector<Note const*>;

    Chord() = default;

    explicit Chord(Note root_note) noexcept;

    Chord(Chord const&) = default;
    Chord(Chord&&) noexcept = default;

    ~Chord() = default;

    Chord& operator=(Chord const&) = default;
    Chord& operator=(Chord&&) noexcept = default;

    void set_root(Note root_note) noexcept;
    void auto_set_root() noexcept;
    void add(Note note);

    bool is_power_chord() const noexcept;
    bool is_triad() const noexcept;
    bool is_tetrad() const noexcept;

    bool is_on_chord() const noexcept;

    std::vector<Note> valid_notes() const;

    Note const& note_1() const noexcept;
    Note const& note_3() const noexcept;
    Note const& note_5() const noexcept;
    Note const& note_6() const noexcept;
    Note const& note_7() const noexcept;

    tensions_type const& tensions() const noexcept { return tensions_; }

private:
    void reorganize();

    struct NoteTagger
    {
        using pointer_type = Note const*;
        pointer_type cn1{nullptr}, cn3{nullptr}, cn5{nullptr}, cn6{nullptr}, cn7{nullptr};
    };

    struct DynamicTagger
    {
        using pointer_type = NoteTagger::pointer_type;
        pointer_type cns[12];
    };

    std::set<Note> notes_;
    tensions_type tensions_;

    NoteTagger tag_;
};

}}} // ompu
