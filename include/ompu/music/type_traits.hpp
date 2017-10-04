#pragma once

#include "ompu/music/music_fwd.hpp"

#include "saya/zed/seq.hpp"
#include "saya/zed/fold.hpp"
#include "saya/zed/meta.hpp"
#include "saya/zed/blackhole.hpp"

#include <type_traits>
#include <tuple>
#include <utility>


namespace ompu { namespace music {

namespace cvt {

template<class Seq>
struct to_tuple;

template<class... Args>
struct to_tuple<std::tuple<Args...>>
{
    using type = std::tuple<Args...>;
};

template<class... Idents>
struct to_tuple<ident_set<Idents...>>
{
    using type = std::tuple<Idents...>;
};

template<class... Tones>
struct to_tuple<tone_set<Tones...>>
{
    using type = std::tuple<Tones...>;
};

template<class Seq>
using to_tuple_t = typename to_tuple<Seq>::type;


template<class T, class Enabled = void>
struct canonical;

template<class T>
using canonical_t = typename canonical<T>::type;

} // cvt

template<class T, class Enabled = void>
struct is_canonical : std::is_same<T, cvt::canonical_t<T>> {};

template<class T>
static constexpr bool is_canonical_v = is_canonical<T>::value;


namespace cvt {

namespace detail {

template<class Mod> struct opposite_mod;
template<> struct opposite_mod<mods::none> { using type = mods::none; };
template<> struct opposite_mod<mods::sharp> { using type = mods::flat; };
template<> struct opposite_mod<mods::flat> { using type = mods::sharp; };
template<> struct opposite_mod<mods::dbl_sharp> { using type = mods::dbl_flat; };
template<> struct opposite_mod<mods::dbl_flat> { using type = mods::dbl_sharp; };
template<> struct opposite_mod<mods::natural> { using type = mods::natural; };

template<class Mod> using opposite_mod_t = typename opposite_mod<Mod>::type;


template<int Ofs>
struct height_shift_canonical_offset
    : std::conditional_t<
        Ofs < 0,
        std::integral_constant<int, -(-Ofs % 12)>,
        std::integral_constant<int, Ofs % 12>
    >
{};

template<ident_height_type Height, int O, int Delta = int(Height) + height_shift_canonical_offset<O>::value>
struct height_shift_impl
    : std::conditional_t<
        Delta < 0,
        std::integral_constant<ident_height_type, 12 + Delta>,
        std::integral_constant<ident_height_type, Delta % 12>
    >
{};

template<ident_height_type Height, int Ofs>
struct height_shift : height_shift_impl<Height, Ofs> {};

template<ident_height_type Height, int Ofs>
constexpr auto height_shift_v = height_shift<Height, Ofs>::value;


template<class Ident>
struct add_sharp;

template<ident_height_type Height>
struct add_sharp<basic_ident<Height, mods::none>>
{
    using type = basic_ident<Height + 1, mods::sharp>;
};
template<ident_height_type Height>
struct add_sharp<basic_ident<Height, mods::sharp>>
{
    using type = basic_ident<Height + 1, mods::dbl_sharp>;
};
template<ident_height_type Height>
struct add_sharp<basic_ident<Height, mods::flat>>
{
    using type = basic_ident<Height + 1, mods::natural>;
};
template<class Ident>
using add_sharp_t = typename add_sharp<Ident>::type;


template<class Ident>
struct add_flat;

template<ident_height_type Height>
struct add_flat<basic_ident<Height, mods::none>>
{
    using type = basic_ident<Height - 1, mods::flat>;
};
template<ident_height_type Height>
struct add_flat<basic_ident<Height, mods::sharp>>
{
    using type = basic_ident<Height - 1, mods::natural>;
};
template<ident_height_type Height>
struct add_flat<basic_ident<Height, mods::flat>>
{
    using type = basic_ident<Height - 1, mods::dbl_flat>;
};
template<class Ident>
using add_flat_t = typename add_flat<Ident>::type;


template<class Ident, class Mod>
struct modded_ident;

template<class Ident>
struct modded_ident<Ident, mods::none>
{
    using type = Ident;
};

template<class Ident>
struct modded_ident<Ident, mods::sharp>
{
    using type = detail::add_sharp_t<Ident>;
};

template<class Ident>
struct modded_ident<Ident, mods::flat>
{
    using type = detail::add_flat_t<Ident>;
};

template<class Ident, class Mod>
using modded_ident_t = typename modded_ident<Ident, Mod>::type;


// FIXME
#if 0

template<class T>
struct strict_root_holder { /* using type = T; */ };

template<class Ident, class RootIdent>
struct rooted_ident;

template<class Ident, class RootIdent>
struct rooted_ident<Ident, strict_root_holder<RootIdent>>
{
    using type =
};
#endif


template<class Ident> struct canonical_ident;
template<class Mod> struct canonical_ident<basic_ident<0, Mod>> { using type = idents::C; };
template<class Mod> struct canonical_ident<basic_ident<1, Mod>> { using type = idents::Cs; };
template<class Mod> struct canonical_ident<basic_ident<2, Mod>> { using type = idents::D; };
template<class Mod> struct canonical_ident<basic_ident<3, Mod>> { using type = idents::Ds; };
template<class Mod> struct canonical_ident<basic_ident<4, Mod>> { using type = idents::E; };
template<class Mod> struct canonical_ident<basic_ident<5, Mod>> { using type = idents::F; };
template<class Mod> struct canonical_ident<basic_ident<6, Mod>> { using type = idents::Fs; };
template<class Mod> struct canonical_ident<basic_ident<7, Mod>> { using type = idents::G; };
template<class Mod> struct canonical_ident<basic_ident<8, Mod>> { using type = idents::Gs; };
template<class Mod> struct canonical_ident<basic_ident<9, Mod>> { using type = idents::A; };
template<class Mod> struct canonical_ident<basic_ident<10, Mod>> { using type = idents::As; };
template<class Mod> struct canonical_ident<basic_ident<11, Mod>> { using type = idents::B; };

template<class Ident>
using canonical_ident_t = typename canonical_ident<Ident>::type;

} // cvt::detail

template<ident_height_type Height, class... Ts>
struct canonical<basic_ident<Height, Ts...>>
{
    using type = detail::canonical_ident_t<basic_ident<Height, Ts...>>;
};


namespace detail {

template<ident_height_type Height> struct canonical_ident_height_impl;
template<> struct canonical_ident_height_impl<0> : std::integral_constant<ident_height_type, 0> {};
template<> struct canonical_ident_height_impl<1> : std::integral_constant<ident_height_type, 0> {};
template<> struct canonical_ident_height_impl<2> : std::integral_constant<ident_height_type, 1> {};
template<> struct canonical_ident_height_impl<3> : std::integral_constant<ident_height_type, 1> {};
template<> struct canonical_ident_height_impl<4> : std::integral_constant<ident_height_type, 2> {};
template<> struct canonical_ident_height_impl<5> : std::integral_constant<ident_height_type, 3> {};
template<> struct canonical_ident_height_impl<6> : std::integral_constant<ident_height_type, 3> {};
template<> struct canonical_ident_height_impl<7> : std::integral_constant<ident_height_type, 4> {};
template<> struct canonical_ident_height_impl<8> : std::integral_constant<ident_height_type, 4> {};
template<> struct canonical_ident_height_impl<9> : std::integral_constant<ident_height_type, 5> {};
template<> struct canonical_ident_height_impl<10> : std::integral_constant<ident_height_type, 5> {};
template<> struct canonical_ident_height_impl<11> : std::integral_constant<ident_height_type, 6> {};


template<ident_height_type Height, class Mod>
struct canonical_ident_height : std::integral_constant<
    ident_height_type,
    canonical_ident_height_impl<
        height_shift_v<
            Height, opposite_mod_t<Mod>::value
        >
    >::value
>
{};

template<ident_height_type Height, class Mod>
constexpr auto canonical_ident_height_v = canonical_ident_height<Height, Mod>::value;

} // cvt::detail



template<class Tone>
struct canonical_tone;

template<class Ident>
struct canonical_tone<basic_tone<Ident>>
{
    using type = basic_tone<canonical_t<Ident>>;
};
template<class Tone>
using canonical_tone_t = typename canonical_tone<Tone>::type;


template<class... Ts>
inline constexpr auto
make_canonical(basic_tone<Ts...>) noexcept
{
    return canonical_tone_t<basic_tone<Ts...>>{};
}

template<class... Ts>
struct canonical<basic_tone<Ts...>>
{
    using type = canonical_tone_t<basic_tone<Ts...>>;
};


template<class Scale>
struct canonical_scale;

template<class OriginallyScaledAs, class... Tones>
struct canonical_scale<basic_scale<OriginallyScaledAs, tone_set<Tones...>>>
{
    // short-circuit for originally-canonically-defined scale
    using type = std::conditional_t<
        std::is_same_v<OriginallyScaledAs, scales::canonical>,
        basic_scale<OriginallyScaledAs, tone_set<Tones...>>,
        basic_scale<
            scales::canonical,
            tone_set<canonical_tone_t<Tones>...>
        >
    >;
};

template<class Scale>
using canonical_scale_t = typename canonical_scale<Scale>::type;

template<class... Ts>
struct canonical<basic_scale<Ts...>>
{
    using type = canonical_scale_t<basic_scale<Ts...>>;
};


template<class Tone, class ResolveToTone, class Context = contexts::key>
struct resolved_to_tone_in_context
{
    // Example:

