#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/key_ident.hpp"
#include "ompu/music/key_traits.hpp"
#include "ompu/music/scale.hpp"


namespace ompu { namespace music {

template<class KeyIdent>
struct basic_key
{
    using traits_type = key_traits<KeyIdent>;
    using ident_type = typename traits_type::ident_type;
    using key_ident_type = typename traits_type::key_ident_type;
    using key_feel = typename traits_type::key_feel;
    using key_scale_type = typename traits_type::key_scale_type;
    using key_sign_type = typename traits_type::key_sign_type;

    using known_scale_roles = std::tuple<
        scales::natural<key_feel>,
        scales::harmonic<key_feel>,
        scales::melodic<key_feel>
    >;

    static constexpr auto name = ident_type::name + sprout::to_string(" ") + key_feel::name;
};

}} // ompu
