#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/key_traits.hpp"
#include "ompu/music/scale.hpp"


namespace ompu { namespace music {

template<class Ident, class KeyFeel>
struct basic_key
{
    using traits_type = key_traits<Ident, KeyFeel>;
    using ident_type = typename traits_type::ident_type;
    using key_feel = typename traits_type::key_feel;
    using key_scale_type = typename traits_type::key_scale_type;
    using key_sign_type = typename traits_type::key_sign_type;

    using known_scale_roles = std::tuple<
        scales::natural<KeyFeel>,
        scales::harmonic<KeyFeel>,
        scales::melodic<KeyFeel>
    >;

    static constexpr auto name = ident_type::name + sprout::to_string(" ") + key_feel::name;
};

}} // ompu
