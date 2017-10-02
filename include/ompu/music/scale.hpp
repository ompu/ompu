#pragma once

#include "ompu/music/tone.hpp"
#include "ompu/music/key_feels.hpp"

#include "saya/zed/fold.hpp"


namespace ompu { namespace music {

namespace detail {

template<class... Idents>
struct basic_scale_def {};

template<unsigned... Heights, class... Mods>
using make_basic_scale_def = basic_scale_def<tone_ident_t<Heights, Mods>...>;


using ionian_scale_def = make_basic_scale_def<
    0, 2, 4, 5, 7, 9, 11,
    mods::None, mods::None, mods::None, mods::None, mods::None, mods::None, mods::None
>;
using dorian_scale_def = make_basic_scale_def<
    0, 2, 3, 5, 7, 9, 10,
    mods::None, mods::None, mods::Flat, mods::None, mods::None, mods::None, mods::Flat
>;
using phrigian_scale_def = make_basic_scale_def<
    0, 1, 3, 5, 7, 8, 10,
    mods::None, mods::Flat, mods::Flat, mods::None, mods::None, mods::Flat, mods::Flat
>;
using lydian_scale_def = make_basic_scale_def<
    0, 2, 4, 6, 7, 9, 11,
    mods::None, mods::None, mods::None, mods::Sharp, mods::None, mods::None, mods::None
>;
using mixolydian_scale_def = make_basic_scale_def<
    0, 2, 4, 5, 7, 9, 10,
    mods::None, mods::None, mods::None, mods::None, mods::None, mods::None, mods::Flat
>;
using aeolian_scale_def = make_basic_scale_def<
    0, 2, 3, 5, 7, 8, 10,
    mods::None, mods::None, mods::Flat, mods::None, mods::None, mods::Flat, mods::Flat
>;
using locrian_scale_def = make_basic_scale_def<
    0, 1, 3, 5, 6, 8, 10,
    mods::None, mods::Flat, mods::Flat, mods::None, mods::Flat, mods::Flat, mods::Flat
>;

} // scales


template<class... Tones>
struct basic_scale;

template<class... Idents>
struct basic_scale<basic_tone<Idents>...>
{
    static constexpr unsigned tones_count = sizeof...(Idents);
    static constexpr auto assoc_sharps = saya::zed::fold_add_v<unsigned, detail::sharp_counter<Idents>::value...>;
    static constexpr auto assoc_flats = saya::zed::fold_add_v<unsigned, detail::flat_counter<Idents>::value...>;
};


namespace detail {

template<class Scale, class Mod>
struct modded_scale_in_C;

template<class Mod, class... Tones>
struct modded_scale_in_C<basic_scale<Tones...>, Mod>
{
    static_assert(
        std::is_same_v<Mod, mods::Sharp> ||
        std::is_same_v<Mod, mods::Flat>,
        "Mod must be either Sharp or Flat here"
    );

