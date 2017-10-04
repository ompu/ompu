#pragma once

#include "ompu/music/ident.hpp"
#include "ompu/music/key_feel.hpp"


namespace ompu { namespace music {

template<class Ident, class KeyFeel>
struct key_ident
{
    using ident_type = Ident;
    using key_feel = KeyFeel;
};

}} // ompu
