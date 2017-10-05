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


template<class ToneOffset>
struct canonical_tone_shift_offset
    : std::conditional_t<
        ToneOffset::value < 0,
        tone_offset<-(-ToneOffset::value % 12)>,
        tone_offset<ToneOffset::value % 12>
    >
{};

template<class Tone, class O, int Delta = int(Tone::height) + canonical_tone_shift_offset<O>::value>
struct canonical_tone_shift_impl
{
    using type = std::conditional_t<
        Delta < 0,
        basic_tone<tone_height<12 + Delta>>,
        basic_tone<tone_height<Delta % 12>>
    >;
};

template<class Tone, class ToneOffset>
struct canonical_tone_shift
{
    using type = typename canonical_tone_shift_impl<Tone, ToneOffset>::type;
};

template<class Tone, class ToneOffset>
using canonical_tone_shift_t = typename canonical_tone_shift<Tone, ToneOffset>::type;

} // cvt::detail


template<class Mod, class AppliedMod>
struct add_mod;

template<class AppliedMod>
struct add_mod<mods::none, AppliedMod> { using type = AppliedMod; };
template<> struct add_mod<mods::sharp, mods::sharp> { using type = mods::dbl_sharp; };
template<> struct add_mod<mods::sharp, mods::flat> { using type = mods::natural; };
template<> struct add_mod<mods::flat, mods::flat> { using type = mods::dbl_flat; };
template<> struct add_mod<mods::flat, mods::sharp> { using type = mods::natural; };

template<class Mod, class AppliedMod>
using add_mod_t = typename add_mod<Mod, AppliedMod>::type;