    using type = basic_scale<
        resolve_tone_in_context_t<
            Tones,
            tone_ident_t<
                canonical_mod_shift_t<Tones::height, Mod>::value,
                mods::None
            >
        >...
    >;
};

template<class Scale, class Mod>
using modded_scale_in_C_t = typename modded_scale_in_C<Scale, Mod>::type;

} // detail


namespace detail {

template<class Ident, class ScaleDef>
struct make_scale;

template<class Ident, unsigned... Heights, class... Mods>
struct make_scale<Ident, basic_scale_def<basic_tone_ident<Heights, Mods>...>>
{
    using root_type = Ident;
    using type = basic_scale<
        basic_tone<
            basic_tone_ident<(Ident::value + Heights) % 12, Mods>
        >...
    >;
};

template<class Ident, class ScaleDef>
using make_scale_t = typename make_scale<Ident, ScaleDef>::type;

} // detail


namespace scales {

template<class Ident>
using ionian = detail::make_scale_t<Ident, detail::ionian_scale_def>;

template<class Ident>
using dorian = detail::make_scale_t<Ident, detail::dorian_scale_def>;

template<class Ident>
using phrigian = detail::make_scale_t<Ident, detail::phrigian_scale_def>;

template<class Ident>
using lydian = detail::make_scale_t<Ident, detail::lydian_scale_def>;

template<class Ident>
using mixolydian = detail::make_scale_t<Ident, detail::mixolydian_scale_def>;

template<class Ident>
using aeolian = detail::make_scale_t<Ident, detail::aeolian_scale_def>;

template<class Ident>
using locrian = detail::make_scale_t<Ident, detail::locrian_scale_def>;

} // scales


namespace scale_mods {

struct Natural {};
struct Harmonic {};
struct Melodic {};

} // scale_mods


namespace detail {

template<class... Scales>
struct dynamic_scale;

template<class Scale>
struct dynamic_scale<Scale>
{
    using scale_type = Scale;
    static constexpr bool is_dynamic = false;
};

template<class Upward, class Downward>
struct dynamic_scale<Upward, Downward>
{
    using upward_scale_type = Upward;
    using downward_scale_type = Downward;
    static constexpr bool is_dynamic = true;
};


template<bool IsGoingUp, class Scale>
struct resolve_dynamic_scale;

template<bool IsGoingUp, class Scale>
struct resolve_dynamic_scale<IsGoingUp, dynamic_scale<Scale>>
{
    using type = typename dynamic_scale<Scale>::scale_type;
};

template<class Upward, class Downward>
struct resolve_dynamic_scale<true, dynamic_scale<Upward, Downward>>
{
    using type = typename dynamic_scale<Upward, Downward>::upward_scale_type;
};

template<class Upward, class Downward>
struct resolve_dynamic_scale<false, dynamic_scale<Upward, Downward>>
{
    using type = typename dynamic_scale<Upward, Downward>::downward_scale_type;
};

template<bool IsGoingUp, class... Scales>
using resolve_dynamic_scale_t = typename resolve_dynamic_scale<IsGoingUp, Scales...>::type;

template<class... Scales>
using canonical_key_scale_t = typename resolve_dynamic_scale<true, Scales...>::type;

} // detail


namespace detail {

template<class ScaleDef, class... ExtraMods>
struct modded_scale_def;

template<class... Idents, class... ExtraMods>
struct modded_scale_def<basic_scale_def<Idents...>, ExtraMods...>
{
    using type = basic_scale_def<
        modded_ident_t<Idents, ExtraMods>...
    >;
};

template<class ScaleDef, class... ExtraMods>
using modded_scale_def_t = typename modded_scale_def<ScaleDef, ExtraMods...>::type;

} // detail


namespace detail {

template<class Ident, class KeyFeel, class ScaleMod>
struct modded_scale;

template<class Ident>
struct modded_scale<Ident, key_feels::Major, scale_mods::Natural>
{
    using type = dynamic_scale<
        make_scale_t<Ident, ionian_scale_def>
    >;
};

template<class Ident>
struct modded_scale<Ident, key_feels::Major, scale_mods::Harmonic>
{
    using upper_scale_type = make_scale_t<
        Ident,
        modded_scale_def_t<
            ionian_scale_def,
            mods::None, mods::None, mods::None, mods::None, mods::None, mods::Flat, mods::None
        >
    >;

    using type = dynamic_scale<
        upper_scale_type, upper_scale_type
    >;
};

template<class Ident>
struct modded_scale<Ident, key_feels::Major, scale_mods::Melodic>
{
    using type = dynamic_scale<
        make_scale_t<Ident, ionian_scale_def>,
        make_scale_t<
            Ident,
            modded_scale_def_t<
                ionian_scale_def,
                mods::None, mods::None, mods::None, mods::None, mods::None, mods::Flat, mods::Flat
            >
        >
    >;
};

// ^^^^^ Major ^^^^^
// ----------------------------------
// vvvvv Minor vvvvv

template<class Ident>
struct modded_scale<Ident, key_feels::Minor, scale_mods::Natural>
{
    using type = dynamic_scale<
        make_scale_t<Ident, aeolian_scale_def>
    >;
};

template<class Ident>
struct modded_scale<Ident, key_feels::Minor, scale_mods::Harmonic>
{
    using upper_scale_type = make_scale_t<
        Ident,
        modded_scale_def_t<
            aeolian_scale_def,
            mods::None, mods::None, mods::None, mods::None, mods::None, mods::Sharp, mods::None
        >
    >;

    using type = dynamic_scale<
        upper_scale_type, upper_scale_type
    >;
};

template<class Ident>
struct modded_scale<Ident, key_feels::Minor, scale_mods::Melodic>
{
    using type = dynamic_scale<
        make_scale_t<
            Ident,
            modded_scale_def_t<
                aeolian_scale_def,
                mods::None, mods::None, mods::None, mods::None, mods::None, mods::Sharp, mods::Sharp
            >
        >,
        make_scale_t<Ident, aeolian_scale_def>
    >;
};

template<class Ident, class KeyFeel, class ScaleMod>
using modded_scale_t = typename modded_scale<Ident, KeyFeel, ScaleMod>::type;

} // detail


namespace scales {

template<class Ident, class KeyFeel>
struct natural : detail::modded_scale_t<Ident, KeyFeel, scale_mods::Natural> {};

template<class Ident>
using natural_major = natural<Ident, key_feels::Major>;

template<class Ident>
using natural_minor = natural<Ident, key_feels::Minor>;


template<class Ident, class KeyFeel>
struct harmonic : detail::modded_scale_t<Ident, KeyFeel, scale_mods::Harmonic> {};

template<class Ident>
using harmonic_major = harmonic<Ident, key_feels::Major>;

template<class Ident>
using harmonic_minor = harmonic<Ident, key_feels::Minor>;


template<class Ident, class KeyFeel>
struct melodic : detail::modded_scale_t<Ident, KeyFeel, scale_mods::Melodic> {};

template<class Ident>
using melodic_major = melodic<Ident, key_feels::Major>;

template<class Ident>
using melodic_minor = melodic<Ident, key_feels::Minor>;

} // scales

}} // ompu
