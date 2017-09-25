#pragma once

#include "ompu/music/traits.hpp"
#include "ompu/music/note.hpp"
#include "ompu/music/key.hpp"
#include "ompu/music/scale.hpp"


#include <array>
#include <utility>


namespace ompu { namespace music {

namespace factory_detail {

template<class Traits_, unsigned N, unsigned... Rest>
struct notes_impl
{
    static auto const& value() { return notes_impl<Traits_, N - 1, N, Rest...>::value(); }
};

template<class Traits_, unsigned... Rest>
struct notes_impl<Traits_, 0, Rest...>
{
    static auto const& value()
    {
        static Note<Traits_> const instance[] = {
            Note<Traits_>{0}, Note<Traits_>{Rest}...
        };
        return instance;
    }
};

} // factory_detail

template<
    class Traits_ = Traits<440>,
    unsigned NoteCount = 127
>
class Factory
{
public:
    Factory() = default;

    auto const* get(notes::MIDI const&, unsigned idx)
    {
        if (!(idx < NoteCount)) {
            throw std::invalid_argument("Note index exceeded");
        }
        return &notes()[idx];
    }

//private:
    static auto const& notes()
    {
        return factory_detail::notes_impl<Traits_, NoteCount>::value();
    }
};

}} // ompu
