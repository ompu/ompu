#pragma once

#include "ompu/music/key_ident.hpp"
#include "ompu/music/key_feel.hpp"
#include "ompu/music/key_traits.hpp"
#include "ompu/music/scale.hpp"
#include "ompu/music/chord.hpp"
#include "ompu/music/diatonic.hpp"
#include "ompu/music/type_traits.hpp"


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

    using diatonic_chords_tetrad = saya::zed::meta_t<
        diatonic_chords_tetrad_helper,
        saya::zed::template_<key_feel, saya::zed::meta_arg>,
        saya::zed::lazy_unwrap_t<std::tuple, all_diatonic_degrees>
    >;
    using diatonic_chords_triad = saya::zed::meta_t<
        diatonic_chords_triad_helper,
        saya::zed::template_<key_feel, saya::zed::meta_arg>,
        saya::zed::lazy_unwrap_t<std::tuple, all_diatonic_degrees>
    >;

    static constexpr auto height = ident_type::height;
    static constexpr auto name = ident_type::name + sprout::to_string(" ") + key_feel::name;
};

}} // ompu
