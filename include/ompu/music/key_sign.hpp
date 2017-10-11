#pragma once

#include "ompu/music/tone.hpp"
#include "ompu/music/ident.hpp"
#include "ompu/music/key_ident.hpp"
#include "ompu/music/scale_def.hpp"
#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {


namespace detail {

template<class KeyTone, class KeyMod>
struct key_mod_count
{
    static constexpr auto N = key_mod_count<
        cvt::detail::canonical_tone_shift_t<KeyTone, tone_offset<7 * KeyMod::offset>>,
        KeyMod
    >::N + 1;
};

template<class KeyTone>
struct key_mod_count<
    KeyTone, mods::none
>
{
    static constexpr std::size_t N = 0;
};

template<class KeyMod>
struct key_mod_count<tones::C, KeyMod>
{
    static constexpr std::size_t N = 0;
};



template<class ToneHeight> struct key_mod_set_height_to_index_impl;
template<> struct key_mod_set_height_to_index_impl<tone_height<0>> : std::integral_constant<std::size_t, 0> {};
template<> struct key_mod_set_height_to_index_impl<tone_height<2>> : std::integral_constant<std::size_t, 1> {};
template<> struct key_mod_set_height_to_index_impl<tone_height<4>> : std::integral_constant<std::size_t, 2> {};
template<> struct key_mod_set_height_to_index_impl<tone_height<5>> : std::integral_constant<std::size_t, 3> {};
template<> struct key_mod_set_height_to_index_impl<tone_height<7>> : std::integral_constant<std::size_t, 4> {};
template<> struct key_mod_set_height_to_index_impl<tone_height<9>> : std::integral_constant<std::size_t, 5> {};
template<> struct key_mod_set_height_to_index_impl<tone_height<11>> : std::integral_constant<std::size_t, 6> {};

template<class KeyIdent, class KeyModdedAs>
struct key_mod_set_impl;


template<>
struct key_mod_set_impl<
    key_ident<
        basic_ident<tones::C, mods::none>,
        key_feels::major
    >,
    mods::none
>
{
    using type = mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::none, mods::none>;
};

// ^^^^^^^^^^ non-modded keys ^^^^^^^^^^^^
// vvvvvvvvvv sharped keys vvvvvvvvvvvvvvv

template<>
struct key_mod_set_impl<key_ident<
    basic_ident<tones::G, mods::none>,
    key_feels::major
>, mods::sharp>
{
    using type = mod_set<mods::none, mods::none, mods::none, mods::sharp, mods::none, mods::none, mods::none>;
};

template<class Tone, class Mod>
struct key_mod_set_impl<key_ident<
    basic_ident<Tone, Mod>,
    key_feels::major
>, mods::sharp>
{
    using type = saya::zed::element_assign_t<
        mod_set,
        typename key_mod_set_impl<
            key_ident<
                // e.g. for key=D, respect key mods on key=G (i.e. offseted -7)
                cvt::detail::remod_if_modded_t<
                    cvt::canonical_ident_t<basic_ident<cvt::detail::canonical_tone_shift_t<Tone, tone_offset<-7>>, mods::none>>,
                    mods::sharp
                >,
                key_feels::major
            >,
            mods::sharp
        >::type,

        // e.g. for key=D, add an another sharp for note C (i.e. offseted -2)
        key_mod_set_height_to_index_impl<
            typename cvt::detail::canonical_tone_shift_t<Tone, tone_offset<-2>>::height_type
        >::value,
        mods::sharp
    >;
};

// ^^^^^^^^^^ sharped keys ^^^^^^^^^^^^^^^
// vvvvvvvvvv flatted keys vvvvvvvvvvvvvvv

template<>
struct key_mod_set_impl<key_ident<
    basic_ident<tones::F, mods::none>,
    key_feels::major
>, mods::flat>
{
    using type = mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::none, mods::flat>;
};


template<class Tone, class Mod>
struct key_mod_set_impl<key_ident<
    basic_ident<Tone, Mod>,
    key_feels::major
>, mods::flat>
{
    using type = saya::zed::element_assign_t<
        mod_set,
        typename key_mod_set_impl<
            key_ident<
                // e.g. for key=Bb, respect key mods on key=F (i.e. offseted -5)
                cvt::detail::remod_if_modded_t<
                    cvt::canonical_ident_t<basic_ident<cvt::detail::canonical_tone_shift_t<Tone, tone_offset<-5>>, mods::none>>,
                    mods::flat
                >,
                key_feels::major
            >,
            mods::flat
        >::type,

        // e.g. for key=Bb, add an another flat for note E (i.e. offseted +6)
        key_mod_set_height_to_index_impl<
            typename cvt::detail::canonical_tone_shift_t<Tone, tone_offset<+6>>::height_type
        >::value,
        mods::flat
    >;
};

template<class Ident, class KeyModdedAs>
struct key_mod_set_impl<key_ident<
    Ident,
    key_feels::minor
>, KeyModdedAs>
{
    using type = typename key_mod_set_impl<
        cvt::detail::relative_key_ident_t<key_ident<Ident, key_feels::minor>>, KeyModdedAs
    >::type;
};

template<class KeyIdent, class KeyModdedAs>
using key_mod_set_impl_t = typename key_mod_set_impl<KeyIdent, KeyModdedAs>::type;

} // detail


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
    using key_mod_set = detail::key_mod_set_impl_t<
#if 0
        key_ident<
            cvt::detail::remod_if_modded_t<
                cvt::canonical_ident_t<Ident>,
                key_sign_mod_t<key_ident<cvt::canonical_ident_t<Ident>, KeyFeel>>
            >,
            KeyFeel
        >,
#endif
        key_ident_type,
        key_sign_mod_t<key_ident_type>
    >;

    static constexpr auto mod_count = key_mod_set::modded_count;
    static constexpr auto const& symbol = key_sign_symbol<mod_type, mod_count>::name;
};

}} // ompu
