#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/key_feel.hpp"
#include "ompu/music/key_ident.hpp"
#include "ompu/music/key_sign.hpp"
#include "ompu/music/scale.hpp"


namespace ompu { namespace music {

template<class KeyIdent>
struct key_traits;

template<class Ident, class KeyFeel>
struct key_traits<key_ident<Ident, KeyFeel>>
{
    using ident_type = Ident;
    using key_ident_type = key_ident<Ident, KeyFeel>;

    using key_feel = KeyFeel;
    using key_scale_type = scales::make_natural<key_ident_type, KeyFeel>;

    using key_sign_type = basic_key_sign<key_ident_type>;

    static constexpr bool is_major_key = is_major_feel_v<KeyFeel>;
    static constexpr bool is_minor_key = is_minor_feel_v<KeyFeel>;
};

}} // ompu
