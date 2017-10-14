#include "ompu/music/concrete/context.hpp"
#include "ompu/music.hpp"


namespace ompu { namespace music { namespace concrete {

namespace detail {

bool ContextKeyboard::empty() const noexcept
{
    return notes_.empty();
}

void ContextKeyboard::on(Note note) noexcept
{
    notes_.emplace(std::move(note));
}

void ContextKeyboard::off(Note note) noexcept
{
    notes_.erase(note);
}

void ContextKeyboard::clear()
{
    notes_.clear();
}

} // detail


Context::Context()
    : key_(keys::CMaj{})
{}

void Context::set(Key key) noexcept
{
    key_ = std::move(key);
}


std::vector<Chord>
Context::get_closest_chords(std::size_t const limit) const
{
    auto const notes = kb_.notes();

    std::vector<Chord> ret;
    ret.reserve(limit);

    try {
        for (auto possible_root : notes) {
            auto const root_height = possible_root.height();
            Chord chord{std::move(possible_root)};

            for (auto possible_note : notes) {
                if (possible_note.height() == root_height) continue;
                chord.add(std::move(possible_note));
            }

            ret.emplace_back(std::move(chord));
            if (ret.size() >= limit) break;
        }

    } catch (invalid_chord_error const& e) {
        if (logger_) {
            *logger_ << e.what() << std::endl;
        }
    }

    ret.shrink_to_fit();
    return ret;
}


void Context::add(Note note) noexcept
{
    kb_.on(std::move(note));
}

void Context::remove(Note note) noexcept
{
    kb_.off(std::move(note));
}

void Context::add_or_remove(bool is_add, Note note) noexcept
{
    is_add ? kb_.on(std::move(note)) : kb_.off(std::move(note));
}

void Context::clear_notes()
{
    kb_.clear();
}

}}} // ompu
