#pragma once

#include "ompu/music/key_ident.hpp"
#include "ompu/music/key_sign.hpp"
#include "ompu/music/type_traits.hpp"


namespace ompu { namespace music {

namespace detail {

template<class ScaledAs, bool Upper>
struct special_scale_set;

template<class ScaledAs, class HeightSet, class ModSet>
struct scale_def
{
    using scaled_as_type = ScaledAs;
    using height_set_type = HeightSet;
    using mod_set_type = ModSet;
};

template<class ReScaledAs, class ScaleDef, class ModSet>
struct modded_scale_def;

template<class ReScaledAs, class ScaleDef>
struct modded_scale_def<ReScaledAs, ScaleDef, saya::zed::empty_seq<>>
{
    using type = scale_def<ReScaledAs, typename ScaleDef::height_set_type, typename ScaleDef::mod_set_type>;
};

template<class ReScaledAs, class ScaledAs, ident_height_type... Heights, class... Mods, class... AppliedMods>
struct modded_scale_def<ReScaledAs, scale_def<ScaledAs, height_set<Heights...>, mod_set<Mods...>>, mod_set<AppliedMods...>>
{
    using type = scale_def<
        ReScaledAs,
        height_set<cvt::detail::height_shift<Heights, AppliedMods::value>::value...>,
        mod_set<
            std::conditional_t<
                std::is_same<AppliedMods, mods::none>::value,
                Mods, cvt::add_mod_t<Mods, AppliedMods>
            >...
        >
    >;
};

template<class ReScaledAs, class ScaleDef, class ModSet>
using modded_scale_def_t = typename modded_scale_def<ReScaledAs, ScaleDef, ModSet>::type;


template<bool Upper> struct special_scale_set<scales::natural<key_feels::major>, Upper>
{
    using type = scale_def<
        scales::natural<key_feels::major>,
        height_set<0, 2, 4, 5, 7, 9, 11>,
        mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::none, mods::none>
    >;
};
template<bool Upper> struct special_scale_set<scales::harmonic<key_feels::major>, Upper>
{
    using type = modded_scale_def_t<
        scales::harmonic<key_feels::major>,
        typename special_scale_set<scales::natural<key_feels::major>, true>::type,
        mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::flat, mods::none>
    >;
};
template<> struct special_scale_set<scales::melodic<key_feels::major>, true>
{
    using type = modded_scale_def_t<
        scales::melodic<key_feels::major>,
        typename special_scale_set<scales::natural<key_feels::major>, true>::type,
        saya::zed::empty_seq<>
    >;
};
template<> struct special_scale_set<scales::melodic<key_feels::major>, false>
{
    using type = modded_scale_def_t<
        scales::melodic<key_feels::major>,
        typename special_scale_set<scales::natural<key_feels::major>, true>::type,
        mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::flat, mods::flat>
    >;
};

// -----------------------------------------

template<bool Upper> struct special_scale_set<scales::natural<key_feels::minor>, Upper>
{
    using type = scale_def<
        scales::natural<key_feels::minor>,
        height_set<9, 11, 0, 2, 4, 5, 7>,
        mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::none, mods::none>
    >;
};
template<bool Upper> struct special_scale_set<scales::harmonic<key_feels::minor>, Upper>
{
    using type = modded_scale_def_t<
        scales::harmonic<key_feels::minor>,
        typename special_scale_set<scales::natural<key_feels::minor>, true>::type,
        mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::sharp, mods::none>
    >;
};
template<> struct special_scale_set<scales::melodic<key_feels::minor>, true>
{
    using type = modded_scale_def_t<
        scales::melodic<key_feels::minor>,
        typename special_scale_set<scales::natural<key_feels::minor>, true>::type,
        mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::sharp, mods::sharp>
    >;
};
template<> struct special_scale_set<scales::melodic<key_feels::minor>, false>
{
    using type = modded_scale_def_t<
        scales::melodic<key_feels::minor>,
        typename special_scale_set<scales::natural<key_feels::minor>, true>::type,
        saya::zed::empty_seq<>
    >;
};

// ----------------------------------------------------------

template<bool Upper> struct special_scale_set<scales::ionian, Upper>
{
    using type = scale_def<
        scales::ionian,
        height_set<0, 2, 4, 5, 7, 9, 11>,
        mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::none, mods::none>
    >;
};
template<bool Upper> struct special_scale_set<scales::dorian, Upper>
{
    using type = scale_def<
        scales::dorian,
        height_set<0, 2, 3, 5, 7, 9, 10>,
        mod_set<mods::none, mods::none, mods::flat, mods::none, mods::none, mods::none, mods::flat>
    >;
};
template<bool Upper> struct special_scale_set<scales::phrigian, Upper>
{
    using type = scale_def<
        scales::phrigian,
        height_set<0, 1, 3, 5, 7, 8, 10>,
        mod_set<mods::none, mods::flat, mods::flat, mods::none, mods::none, mods::flat, mods::flat>
    >;
};
template<bool Upper> struct special_scale_set<scales::lydian, Upper>
{
    using type = scale_def<
        scales::lydian,
        height_set<0, 2, 4, 6, 7, 9, 11>,
        mod_set<mods::none, mods::none, mods::none, mods::sharp, mods::none, mods::none, mods::none>
    >;
};
template<bool Upper> struct special_scale_set<scales::mixolydian, Upper>
{
    using type = scale_def<
        scales::mixolydian,
        height_set<0, 2, 4, 5, 7, 9, 10>,
        mod_set<mods::none, mods::none, mods::none, mods::none, mods::none, mods::none, mods::flat>
    >;
};
template<bool Upper> struct special_scale_set<scales::aeolian, Upper>
{
    using type = scale_def<
        scales::aeolian,
        height_set<0, 2, 3, 5, 7, 8, 10>,
        mod_set<mods::none, mods::none, mods::flat, mods::none, mods::none, mods::flat, mods::flat>
    >;
};
template<bool Upper> struct special_scale_set<scales::locrian, Upper>
{
    using type = scale_def<
        scales::locrian,
        height_set<0, 1, 3, 5, 6, 8, 10>,
        mod_set<mods::none, mods::flat, mods::flat, mods::none, mods::flat, mods::flat, mods::flat>
    >;
};

template<class ScaledAs, bool Upper>
using special_scale_set_t = typename special_scale_set<ScaledAs, Upper>::type;


template<class ScaleDef>
struct pack_to_tone_set;

template<class ScaledAs, ident_height_type... Heights, class... Mods>
struct pack_to_tone_set<scale_def<ScaledAs, height_set<Heights...>, mod_set<Mods...>>>
{
    using type = tone_set<
        basic_tone<basic_ident<Heights, Mods>>...
    >;
};

template<class ScaleDef>
using pack_to_tone_set_t = typename pack_to_tone_set<ScaleDef>::type;


template<class KeyIdent, class ScaledAs>
struct scale_def_to_scale
{
    using upward_scale =
        basic_scale<
            ScaledAs,
            cvt::interpret_in_key_t<
                KeyIdent,
                pack_to_tone_set_t<special_scale_set_t<ScaledAs, true>>
            >
        >
    ;
    using downward_scale =
        basic_scale<
            ScaledAs,
            cvt::interpret_in_key_t<
                KeyIdent,
                pack_to_tone_set_t<special_scale_set_t<ScaledAs, false>>
            >
        >
    ;

    using type = std::conditional_t<
        std::is_same_v<upward_scale, downward_scale>,
        dynamic_scale<
            ScaledAs,
            upward_scale
        >,
        dynamic_scale<
            ScaledAs,
            upward_scale, downward_scale
        >
    >;
};

template<class KeyIdent, class ScaledAs>
using scale_def_to_scale_t = typename scale_def_to_scale<KeyIdent, ScaledAs>::type;

} // detail

}} // ompu
