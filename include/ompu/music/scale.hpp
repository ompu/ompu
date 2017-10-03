#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/mod.hpp"
#include "ompu/music/tone.hpp"
#include "ompu/music/key_feel.hpp"


namespace ompu { namespace music {

namespace scales {

struct wild { static constexpr auto name = sprout::to_string("(Wild scale)"); };

struct canonical { static constexpr auto name = sprout::to_string("(Canonical scale on C)"); };

struct ionian { static constexpr auto name = sprout::to_string("Ionian"); };
struct dorian { static constexpr auto name = sprout::to_string("Dorian"); };
struct phrigian { static constexpr auto name = sprout::to_string("Phrigian"); };
struct lydian { static constexpr auto name = sprout::to_string("Lydian"); };
struct mixolydian { static constexpr auto name = sprout::to_string("Mixolydian"); };
struct aeolian { static constexpr auto name = sprout::to_string("Aeolian"); };
struct locrian { static constexpr auto name = sprout::to_string("Locrian"); };


template<class KeyFeel>
struct natural
{
    using key_feel = KeyFeel;
    static constexpr auto name = sprout::to_string("Natural ") + KeyFeel::name;
};

template<class KeyFeel>
struct harmonic
{
    using key_feel = KeyFeel;
    static constexpr auto name = sprout::to_string("Harmonic ") + KeyFeel::name;
};

template<class KeyFeel>
struct melodic
{
    using key_feel = KeyFeel;
    static constexpr auto name = sprout::to_string("Melodic ") + KeyFeel::name;
};

} // scales


namespace detail {

template<class ScaledAs, class... Idents>
struct basic_scale_def {};

template<class ScaledAs, unsigned... Heights, class... Mods>
using pack_to_scale_def = basic_scale_def<ScaledAs, basic_ident<Heights, Mods>...>;


template<class ScaledAs>
struct scale_def;

template<> struct scale_def<scales::ionian>
{
    using type = pack_to_scale_def<
        scales::ionian,
        0, 2, 4, 5, 7, 9, 11,
        mods::none, mods::none, mods::none, mods::none, mods::none, mods::none, mods::none
    >;
};
template<> struct scale_def<scales::dorian>
{
    using type = pack_to_scale_def<
        scales::dorian,
        0, 2, 3, 5, 7, 9, 10,
        mods::none, mods::none, mods::flat, mods::none, mods::none, mods::none, mods::flat
    >;
};
template<> struct scale_def<scales::phrigian>
{
    using type = pack_to_scale_def<
        scales::phrigian,
        0, 1, 3, 5, 7, 8, 10,
        mods::none, mods::flat, mods::flat, mods::none, mods::none, mods::flat, mods::flat
    >;
};
template<> struct scale_def<scales::lydian>
{
    using type = pack_to_scale_def<
        scales::lydian,
        0, 2, 4, 6, 7, 9, 11,
        mods::none, mods::none, mods::none, mods::sharp, mods::none, mods::none, mods::none
    >;
};
template<> struct scale_def<scales::mixolydian>
{
    using type = pack_to_scale_def<
        scales::mixolydian,
        0, 2, 4, 5, 7, 9, 10,
        mods::none, mods::none, mods::none, mods::none, mods::none, mods::none, mods::flat
    >;
};
template<> struct scale_def<scales::aeolian>
{
    using type = pack_to_scale_def<
        scales::aeolian,
        0, 2, 3, 5, 7, 8, 10,
        mods::none, mods::none, mods::flat, mods::none, mods::none, mods::flat, mods::flat
    >;
};
template<> struct scale_def<scales::locrian>
{
    using type = pack_to_scale_def<
        scales::locrian,
        0, 1, 3, 5, 6, 8, 10,
        mods::none, mods::flat, mods::flat, mods::none, mods::flat, mods::flat, mods::flat
    >;
};
template<class ScaledAs>
using scale_def_t = typename scale_def<ScaledAs>::type;

} // detail


template<class ScaledAs, class... Idents>
struct basic_scale<ScaledAs, basic_tone<Idents>...>
{
    static constexpr unsigned tones_count = sizeof...(Idents);
    using tones_seq = std::tuple<basic_tone<Idents>...>;

