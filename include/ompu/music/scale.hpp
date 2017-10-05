#pragma once

#include "ompu/music/type_traits.hpp"
#include "ompu/music/mod.hpp"
#include "ompu/music/tone.hpp"
#include "ompu/music/key_sign.hpp"
#include "ompu/music/scale_def.hpp"


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


template<class ScaledAs, class... Idents>
struct basic_scale<ScaledAs, ident_set<Idents...>>
{
    static constexpr unsigned tones_count = sizeof...(Idents);
    using ident_set_type = ident_set<Idents...>;

    using scaled_as_type = ScaledAs;
    static constexpr bool is_wild = std::is_same_v<scaled_as_type, scales::wild>;

    static constexpr auto assoc_sharps = saya::zed::fold_add_v<std::size_t, detail::sharp_counter<Idents>::value...>;
    static constexpr auto assoc_flats = saya::zed::fold_add_v<std::size_t, detail::flat_counter<Idents>::value...>;
};

template<class... Tones>
inline constexpr auto make_wild_scale(Tones...)
{
    return basic_scale<scales::wild, Tones...>{};
}

template<class... Tones>
inline constexpr auto make_wild_scale(tone_set<Tones...>)
{
    return basic_scale<scales::wild, Tones...>{};
}

namespace scales {

template<class Ident>
using make_ionian = detail::scale_def_to_scale_t<Ident, scales::ionian>;

template<class Ident>
using make_dorian = detail::scale_def_to_scale_t<Ident, scales::dorian>;

template<class Ident>
using make_phrigian = detail::scale_def_to_scale_t<Ident, scales::phrigian>;

template<class Ident>
using make_lydian = detail::scale_def_to_scale_t<Ident, scales::lydian>;

template<class Ident>
using make_mixolydian = detail::scale_def_to_scale_t<Ident, scales::mixolydian>;

template<class Ident>
using make_aeolian = detail::scale_def_to_scale_t<Ident, scales::aeolian>;

template<class Ident>
using make_locrian = detail::scale_def_to_scale_t<Ident, scales::locrian>;

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


namespace scales {

template<class Ident, class KeyFeel>
using make_natural = detail::scale_def_to_scale_t<Ident, scales::natural<KeyFeel>>;

template<class Ident>
using make_natural_major = make_natural<Ident, key_feels::major>;

template<class Ident>
using make_natural_minor = make_natural<Ident, key_feels::minor>;


template<class Ident, class KeyFeel>
using make_harmonic = detail::scale_def_to_scale_t<Ident, scales::harmonic<KeyFeel>>;

template<class Ident>
using make_harmonic_major = make_harmonic<Ident, key_feels::major>;

template<class Ident>
using make_harmonic_minor = make_harmonic<Ident, key_feels::minor>;


template<class Ident, class KeyFeel>
using make_melodic = detail::scale_def_to_scale_t<Ident, scales::melodic<KeyFeel>>;

template<class Ident>
using make_melodic_major = make_melodic<Ident, key_feels::major>;

template<class Ident>
using make_melodic_minor = make_melodic<Ident, key_feels::minor>;

} // scales

}} // ompu
