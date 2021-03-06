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
    disharmony_error() : invalid_chord_error("specified notes yield inevitable disharmony") {}
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

    // std::string str() const;

    bool is_valid() const noexcept;

    bool is_power_chord() const noexcept;
    bool is_triad() const noexcept;
    bool is_tetrad() const noexcept;

    bool is_sixth_chord() const noexcept;
    bool is_seventh_chord() const noexcept;

    bool is_on_chord() const noexcept;

    // other trivial chord traits
    bool is_major() const noexcept;
    bool is_minor() const noexcept;
    bool has_b5() const noexcept;
    bool has_aug5() const noexcept;

    bool has_m7() const noexcept;
    bool has_M7() const noexcept;
    bool has_dim7() const noexcept;

    bool has_tension() const noexcept;
    bool has_add9() const noexcept;


    std::vector<Note> valid_notes() const;

    Note const& bass() const noexcept;
    Note const& note_1() const noexcept;
    Note const& note_3() const noexcept;
    Note const& note_5() const noexcept;
    Note const& note_6() const noexcept;
    Note const& note_7() const noexcept;

    tensions_type const& tensions() const noexcept { return tensions_; }

    friend std::ostream& operator<<(std::ostream& os, Chord const& ch);

private:
    void reorganize();
    char const* the_7_or_9() const noexcept;
    char const* the_tension() const noexcept;
    note_height_type distance_from_root(Note const* to_note) const noexcept;

    struct NoteTagger
    {
        using pointer_type = Note const*;
        pointer_type cn1{nullptr}, cn3{nullptr}, cn5{nullptr}, cn6{nullptr}, cn7{nullptr}, cn9{nullptr};
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