    using scaled_as_type = ScaledAs;
    static constexpr bool is_wild = std::is_same_v<scaled_as_type, scales::wild>;

    using canonical_type = cvt::detail::canonical_scale_t<basic_scale>;
    static constexpr bool is_canonical = std::is_same_v<basic_scale, canonical_type>;

    static constexpr auto assoc_sharps = saya::zed::fold_add_v<unsigned, detail::sharp_counter<Idents>::value...>;
    static constexpr auto assoc_flats = saya::zed::fold_add_v<unsigned, detail::flat_counter<Idents>::value...>;
};

template<class... Tones>
struct wild_scale : basic_scale<scales::wild, Tones...> {};

template<class... Tones>
inline constexpr auto make_wild_scale(Tones...)
{
    return wild_scale<Tones...>{};
}


namespace detail {

template<class Ident, class ScaleDef>
struct rooted_scale;

template<class ScaledAs, class Ident, unsigned... Heights, class... Mods>
struct rooted_scale<Ident, basic_scale_def<ScaledAs, basic_ident<Heights, Mods>...>>
{
    using root_type = Ident;
    using type = basic_scale<
        ScaledAs,
        basic_tone<
            basic_ident<(Ident::height + Heights) % 12, Mods>
        >...
    >;
};

template<class Ident, class ScaleDef>
using rooted_scale_t = typename rooted_scale<Ident, ScaleDef>::type;

} // detail


namespace scales {

template<class Ident>
using make_ionian = detail::rooted_scale_t<Ident, detail::scale_def_t<scales::ionian>>;

template<class Ident>
using make_dorian = detail::rooted_scale_t<Ident, detail::scale_def_t<scales::dorian>>;

template<class Ident>
using make_phrigian = detail::rooted_scale_t<Ident, detail::scale_def_t<scales::phrigian>>;

template<class Ident>
using make_lydian = detail::rooted_scale_t<Ident, detail::scale_def_t<scales::lydian>>;

template<class Ident>
using make_mixolydian = detail::rooted_scale_t<Ident, detail::scale_def_t<scales::mixolydian>>;

template<class Ident>
using make_aeolian = detail::rooted_scale_t<Ident, detail::scale_def_t<scales::aeolian>>;

template<class Ident>
using make_locrian = detail::rooted_scale_t<Ident, detail::scale_def_t<scales::locrian>>;

} // scales


template<class ScaledAs, class Scale>
struct dynamic_scale<ScaledAs, Scale>
{
    using upward_scale_type = Scale;
    using downward_scale_type = upward_scale_type;

    using scaled_as_type = ScaledAs;
    static constexpr bool is_dynamic = false;
};

template<class ScaledAs, class Upward, class Downward>
struct dynamic_scale<ScaledAs, Upward, Downward>
{
    using upward_scale_type = Upward;
    using downward_scale_type = Downward;

    static_assert(
        std::is_same_v<typename Upward::scaled_as_type, typename Downward::scaled_as_type>,
        "dynamic_scale: Upward and Downward scales must be scaled-as same role"
    );
    using scaled_as_type = ScaledAs; // typename Upward::scaled_as_type;

