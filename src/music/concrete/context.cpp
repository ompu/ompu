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
    std::vector<Chord> ret;
    ret.reserve(limit);



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