template<class Ident> struct canonical_ident;
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<0 >>, Mod>> { using type = idents::C; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<1 >>, Mod>> { using type = idents::Cs; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<2 >>, Mod>> { using type = idents::D; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<3 >>, Mod>> { using type = idents::Ds; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<4 >>, Mod>> { using type = idents::E; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<5 >>, Mod>> { using type = idents::F; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<6 >>, Mod>> { using type = idents::Fs; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<7 >>, Mod>> { using type = idents::G; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<8 >>, Mod>> { using type = idents::Gs; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<9 >>, Mod>> { using type = idents::A; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<10>>, Mod>> { using type = idents::As; };
template<class Mod> struct canonical_ident<basic_ident<basic_tone<tone_height<11>>, Mod>> { using type = idents::B; };

template<class Ident>
using canonical_ident_t = typename canonical_ident<Ident>::type;


template<class Tone, class Mod>
struct canonical<basic_ident<Tone, Mod>>
{
    using type = canonical_ident_t<basic_ident<Tone, Mod>>;
};


namespace detail {

template<class ToneHeight> struct non_modded_tone_height_impl;
template<> struct non_modded_tone_height_impl<tone_height<0>>  { using type = tone_height<0>; };
template<> struct non_modded_tone_height_impl<tone_height<1>>  { using type = tone_height<0>; };
template<> struct non_modded_tone_height_impl<tone_height<2>>  { using type = tone_height<1>; };
template<> struct non_modded_tone_height_impl<tone_height<3>>  { using type = tone_height<1>; };
template<> struct non_modded_tone_height_impl<tone_height<4>>  { using type = tone_height<2>; };
template<> struct non_modded_tone_height_impl<tone_height<5>>  { using type = tone_height<3>; };
template<> struct non_modded_tone_height_impl<tone_height<6>>  { using type = tone_height<3>; };
template<> struct non_modded_tone_height_impl<tone_height<7>>  { using type = tone_height<4>; };
template<> struct non_modded_tone_height_impl<tone_height<8>>  { using type = tone_height<4>; };
template<> struct non_modded_tone_height_impl<tone_height<9>>  { using type = tone_height<5>; };
template<> struct non_modded_tone_height_impl<tone_height<10>> { using type = tone_height<5>; };
template<> struct non_modded_tone_height_impl<tone_height<11>> { using type = tone_height<6>; };


template<class Tone, class Mod>
struct non_modded_tone_height
{
    using type = typename non_modded_tone_height_impl<
        typename canonical_tone_shift_t<
            Tone, typename opposite_mod_t<Mod>::offset_type
        >::height_type
    >::type;
};

template<class Tone, class Mod>
using non_modded_tone_height_t = typename non_modded_tone_height<Tone, Mod>::type;

} // cvt::detail



template<class Scale>
struct canonical_scale;

template<class OriginallyScaledAs, class... Idents>
struct canonical_scale<basic_scale<OriginallyScaledAs, ident_set<Idents...>>>
{
    // short-circuit for originally-canonically-defined scale
    using type = std::conditional_t<
        std::is_same_v<OriginallyScaledAs, scales::canonical>,
        basic_scale<OriginallyScaledAs, ident_set<Idents...>>,
        basic_scale<
            scales::canonical,
            ident_set<canonical_ident_t<Idents>...>
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


namespace detail {

template<class ToneSet, class ToneOffset>
struct tone_set_shift;

template<class... Tones, class ToneOffset>
struct tone_set_shift<tone_set<Tones...>, ToneOffset>
{
    using type = tone_set<
        canonical_tone_shift_t<Tones, ToneOffset>...
    >;
};

template<class ToneSet, class ToneOffset>
using tone_set_shift_t = typename tone_set_shift<ToneSet, ToneOffset>::type;

} // detail

} // cvt


namespace detail {

template<class Tone, class ToneSet>
struct is_non_modded_height_impl;

template<class Tone, class... CompareToTones>
struct is_non_modded_height_impl<
    Tone,
    tone_set<CompareToTones...>
> : saya::zed::any_of<
    saya::zed::eq,
    saya::zed::template_<typename Tone::height_type, saya::zed::meta_arg>,
    typename CompareToTones::height_type...
>
{};

template<class Tone>
struct is_non_modded_height
    : is_non_modded_height_impl<Tone, make_tone_set<0, 2, 4, 5, 7, 9, 11>>
{};

template<class Tone>
constexpr bool is_non_modded_height_v = is_non_modded_height<Tone>::value;


template<class ToneSet>
struct modded_height_count;

template<class... Tones>
struct modded_height_count<
    tone_set<Tones...>
> : std::integral_constant<
    std::size_t,
    saya::zed::fold_add_v<
        std::size_t,
        0,
        std::conditional_t<
            is_non_modded_height<Tones>::value,
            std::integral_constant<std::size_t, 0>,
            std::integral_constant<std::size_t, 1>
        >::value...
    >
>
{};

template<class ToneSet>
constexpr auto modded_height_count_v = modded_height_count<ToneSet>::value;

} // detail


template<class Tone, class Scale>
struct is_off_scaled;

template<class Tone, class ScaledAs, class... Tones>
struct is_off_scaled<
    Tone, basic_scale<ScaledAs, tone_set<Tones...>>
> : saya::zed::any_of<
    saya::zed::eq,
    saya::zed::template_<typename Tone::height_type, saya::zed::meta_arg>,
    typename Tones::height_type...
>
{
    static_assert(
        !std::is_same_v<ScaledAs, scales::wild>,
        "it does not make sense to check whether a tone is off-scaled in a wild scale"
    );
};

template<class Tone, class Scale>
constexpr bool is_off_scaled_v = is_off_scaled<Tone, Scale>;


template<class ToneSet, class CompareToScale>
struct off_scaled_tone_count;

template<class CompareToScale, class... Tones>
struct off_scaled_tone_count<
    tone_set<Tones...>,
    CompareToScale
> : std::integral_constant<std::size_t, (0 + is_off_scaled<Tones, CompareToScale>::value)...>
{};

template<class ToneSet, class CompareToScale>
constexpr auto off_scaled_tone_count_v = off_scaled_tone_count<ToneSet, CompareToScale>::value;


namespace cvt {
namespace detail {

template<class Tone, class Mod>
struct mod_if_off_scaled
{
    using type = basic_ident<
        Tone,
        std::conditional_t<
            music::detail::is_non_modded_height_v<Tone>,
            mods::none,
            Mod
        >
    >;
};
template<class Tone, class Mod>
using mod_if_off_scaled_t = typename mod_if_off_scaled<Tone, Mod>::type;

} // cvt::detail

} // cvt



template<class KeyIdent>
struct key_sign_mod_count;

template<class Ident>
struct key_sign_mod_count<key_ident<Ident, key_feels::major>>
    : std::integral_constant<
        std::size_t,
        detail::modded_height_count_v<
            cvt::detail::tone_set_shift_t<
                detail::make_tone_set<0, 2, 4, 5, 7, 9, 11>,
                tone_offset<Ident::height>
            >
        >
    >
{};

template<class Ident>
struct key_sign_mod_count<key_ident<Ident, key_feels::minor>>
    : std::integral_constant<
        std::size_t,
        key_sign_mod_count<
            key_ident<
                cvt::detail::canonical_tone_shift_t<
                    typename Ident::tone_type,
                    tone_offset<-9>
                >,
                key_feels::major
            >
        >::value
    >
{};

template<class KeyIdent>
constexpr auto key_sign_mod_count_v = key_sign_mod_count<KeyIdent>::value;


template<class KeyIdent> struct key_sign_mod;
template<> struct key_sign_mod<key_ident<idents::C, key_feels::major>> { using type = mods::none; };
template<> struct key_sign_mod<key_ident<idents::A, key_feels::minor>> { using type = mods::none; };

template<> struct key_sign_mod<key_ident<idents::G, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::E, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::D, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::B, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::A, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Fs, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::E, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Cs, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::B, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Gs, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Fs, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Ds, key_feels::minor>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::Cs, key_feels::major>> { using type = mods::sharp; };
template<> struct key_sign_mod<key_ident<idents::As, key_feels::minor>> { using type = mods::sharp; };

template<> struct key_sign_mod<key_ident<idents::F, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::D, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Bb, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::G, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Eb, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::C, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Ab, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::F, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Db, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Bb, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Gb, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Eb, key_feels::minor>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Cb, key_feels::major>> { using type = mods::flat; };
template<> struct key_sign_mod<key_ident<idents::Ab, key_feels::minor>> { using type = mods::flat; };

template<class KeyIdent>
using key_sign_mod_t = typename key_sign_mod<KeyIdent>::type;


namespace cvt {

template<class KeyIdent, class Target>
struct interpret_in_key;

template<class KeyIdent, class Tone, class UnusedMod>
struct interpret_in_key<KeyIdent, basic_ident<Tone, UnusedMod>>
{
    using type = detail::mod_if_off_scaled_t<
        cvt::detail::canonical_tone_shift_t<
            Tone,
            std::conditional_t<
                std::is_same_v<typename KeyIdent::key_feel, key_feels::major>,
                tone_offset<KeyIdent::height_type::value>,
                tone_offset<cvt::detail::canonical_tone_shift_t<typename KeyIdent::tone_type, tone_offset<-9>>::height>
            >
        >,
        key_sign_mod_t<KeyIdent>
    >;
};

template<class KeyIdent, class ToneHeight>
struct interpret_in_key<KeyIdent, basic_tone<ToneHeight>>
{
    using type = typename interpret_in_key<KeyIdent, basic_ident<basic_tone<ToneHeight>, mods::none>>::tone_type;
};

template<class KeyIdent, class... Tones>
struct interpret_in_key<KeyIdent, tone_set<Tones...>>
{
    using type = tone_set<typename interpret_in_key<KeyIdent, Tones>::tone_type...>;
};

template<class KeyIdent, class... Idents>
struct interpret_in_key<KeyIdent, ident_set<Idents...>>
{
    using type = ident_set<typename interpret_in_key<KeyIdent, Idents>::type...>;
};

template<class KeyIdent, class Target>
using interpret_in_key_t = typename interpret_in_key<KeyIdent, Target>::type;


} // cvt


namespace cvt { namespace detail {

template<class Key>
struct force_enharmonic_key;

template<class Ident, class KeyFeel>
struct force_enharmonic_key<basic_key<key_ident<Ident, KeyFeel>>>
{
    using type = basic_key<key_ident<canonical_ident_t<Ident>, KeyFeel>>;
};

template<class Key>
using force_enharmonic_key_t = typename force_enharmonic_key<Key>::type;

} // detail


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

} // cvt


namespace detail {

template<class Ident> struct sharp_counter : std::integral_constant<std::size_t, 0> {};
template<class Tone> struct sharp_counter<basic_ident<Tone, mods::sharp>> : std::integral_constant<std::size_t, 1> {};
template<class Tone> struct sharp_counter<basic_ident<Tone, mods::dbl_sharp>> : std::integral_constant<std::size_t, 2> {};
template<class Ident> constexpr auto sharp_counter_v = sharp_counter<Ident>::value;

template<class Ident> struct flat_counter : std::integral_constant<std::size_t, 0> {};
template<class Tone> struct flat_counter<basic_ident<Tone, mods::flat>> : std::integral_constant<std::size_t, 1> {};
template<class Tone> struct flat_counter<basic_ident<Tone, mods::dbl_flat>> : std::integral_constant<std::size_t, 2> {};
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
    basic_key<key_ident<Ident, KeyFeel>>,
    std::enable_if_t<
        has_enharmonic_key_v<basic_key<key_ident<Ident, KeyFeel>>>
    >
>
{
    using type = basic_key<
        key_ident<
            basic_ident<
                typename Ident::tone_type,
                std::conditional_t<
                    Ident::is_sharped, mods::flat, mods::sharp
                >
            >,
            KeyFeel
        >
    >;

    static_assert(!std::is_same_v<type, basic_key<key_ident<Ident, KeyFeel>>>, "enharmonic key must not be the same as original key");
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

template<class Tone, class Mod>
struct relative_ident<
    basic_ident<Tone, Mod>,
    key_feels::major
>
{
    using type = basic_ident<canonical_tone_shift_t<Tone, tone_offset<9>>, Mod>;
};
template<class Tone, class Mod>
struct relative_ident<
    basic_ident<Tone, Mod>,
    key_feels::minor
>
{
    using type = basic_ident<canonical_tone_shift_t<Tone, tone_offset<-9>>, Mod>;
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
struct relative_key<basic_key<key_ident<Ident, KeyFeel>>>
{
    using type = basic_key<
        key_ident<
            detail::relative_ident_t<Ident, KeyFeel>,
            opposite_key_feel_t<KeyFeel>
        >
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

template<class... Tones, class... Mods>
struct pack_to_ident_set<
    tone_set<Tones...>,
    mod_set<Mods...>
>
{
    using type = ident_set<basic_ident<Tones, Mods>...>;
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

template<> struct is_diatonic<basic_degree<relative_tone_height<0>>> : std::true_type {};
template<> struct is_diatonic<basic_degree<relative_tone_height<2>>> : std::true_type {};
template<> struct is_diatonic<basic_degree<relative_tone_height<4>>> : std::true_type {};
template<> struct is_diatonic<basic_degree<relative_tone_height<5>>> : std::true_type {};
template<> struct is_diatonic<basic_degree<relative_tone_height<7>>> : std::true_type {};
template<> struct is_diatonic<basic_degree<relative_tone_height<9>>> : std::true_type {};
template<> struct is_diatonic<basic_degree<relative_tone_height<11>>> : std::true_type {};

}} // ompu