    static constexpr bool is_dynamic = true;
};


namespace detail {

template<class ScaledAs, class ScaleDef, class... ExtraMods>
struct modded_scale_def;

template<class ScaledAs, class OriginallyScaledAs, class... Idents, class... ExtraMods>
struct modded_scale_def<ScaledAs, basic_scale_def<OriginallyScaledAs, Idents...>, ExtraMods...>
{
    using type = basic_scale_def<
        ScaledAs,
        cvt::detail::modded_ident_t<Idents, ExtraMods>...
    >;
};

template<class ScaledAs, class ScaleDef, class... ExtraMods>
using modded_scale_def_t = typename modded_scale_def<ScaledAs, ScaleDef, ExtraMods...>::type;

} // detail


namespace detail {

template<class Ident, class ScaleMod>
struct modded_scale;

template<class Ident>
struct modded_scale<Ident, scales::natural<key_feels::major>>
{
    using type = dynamic_scale<
        scales::natural<key_feels::major>,
        rooted_scale_t<Ident, scale_def_t<scales::ionian>>
    >;
};

template<class Ident>
struct modded_scale<Ident, scales::harmonic<key_feels::major>>
{
    using upper_scale_type = rooted_scale_t<
        Ident,
        modded_scale_def_t<
            scales::harmonic<key_feels::major>,
            scale_def_t<scales::ionian>,
            mods::none, mods::none, mods::none, mods::none, mods::none, mods::flat, mods::none
        >
    >;

    using type = dynamic_scale<
        scales::harmonic<key_feels::major>,
        upper_scale_type, upper_scale_type
    >;
};

template<class Ident>
struct modded_scale<Ident, scales::melodic<key_feels::major>>
{
    using type = dynamic_scale<
        scales::melodic<key_feels::major>,
        rooted_scale_t<Ident, scale_def_t<scales::ionian>>,
        rooted_scale_t<
            Ident,
            modded_scale_def_t<
                scales::melodic<key_feels::major>,
                scale_def_t<scales::ionian>,
                mods::none, mods::none, mods::none, mods::none, mods::none, mods::flat, mods::flat
            >
        >
    >;
};

// ^^^^^ Major ^^^^^
// ----------------------------------
// vvvvv Minor vvvvv

template<class Ident>
struct modded_scale<Ident, scales::natural<key_feels::minor>>
{
    using type = dynamic_scale<
        scales::natural<key_feels::minor>,
        rooted_scale_t<Ident, scale_def_t<scales::aeolian>>
    >;
};

template<class Ident>
struct modded_scale<Ident, scales::harmonic<key_feels::minor>>
{
    using upper_scale_type = rooted_scale_t<
        Ident,
        modded_scale_def_t<
            scales::harmonic<key_feels::minor>,
            scale_def_t<scales::aeolian>,
            mods::none, mods::none, mods::none, mods::none, mods::none, mods::sharp, mods::none
        >
    >;

    using type = dynamic_scale<
        scales::harmonic<key_feels::minor>,
        upper_scale_type, upper_scale_type
    >;
};

template<class Ident>
struct modded_scale<Ident, scales::melodic<key_feels::minor>>
{
    using type = dynamic_scale<
        scales::melodic<key_feels::minor>,
        rooted_scale_t<
            Ident,
            modded_scale_def_t<
                scales::melodic<key_feels::minor>,
                scale_def_t<scales::aeolian>,
                mods::none, mods::none, mods::none, mods::none, mods::none, mods::sharp, mods::sharp
            >
        >,
        rooted_scale_t<Ident, scale_def_t<scales::aeolian>>
    >;
};

template<class Ident, class ScaleMod>
using modded_scale_t = typename modded_scale<Ident, ScaleMod>::type;

} // detail


namespace scales {

template<class Ident, class KeyFeel>
using make_natural = detail::modded_scale_t<Ident, scales::natural<KeyFeel>>;

template<class Ident>
using make_natural_major = make_natural<Ident, key_feels::major>;

template<class Ident>
using make_natural_minor = make_natural<Ident, key_feels::minor>;


template<class Ident, class KeyFeel>
using make_harmonic = detail::modded_scale_t<Ident, scales::harmonic<KeyFeel>>;

template<class Ident>
using make_harmonic_major = make_harmonic<Ident, key_feels::major>;

template<class Ident>
using make_harmonic_minor = make_harmonic<Ident, key_feels::minor>;


template<class Ident, class KeyFeel>
using make_melodic = detail::modded_scale_t<Ident, scales::melodic<KeyFeel>>;

template<class Ident>
using make_melodic_major = make_melodic<Ident, key_feels::major>;

template<class Ident>
using make_melodic_minor = make_melodic<Ident, key_feels::minor>;

} // scales

}} // ompu
