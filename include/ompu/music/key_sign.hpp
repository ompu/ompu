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
struct key_natural_tones;

template<class Ident>
struct key_natural_tones<
    key_ident<Ident, key_feels::major>
>
{
    using type = cvt::detail::tone_set_shift_t<
        detail::make_tone_set<0, 2, 4, 5, 7, 9, 11>,
        tone_offset<Ident::height>
    >;
};

template<class Ident>
struct key_natural_tones<
    key_ident<Ident, key_feels::minor>
>
{
    using type = cvt::detail::tone_set_shift_t<
        detail::make_tone_set<9, 11, 0, 2, 4, 5, 7>,
        tone_offset<cvt::detail::canonical_tone_shift_t<typename Ident::tone_type, tone_offset<-9>>::height>
    >;
};

template<class KeyIdent>
using key_natural_tones_t = typename key_natural_tones<KeyIdent>::type;


template<class KeyIdent>
struct basic_key_sign;

template<class Ident, class KeyFeel>
struct basic_key_sign<key_ident<Ident, KeyFeel>>
{
    using key_ident_type = key_ident<Ident, KeyFeel>;
    using mod_type = key_sign_mod_t<key_ident_type>;
    static constexpr auto assoc_mods = off_scaled_tone_count<key_ident_type, key_natural_tones_t<key_ident_type>>::value;
    static constexpr auto const& symbol = key_sign_symbol<mod_type, assoc_mods>::name;
};

}} // ompu