    // Tone = C# (1, mods::none) on C#Maj
    // Resolve to Tone = D (2, mods::none) on CMaj
    // Result = C# (1, mods::sharp) -- reason: upward resolution

    // Tone = C# (1, mods::none) on C#Maj
    // Resolve to Tone = C (0, mods::none) on CMaj
    // Result = Db (1, mods::flat) -- reason: downward resolution

    static_assert(
        is_canonical_v<ResolveToTone>,
        "resolution context tone must be a canonical tone (you can't resolve a tone to an already-shapred-or-flatted another tone)"
    );

private:
    static constexpr auto from_height = Tone::ident_type::height;
    static constexpr auto to_height = ResolveToTone::ident_type::height;

public:
    static_assert(
        from_height != to_height,
        "you can't resolve a tone to an another tone with same height"
    );

    using type = std::conditional_t<
        from_height < to_height,
        basic_tone<basic_ident<from_height, mods::sharp>>,
        basic_tone<basic_ident<from_height, mods::flat>>
    >;
};

template<class... Args>
using resolved_to_tone_in_context_t = typename resolved_to_tone_in_context<Args...>::type;


namespace detail {

template<class Scale, class Mod>
struct force_modded_scale_in_C;

template<class Mod, class... Tones>
struct force_modded_scale_in_C<basic_scale<Tones...>, Mod>
{
    static_assert(
        std::is_same_v<Mod, mods::sharp> ||
        std::is_same_v<Mod, mods::flat>,
        "Mod must be either Sharp or Flat here"
    );

