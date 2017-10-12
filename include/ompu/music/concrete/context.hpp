#pragma once

#include "ompu/music/music_fwd.hpp"

#include "ompu/music/concrete/note.hpp"
#include "ompu/music/concrete/chord.hpp"
#include "ompu/music/concrete/key.hpp"

#include <boost/variant.hpp>

#include <vector>
#include <set>
#include <type_traits>
#include <stdexcept>


namespace ompu { namespace music { namespace concrete {

namespace detail {

class ContextKeyboard
{
public:
    using index_type = std::make_signed_t<octave_type>;

    std::set<Note> const& notes() const noexcept { return notes_; }
    bool empty() const noexcept;

    void clear();

    void on(Note note) noexcept;
    void off(Note note) noexcept;

private:
    std::set<Note> notes_;
};

} // detail


struct context_error : std::runtime_error
{
    explicit context_error(std::string reason) : std::runtime_error(std::move(reason)) {}
};

class Context
{
public:
    Context();
    ~Context() = default;

    void set(Key key) noexcept;

    void add(Note note) noexcept;
    void remove(Note note) noexcept;

    void add_or_remove(bool is_add, Note note) noexcept;

    void clear_notes();

    std::vector<Chord>
    get_closest_chords(std::size_t limit = 3) const;

private:
    Key key_;
    detail::ContextKeyboard kb_;
};

}}} // ompu
