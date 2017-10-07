#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/tone.hpp"
#include "ompu/music/ident.hpp"
#include "ompu/music/key_ident.hpp"
#include "ompu/music/scale_def.hpp"


namespace ompu { namespace music {

template<class Mod, std::size_t Count>
struct key_sign_symbol
{
    static constexpr auto const name = Mod::name + key_sign_symbol<Mod, Count - 1>::name;
};

template<class Mod>
struct key_sign_symbol<Mod, 1>
{
    static constexpr auto const& name = Mod::name;
};

template<class Mod>
struct key_sign_symbol<Mod, 0>
{
    static constexpr auto const& name = detail::symbol_names::empty;
};


template<class KeyIdent>
struct basic_key_sign;

template<class Ident, class KeyFeel>
struct basic_key_sign<key_ident<Ident, KeyFeel>>
{
    using key_ident_type = key_ident<Ident, KeyFeel>;
    using mod_type = key_sign_mod_t<key_ident_type>;
    static constexpr auto assoc_mods =
        off_scaled_tone_count<
            std::conditional_t<
                std::is_same<KeyFeel, key_feels::major>::value,
                key_natural_tones<key_ident<idents::C, key_feels::major>>,
                key_natural_tones<key_ident<idents::A, key_feels::minor>>
            >,
            key_natural_tones_t<key_ident<Ident, KeyFeel>>
        >::value
    ;
    static constexpr auto const& symbol = key_sign_symbol<mod_type, assoc_mods>::name;
};

}} // ompu