    using type = basic_scale<
        resolved_to_tone_in_context_t<
            Tones,
            basic_ident<
                height_shift_v<Tones::height, Mod::value>,
                mods::none
            >,
            contexts::key
        >...
    >;
};

template<class Scale, class Mod>
using force_modded_scale_in_C_t = typename force_modded_scale_in_C<Scale, Mod>::type;


template<class Key>
struct force_enharmonic_key;

template<class Ident, class KeyFeel>
struct force_enharmonic_key<basic_key<Ident, KeyFeel>>
{
    using type = basic_key<canonical_ident_t<Ident>, KeyFeel>;
};

template<class Key>
using force_enharmonic_key_t = typename force_enharmonic_key<Key>::type;

} // cvt::detail


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

template<class Scale>
inline /* constexpr */ auto make_resolve_dynamic_scale(Scale const&, bool const is_going_up)
{
    if (is_going_up) {
        return resolve_dynamic_scale_t<true, Scale>{};
    } else {
        return resolve_dynamic_scale_t<false, Scale>{};
    }
}


template<class... Scales>
using canonical_key_scale_t = typename resolve_dynamic_scale<true, Scales...>::type;


namespace detail {

template<class RootMod, class SharpedScale, class FlattedScale>
struct canonical_key_mod
{
    using mod_type = std::conditional_t<
        SharpedScale::assoc_sharps == 0 && FlattedScale::assoc_flats == 0,
        mods::none,
        std::conditional_t<
            SharpedScale::assoc_sharps == FlattedScale::assoc_flats,
            RootMod,
            std::conditional_t<
                SharpedScale::assoc_sharps < FlattedScale::assoc_flats,
                mods::sharp,
                mods::flat
            >
        >
    >;

