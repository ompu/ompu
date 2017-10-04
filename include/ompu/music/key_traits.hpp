#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/key_feel.hpp"
#include "ompu/music/key_sign.hpp"
#include "ompu/music/scale.hpp"


namespace ompu { namespace music {

template<class Ident, class KeyFeel>
struct key_traits
{
    using ident_type = Ident;

    using key_feel = KeyFeel;
    using key_scale_type = scales::make_natural<Ident, KeyFeel>;

    using key_sign_type = basic_key_sign<Ident, key_feel>;

    static constexpr bool is_major_key = is_major_feel_v<KeyFeel>;
    static constexpr bool is_minor_key = is_minor_feel_v<KeyFeel>;
};

}} // ompu
