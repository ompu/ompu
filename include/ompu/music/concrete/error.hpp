#pragma once

#include "ompu/music/concrete/types.hpp"

#include <stdexcept>


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

}}} // ompu
