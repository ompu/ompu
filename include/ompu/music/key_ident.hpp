#pragma once

#include "ompu/music/ident.hpp"
#include "ompu/music/key_feel.hpp"


namespace ompu { namespace music {

template<class Ident, class KeyFeel>
struct key_ident
{
    using ident_type = Ident;
    using tone_type = typename Ident::tone_type;
    using height_type = typename Ident::height_type;
    using key_feel = KeyFeel;

    static constexpr auto height = Ident::height;
};

}} // ompu