    static constexpr auto assoc_sharps = SharpedScale::assoc_sharps;
    static constexpr auto assoc_flats = FlattedScale::assoc_flats;

    static constexpr auto assoc_lesser_mods = assoc_sharps <= assoc_flats ? assoc_sharps : assoc_flats;
};

template<class RootMod, class SharpedScale, class FlattedScale>
using canonical_key_mod_t = typename canonical_key_mod<RootMod, SharpedScale, FlattedScale>::type;

} // cvt::detail
} // cvt


namespace detail {

template<class Ident> struct sharp_counter : std::integral_constant<ident_height_type, 0> {};
template<ident_height_type Height> struct sharp_counter<basic_ident<Height, mods::sharp>> : std::integral_constant<ident_height_type, 1> {};
template<ident_height_type Height> struct sharp_counter<basic_ident<Height, mods::dbl_sharp>> : std::integral_constant<ident_height_type, 2> {};
template<class Ident> constexpr auto sharp_counter_v = sharp_counter<Ident>::value;

template<class Ident> struct flat_counter : std::integral_constant<ident_height_type, 0> {};
template<ident_height_type Height> struct flat_counter<basic_ident<Height, mods::flat>> : std::integral_constant<ident_height_type, 1> {};
template<ident_height_type Height> struct flat_counter<basic_ident<Height, mods::dbl_flat>> : std::integral_constant<ident_height_type, 2> {};
template<class Ident> constexpr auto flat_counter_v = flat_counter<Ident>::value;



template<class Key>
struct has_enharmonic_key_impl : std::false_type {};

template<> struct has_enharmonic_key_impl<keys::BMaj> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::FsMaj> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::CsMaj> : std::true_type {};

template<> struct has_enharmonic_key_impl<keys::Gsmin> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::Dsmin> : std::true_type {};
template<> struct has_enharmonic_key_impl<keys::Asmin> : std::true_type {};

} // detail


template<class Key>
struct has_enharmonic_key
    : std::conditional_t<
        detail::has_enharmonic_key_impl<Key>::value ||
        detail::has_enharmonic_key_impl<cvt::detail::force_enharmonic_key_t<Key>>::value,
        std::true_type,
        std::false_type
    >
{};

template<class Key>
constexpr bool has_enharmonic_key_v = has_enharmonic_key<Key>::value;


template<class KeyFeel> struct is_major_feel;
template<> struct is_major_feel<key_feels::major> : std::true_type {};
template<> struct is_major_feel<key_feels::minor> : std::false_type {};
template<class KeyFeel> constexpr auto is_major_feel_v = is_major_feel<KeyFeel>::value;

template<class KeyFeel> struct is_minor_feel;
template<> struct is_minor_feel<key_feels::major> : std::false_type {};
template<> struct is_minor_feel<key_feels::minor> : std::true_type {};
template<class KeyFeel> constexpr auto is_minor_feel_v = is_minor_feel<KeyFeel>::value;


namespace cvt {

template<class Key, class Enabled = void>
struct enharmonic_key { using type = void; };

template<class Ident, class KeyFeel>
struct enharmonic_key<
    basic_key<Ident, KeyFeel>,
    std::enable_if_t<
        has_enharmonic_key_v<basic_key<Ident, KeyFeel>>
    >
>
{
    using type = basic_key<
        basic_ident<
            Ident::height,
            std::conditional_t<
                Ident::sharped, mods::flat, mods::sharp
            >
        >,
        KeyFeel
    >;

    static_assert(!std::is_same_v<type, basic_key<Ident, KeyFeel>>, "enharmonic key must not be the same as original key");
};

template<class Key>
using enharmonic_key_t = typename enharmonic_key<Key>::type;


template<class Key, class Enabled = void>
struct enharmonic_key_pair;

template<class Key>
struct enharmonic_key_pair<Key, std::enable_if_t<has_enharmonic_key_v<Key>>>
{
    using type = std::tuple<Key, enharmonic_key_t<Key>>;
};

template<class Key>
struct enharmonic_key_pair<Key, std::enable_if_t<!has_enharmonic_key_v<Key>>>
{
    using type = std::tuple<Key>;
};

template<class Key>
using enharmonic_key_pair_t = typename enharmonic_key_pair<Key>::type;


namespace detail {

template<class Ident, class KeyFeel>
struct relative_ident;

template<ident_height_type Height, class Mod>
struct relative_ident<
    basic_ident<Height, Mod>,
    key_feels::major
>
{
    using type = basic_ident<height_shift_v<Height, 9>, Mod>;
};
template<ident_height_type Height, class Mod>
struct relative_ident<
    basic_ident<Height, Mod>,
    key_feels::minor
>
{
    using type = basic_ident<height_shift_v<Height, -9>, Mod>;
};

template<class Ident, class KeyFeel>
using relative_ident_t = typename relative_ident<Ident, KeyFeel>::type;

} // cvt::detail


template<class KeyFeel>
struct opposite_key_feel;

template<>
struct opposite_key_feel<key_feels::major>
{
    using type = key_feels::minor;
};
template<>
struct opposite_key_feel<key_feels::minor>
{
    using type = key_feels::major;
};
template<class KeyFeel>
using opposite_key_feel_t = typename opposite_key_feel<KeyFeel>::type;


template<class Key>
struct relative_key;

template<class Ident, class KeyFeel>
struct relative_key<basic_key<Ident, KeyFeel>>
{
    using type = basic_key<
        detail::relative_ident_t<Ident, KeyFeel>,
        opposite_key_feel_t<KeyFeel>
    >;
};

template<class Key>
using relative_key_t = typename relative_key<Key>::type;


template<class KeyFeel, class FeelMod>
struct modded_feel;

template<class KeyFeel>
struct modded_feel<
    KeyFeel,
    feel_mods::same
>
{
    using type = KeyFeel;
};

template<>
struct modded_feel<
    key_feels::major,
    feel_mods::opposite
>
{
    using type = key_feels::minor;
};

template<>
struct modded_feel<
    key_feels::minor,
    feel_mods::opposite
>
{
    using type = key_feels::major;
};

template<class KeyFeel, class FixedKeyFeel>
struct modded_feel<
    KeyFeel,
    feel_mods::forced_fixed<FixedKeyFeel>
>
{
    using type = FixedKeyFeel;
};

template<class KeyFeel, class FeelMod>
using modded_feel_t = typename modded_feel<KeyFeel, FeelMod>::type;


template<class HeightSet, class ModSet>
struct pack_to_ident_set;

template<ident_height_type... Heights, class... Mods>
struct pack_to_ident_set<
    height_set<Heights...>,
    mod_set<Mods...>
>
{
    using type = ident_set<basic_ident<Heights, Mods>...>;
};

template<class HeightSet, class ModSet>
using pack_to_ident_set_t = typename pack_to_ident_set<HeightSet, ModSet>::type;

} // cvt


template<class ScaleLike, class Enabled = void>
struct is_scale : std::false_type {};

template<
    class... Ts
>
struct is_scale<dynamic_scale<Ts...>> : std::true_type {};

template<
    class... Ts
>
struct is_scale<
    basic_scale<Ts...>
> : std::true_type {};

template<class ScaleLike>
constexpr auto is_scale_v = is_scale<ScaleLike>::value;


template<class Degree>
struct is_diatonic : std::false_type {};

template<class Degree>
constexpr bool is_diatonic_v = is_diatonic<Degree>::value;

template<> struct is_diatonic<basic_degree<0>> : std::true_type {};
template<> struct is_diatonic<basic_degree<2>> : std::true_type {};
template<> struct is_diatonic<basic_degree<4>> : std::true_type {};
template<> struct is_diatonic<basic_degree<5>> : std::true_type {};
template<> struct is_diatonic<basic_degree<7>> : std::true_type {};
template<> struct is_diatonic<basic_degree<9>> : std::true_type {};
template<> struct is_diatonic<basic_degree<11>> : std::true_type {};

}} // ompu
